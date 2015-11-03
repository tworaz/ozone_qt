// Copyright (c) 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OVERLAY_MANAGER_OZONE_QT_H_
#define OVERLAY_MANAGER_OZONE_QT_H_

#include "ui/ozone/public/overlay_manager_ozone.h"

namespace ui {

class OverlayManagerOzoneQt : public OverlayManagerOzone {
 public:
  OverlayManagerOzoneQt();
  ~OverlayManagerOzoneQt() override;

  scoped_ptr<OverlayCandidatesOzone> CreateOverlayCandidates(
      gfx::AcceleratedWidget w) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(OverlayManagerOzoneQt);
};

} // namespace ui

#endif // OVERLAY_MANAGER_OZONE_QT_H_
