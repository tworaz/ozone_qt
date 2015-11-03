// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef OZONE_QT_COPONENTS_IME_INPUT_METHOD_QT_H_
#define OZONE_QT_COPONENTS_IME_INPUT_METHOD_QT_H_

#include <QtCore/QVariant>

#include "ui/base/ime/input_method_base.h"

class QInputMethod;
class QInputMethodEvent;

namespace ui {

class InputMethodQt : public InputMethodBase {
 public:
  explicit InputMethodQt(internal::InputMethodDelegate* delegate);
  ~InputMethodQt() override;

  // Overriden from InputMethod.
  bool OnUntranslatedIMEMessage(const base::NativeEvent& event,
                                NativeEventResult* result) override;
  void DispatchKeyEvent(ui::KeyEvent* event) override;
  void OnCaretBoundsChanged(const TextInputClient* client) override;
  void CancelComposition(const TextInputClient* client) override;
  void OnInputLocaleChanged() override;
  std::string GetInputLocale() override;
  bool IsCandidatePopupOpen() const override;
  void OnTextInputTypeChanged(const TextInputClient* client) override;

  void ProcessEvent(QInputMethodEvent& event);
  QVariant Query(Qt::InputMethodQuery) const;

 private:
  QInputMethod& input_method_;

  DISALLOW_COPY_AND_ASSIGN(InputMethodQt);
};

}  // namespace ui

#endif  // OZONE_QT_COPONENTS_IME_INPUT_METHOD_QT_H_
