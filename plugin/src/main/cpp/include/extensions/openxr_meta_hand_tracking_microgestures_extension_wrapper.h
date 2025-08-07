/**************************************************************************/
/*  openxr_meta_hand_tracking_microgestures_extension_wrapper.h           */
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

#pragma once

#include <openxr/openxr.h>
#include <godot_cpp/classes/open_xr_extension_wrapper_extension.hpp>
#include <map>

using namespace godot;

class OpenXRMetaHandTrackingMicrogesturesExtensionWrapper : public OpenXRExtensionWrapperExtension {
	GDCLASS(OpenXRMetaHandTrackingMicrogesturesExtensionWrapper, OpenXRExtensionWrapperExtension);

public:
	static OpenXRMetaHandTrackingMicrogesturesExtensionWrapper *get_singleton();

	OpenXRMetaHandTrackingMicrogesturesExtensionWrapper();
	~OpenXRMetaHandTrackingMicrogesturesExtensionWrapper();

	godot::Dictionary _get_requested_extensions() override;

	uint64_t _set_system_properties_and_get_next_pointer(void *p_next_pointer) override;
	void _on_instance_destroyed() override;

protected:
	static void _bind_methods();

private:
	void cleanup();

	static OpenXRMetaHandTrackingMicrogesturesExtensionWrapper *singleton;

	std::map<godot::String, bool *> request_extensions;
	bool meta_hand_tracking_microgestures_ext = false;
};
