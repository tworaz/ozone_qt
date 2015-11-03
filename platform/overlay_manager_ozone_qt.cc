// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ozone_qt/platform/overlay_manager_ozone_qt.h"

#include "ui/ozone/public/overlay_candidates_ozone.h"

namespace ui {

OverlayManagerOzoneQt::OverlayManagerOzoneQt() {
}

OverlayManagerOzoneQt::~OverlayManagerOzoneQt() {
}

scoped_ptr<OverlayCandidatesOzone>
OverlayManagerOzoneQt::CreateOverlayCandidates(gfx::AcceleratedWidget w) {
  return nullptr;
}

} // namespace ui
