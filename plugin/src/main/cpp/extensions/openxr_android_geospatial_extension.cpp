/**************************************************************************/
/*  openxr_android_geospatial_extension.cpp                               */
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

#include "extensions/openxr_android_geospatial_extension.h"

#include <godot_cpp/classes/open_xrapi_extension.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

OpenXRAndroidGeospatialExtension *OpenXRAndroidGeospatialExtension::singleton = nullptr;

OpenXRAndroidGeospatialExtension *OpenXRAndroidGeospatialExtension::get_singleton() {
	if (singleton == nullptr) {
		singleton = memnew(OpenXRAndroidGeospatialExtension());
	}
	return singleton;
}

OpenXRAndroidGeospatialExtension::OpenXRAndroidGeospatialExtension() :
		OpenXRExtensionWrapper() {
	ERR_FAIL_COND_MSG(singleton != nullptr, "An OpenXRAndroidGeospatialExtension singleton already exists.");

	request_extensions[XR_ANDROID_GEOSPATIAL_EXTENSION_NAME] = &androidxr_geospatial_ext;
	singleton = this;
}

OpenXRAndroidGeospatialExtension::~OpenXRAndroidGeospatialExtension() {
	cleanup();
	singleton = nullptr;
}

godot::Dictionary OpenXRAndroidGeospatialExtension::_get_requested_extensions(uint64_t p_xr_version) {
	godot::Dictionary result;
	for (auto ext : request_extensions) {
		godot::String key = ext.first;
		uint64_t value = reinterpret_cast<uint64_t>(ext.second);
		result[key] = (godot::Variant)value;
	}
	return result;
}

void OpenXRAndroidGeospatialExtension::_on_instance_created(uint64_t p_instance) {
	if (androidxr_geospatial_ext) {
		bool result = initialize_android_geospatial_extension((XrInstance)p_instance);
		if (!result) {
			UtilityFunctions::print("Failed to initialize android_geospatial extension");
			androidxr_geospatial_ext = false;
		}
	}
}

void OpenXRAndroidGeospatialExtension::_on_instance_destroyed() {
	cleanup();
}

void OpenXRAndroidGeospatialExtension::_on_session_created(uint64_t p_instance) {
	if (!androidxr_geospatial_ext) {
		return;
	}

	// TODO Initialize needed handles?
}

void OpenXRAndroidGeospatialExtension::_bind_methods() {
}

void OpenXRAndroidGeospatialExtension::cleanup() {
	androidxr_geospatial_ext = false;
}

bool OpenXRAndroidGeospatialExtension::initialize_android_geospatial_extension(XrInstance p_instance) {
	GDEXTENSION_INIT_XR_FUNC_V(xrCreateGeospatialTrackerANDROID);
	GDEXTENSION_INIT_XR_FUNC_V(xrDestroyGeospatialTrackerANDROID);
	GDEXTENSION_INIT_XR_FUNC_V(xrLocateGeospatialPoseFromPoseANDROID);
	GDEXTENSION_INIT_XR_FUNC_V(xrLocateGeospatialPoseANDROID);
	GDEXTENSION_INIT_XR_FUNC_V(xrCheckVpsAvailabilityAsyncANDROID);
	GDEXTENSION_INIT_XR_FUNC_V(xrCheckVpsAvailabilityCompleteANDROID);

	return true;
}
