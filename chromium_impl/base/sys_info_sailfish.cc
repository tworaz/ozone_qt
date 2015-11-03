// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/sys_info.h"

#include "base/lazy_instance.h"
#include "base/native_library.h"
#include "base/strings/stringprintf.h"
#include "base/sys_info_internal.h"

#define PROP_VALUE_MAX 92

namespace base {

namespace {

const char kDefaultAndroidVersionStr[] = "4.1.2";
const char kHybrisPropertyLibrary[] = "libandroid-properties.so.1";
const int kDefaultAndroidMajorVersion = 4;
const int kDefaultAndroidMinorVersion = 1;
const int kDefaultAndroidBugfixVersion = 2;

void ParseOSVersionNumbers(const char* os_version_str,
                           int32 *major_version,
                           int32 *minor_version,
                           int32 *bugfix_version) {
  if (os_version_str[0]) {
    // Try to parse out the version numbers from the string.
    int num_read = sscanf(os_version_str, "%d.%d.%d", major_version,
                          minor_version, bugfix_version);

    if (num_read > 0) {
      // If we don't have a full set of version numbers, make the extras 0.
      if (num_read < 2) *minor_version = 0;
      if (num_read < 3) *bugfix_version = 0;
      return;
    }
  }

  // For some reason, we couldn't parse the version number string.
  *major_version = kDefaultAndroidMajorVersion;
  *minor_version = kDefaultAndroidMinorVersion;
  *bugfix_version = kDefaultAndroidBugfixVersion;
}

typedef int (*PropertyGetFunction)(const char*, char*, const char*);

PropertyGetFunction DynamicallyLoadHybrisPropertyGet() {
  base::NativeLibrary prop_library;
  base::NativeLibraryLoadError error;
  prop_library = base::LoadNativeLibrary(
			base::FilePath(kHybrisPropertyLibrary), &error);
  if (!prop_library)
    LOG(FATAL) << "Failed to load " << kHybrisPropertyLibrary;

  auto system_property_get =
		  reinterpret_cast<PropertyGetFunction>(
					base::GetFunctionPointerFromNativeLibrary(prop_library,
						                                        "property_get"));
	if (!system_property_get) {
    LOG(FATAL) << "Can't resolve property_get()";
	}

	return system_property_get;
}

static base::LazyInstance<base::internal::LazySysInfoValue<
    PropertyGetFunction, DynamicallyLoadHybrisPropertyGet> >::Leaky
    g_lazy_hybris_property_get = LAZY_INSTANCE_INITIALIZER;

int hybris_property_get(const char* name, char* value, const char* def_val) {
  return g_lazy_hybris_property_get.Get().value()(name, value, def_val);
}

} // namespace

// static
std::string SysInfo::OperatingSystemName() {
	return "Android";
}

// static
std::string SysInfo::OperatingSystemVersion() {
  int32 major, minor, bugfix;
  OperatingSystemVersionNumbers(&major, &minor, &bugfix);
  return StringPrintf("%d.%d.%d", major, minor, bugfix);
}

void SysInfo::OperatingSystemVersionNumbers(int32* major_version,
                                            int32* minor_version,
                                            int32* bugfix_version) {
  // Read the version number string out from the properties.
  char os_version_str[PROP_VALUE_MAX];
  hybris_property_get("ro.build.version.release", os_version_str,
			                kDefaultAndroidVersionStr);

  // Parse out the numbers.
  ParseOSVersionNumbers(os_version_str, major_version, minor_version,
                        bugfix_version);
}

} // namespace base
