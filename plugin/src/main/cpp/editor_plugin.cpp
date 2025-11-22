/**************************************************************************/
/*  editor_plugin.cpp                                                     */
/**************************************************************************/
/*                       This file is part of:                            */
/*                              GODOT XR                                  */
/*                      https://godotengine.org                           */
/**************************************************************************/
/* Copyright (c) 2022-present Godot XR contributors (see CONTRIBUTORS.md) */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "editor_plugin.h"

#include "editor/xr_project_setup_dialog.h"
#include "export/android_xr_export_plugin.h"
#include "export/khronos_export_plugin.h"
#include "export/lynx_export_plugin.h"
#include "export/magicleap_export_plugin.h"
#include "export/meta_export_plugin.h"
#include "export/pico_export_plugin.h"

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/http_request.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

OpenXRVendorsEditorPlugin *OpenXRVendorsEditorPlugin::singleton = nullptr;

OpenXRVendorsEditorPlugin *OpenXRVendorsEditorPlugin::get_singleton() {
	return singleton;
}

void OpenXRVendorsEditorPlugin::_add_export_plugin(const Ref<EditorExportPlugin> &p_plugin) {
	add_export_plugin(p_plugin);
	export_plugins.push_back(p_plugin);
}

void OpenXRVendorsEditorPlugin::_bind_methods() {
}

void OpenXRVendorsEditorPlugin::_notification(uint32_t p_what) {
	switch (p_what) {
		case NOTIFICATION_POSTINITIALIZE: {
			_xr_project_setup_dialog = memnew(XrProjectSetupDialog);
			add_child(_xr_project_setup_dialog);
		} break;

		case NOTIFICATION_ENTER_TREE: {
			add_tool_menu_item("XR Project Setup Wizard...", callable_mp(this, &OpenXRVendorsEditorPlugin::_open_project_setup));

			Ref<KhronosEditorExportPlugin> khronos_export_plugin;
			khronos_export_plugin.instantiate();
			_add_export_plugin(khronos_export_plugin);

			Ref<LynxEditorExportPlugin> lynx_export_plugin;
			lynx_export_plugin.instantiate();
			_add_export_plugin(lynx_export_plugin);

			Ref<AndroidXREditorExportPlugin> android_xr_export_plugin;
			android_xr_export_plugin.instantiate();
			_add_export_plugin(android_xr_export_plugin);

			Ref<MagicleapEditorExportPlugin> magicleap_export_plugin;
			magicleap_export_plugin.instantiate();
			_add_export_plugin(magicleap_export_plugin);

			Ref<MetaEditorExportPlugin> meta_export_plugin;
			meta_export_plugin.instantiate();
			_add_export_plugin(meta_export_plugin);

			Ref<PicoEditorExportPlugin> pico_export_plugin;
			pico_export_plugin.instantiate();
			_add_export_plugin(pico_export_plugin);
		} break;

		case NOTIFICATION_EXIT_TREE: {
			remove_tool_menu_item("XR Project Setup Wizard...");

			for (const Ref<EditorExportPlugin> &export_plugin : export_plugins) {
				remove_export_plugin(export_plugin);
			}
			export_plugins.clear();
		} break;
	}
}

void OpenXRVendorsEditorPlugin::_open_project_setup() {
	_xr_project_setup_dialog->open();
}

void OpenXRVendorsEditorPlugin::open_asset_library(const String &p_search_string) {
	SceneTree *tree = get_tree();
	if (tree == nullptr) {
		return;
	}

	Window *root = tree->get_root();
	if (root == nullptr) {
		return;
	}

	Node *asset_lib = root->find_child("@EditorAssetLibrary@*", true, false);
	if (asset_lib == nullptr) {
		return;
	}

	Node *lib_main_vbox = asset_lib->find_child("@VBoxContainer@*", false, false);
	if (lib_main_vbox == nullptr) {
		return;
	}

	// Asset Library node has multiple HBoxContainer children, but the search container is added first.
	Node *search_hb = lib_main_vbox->get_child(0);
	if (search_hb == nullptr) {
		return;
	}

	LineEdit *asset_library_filter = Object::cast_to<LineEdit>(search_hb->find_child("@LineEdit@*", false, false));
	if (asset_library_filter == nullptr) {
		return;
	}

	Button *asset_lib_button = Object::cast_to<Button>(root->find_child("AssetLib", true, false));
	if (asset_lib_button == nullptr) {
		return;
	}

	asset_lib_button->emit_signal("pressed");

	if (!asset_library_filter->is_editable()) {
		HTTPRequest *asset_lib_request = Object::cast_to<HTTPRequest>(asset_lib->find_child("*HTTPRequest*", true, false));
		asset_lib_request->connect("request_completed", callable_mp(this, &OpenXRVendorsEditorPlugin::_on_asset_library_request_completed).bind(asset_library_filter, p_search_string), CONNECT_ONE_SHOT);
	} else {
		asset_library_filter->set_text(p_search_string);
		asset_library_filter->emit_signal("text_changed", asset_library_filter->get_text());
	}
}

void OpenXRVendorsEditorPlugin::_on_asset_library_request_completed(int p_result, int p_response_code, const PackedStringArray &p_headers, const PackedByteArray &p_body, LineEdit *p_asset_library_filter, String p_search_string) {
	if (p_response_code != 200) {
		UtilityFunctions::print_verbose(vformat("Asset Library HTTPRequest returned with response code %s", p_response_code));
		return;
	}

	if (p_asset_library_filter == nullptr) {
		UtilityFunctions::print_verbose("No Asset Library LineEdit node found");
		return;
	}

	p_asset_library_filter->set_text(p_search_string);
	p_asset_library_filter->emit_signal("text_changed", p_asset_library_filter->get_text());
}

OpenXRVendorsEditorPlugin::OpenXRVendorsEditorPlugin() {
	ERR_FAIL_COND_MSG(singleton != nullptr, "An OpenXRVendorsEditorPlugin singleton already exists");
	singleton = this;
}

OpenXRVendorsEditorPlugin::~OpenXRVendorsEditorPlugin() {
	singleton = nullptr;
}
