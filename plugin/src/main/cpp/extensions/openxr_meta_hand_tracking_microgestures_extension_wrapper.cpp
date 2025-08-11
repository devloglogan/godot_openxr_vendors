/**************************************************************************/
/*  openxr_meta_hand_tracking_microgestures_extension_wrapper.cpp         */
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

#include "extensions/openxr_meta_hand_tracking_microgestures_extension_wrapper.h"
#include "openxr/openxr.h"
#include "util.h"

#include <godot_cpp/classes/open_xrapi_extension.hpp>

using namespace godot;

OpenXRMetaHandTrackingMicrogesturesExtensionWrapper *OpenXRMetaHandTrackingMicrogesturesExtensionWrapper::singleton = nullptr;

OpenXRMetaHandTrackingMicrogesturesExtensionWrapper *OpenXRMetaHandTrackingMicrogesturesExtensionWrapper::get_singleton() {
	if (singleton == nullptr) {
		singleton = memnew(OpenXRMetaHandTrackingMicrogesturesExtensionWrapper());
	}
	return singleton;
}

OpenXRMetaHandTrackingMicrogesturesExtensionWrapper::OpenXRMetaHandTrackingMicrogesturesExtensionWrapper() :
		OpenXRExtensionWrapperExtension() {
	ERR_FAIL_COND_MSG(singleton != nullptr, "An OpenXRMetaHandTrackingMicrogesturesExtensionWrapper singleton already exists.");

	request_extensions[XR_META_HAND_TRACKING_MICROGESTURES_EXTENSION_NAME] = &meta_hand_tracking_microgestures_ext;
	singleton = this;
}

OpenXRMetaHandTrackingMicrogesturesExtensionWrapper::~OpenXRMetaHandTrackingMicrogesturesExtensionWrapper() {
	cleanup();
	singleton = nullptr;
}

godot::Dictionary OpenXRMetaHandTrackingMicrogesturesExtensionWrapper::_get_requested_extensions() {
	godot::Dictionary result;
	for (auto ext : request_extensions) {
		godot::String key = ext.first;
		uint64_t value = reinterpret_cast<uint64_t>(ext.second);
		result[key] = (godot::Variant)value;
	}
	return result;
}

void OpenXRMetaHandTrackingMicrogesturesExtensionWrapper::_on_instance_destroyed() {
	cleanup();
}

void OpenXRMetaHandTrackingMicrogesturesExtensionWrapper::_bind_methods() {}

void OpenXRMetaHandTrackingMicrogesturesExtensionWrapper::cleanup() {
	meta_hand_tracking_microgestures_ext = false;
}
