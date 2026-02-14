/**************************************************************************/
/*  openxr_android_geospatial_extension.h                                 */
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

#include <androidxr/xr_android_geospatial.h>
#include <godot_cpp/classes/open_xr_extension_wrapper.hpp>
#include <map>

#include "util.h"

using namespace godot;

class OpenXRAndroidGeospatialExtension : public OpenXRExtensionWrapper {
	GDCLASS(OpenXRAndroidGeospatialExtension, OpenXRExtensionWrapper);

public:
	static OpenXRAndroidGeospatialExtension *get_singleton();

	OpenXRAndroidGeospatialExtension();
	~OpenXRAndroidGeospatialExtension();

	godot::Dictionary _get_requested_extensions(uint64_t p_xr_version) override;

	void _on_instance_created(uint64_t p_instance) override;
	void _on_instance_destroyed() override;
	void _on_session_created(uint64_t p_instance) override;

	// Bind methods functions

protected:
	static void _bind_methods();

private:
	EXT_PROTO_XRRESULT_FUNC3(xrCreateGeospatialTrackerANDROID,
			(XrSession), session,
			(const XrGeospatialTrackerCreateInfoANDROID *), createInfo,
			(XrGeospatialTrackerANDROID *), geospatialTrackerOutput)

	EXT_PROTO_XRRESULT_FUNC1(xrDestroyGeospatialTrackerANDROID,
			(XrGeospatialTrackerANDROID), geospatialTracker);

	EXT_PROTO_XRRESULT_FUNC3(xrLocateGeospatialPoseFromPoseANDROID,
			(XrGeospatialTrackerANDROID), geospatialTracker,
			(const XrGeospatialPoseFromPoseLocateInfoANDROID *), locateInfo,
			(XrGeospatialPoseResultANDROID *), geospatialPoseResult);

	EXT_PROTO_XRRESULT_FUNC3(xrLocateGeospatialPoseANDROID,
			(XrGeospatialTrackerANDROID), geospatialTracker,
			(const XrGeospatialPoseLocateInfoANDROID *), locateInfo,
			(XrSpaceLocation *), location);

	EXT_PROTO_XRRESULT_FUNC4(xrCheckVpsAvailabilityAsyncANDROID,
			(XrSession), session,
			(double), latitude,
			(double), longitude,
			(XrFutureEXT *), future);

	EXT_PROTO_XRRESULT_FUNC3(xrCheckVpsAvailabilityCompleteANDROID,
			(XrSession), session,
			(XrFutureEXT), future,
			(XrVPSAvailabilityCheckCompletionANDROID *), completion);

	bool initialize_android_geospatial_extension(XrInstance p_instance);

	void cleanup();

	static OpenXRAndroidGeospatialExtension *singleton;

	std::map<godot::String, bool *> request_extensions;
	bool androidxr_geospatial_ext = false;

	// Handles
};
