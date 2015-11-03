// Copyright 2015 Piotr Tworek. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ozone_qt/components/ime/input_method_qt.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QInputMethodEvent>

#include "ozone_qt/util/type_conversion.h"
#include "ui/base/ime/text_input_client.h"
#include "ui/events/event.h"
#include "ui/events/event_constants.h"

namespace ui {

namespace {

Qt::InputMethodHints ImeHintsFromInputType(ui::TextInputType input_type)
{
  switch (input_type) {
  case ui::TEXT_INPUT_TYPE_NONE:
    return Qt::ImhNone;
  case ui::TEXT_INPUT_TYPE_TEXT:
    return Qt::ImhPreferLowercase;
  case ui::TEXT_INPUT_TYPE_SEARCH:
    return Qt::ImhPreferLowercase |
           Qt::ImhNoAutoUppercase;
  case ui::TEXT_INPUT_TYPE_PASSWORD:
    return Qt::ImhSensitiveData |
           Qt::ImhNoPredictiveText |
           Qt::ImhNoAutoUppercase;
  case ui::TEXT_INPUT_TYPE_EMAIL:
    return Qt::ImhEmailCharactersOnly;
  case ui::TEXT_INPUT_TYPE_NUMBER:
    return Qt::ImhFormattedNumbersOnly;
  case ui::TEXT_INPUT_TYPE_TELEPHONE:
    return Qt::ImhDialableCharactersOnly;
  case ui::TEXT_INPUT_TYPE_URL:
    return Qt::ImhUrlCharactersOnly |
           Qt::ImhNoPredictiveText |
           Qt::ImhNoAutoUppercase;
  case ui::TEXT_INPUT_TYPE_DATE_TIME:
  case ui::TEXT_INPUT_TYPE_DATE_TIME_LOCAL:
  case ui::TEXT_INPUT_TYPE_DATE_TIME_FIELD:
    return Qt::ImhDate | Qt::ImhTime;
  case ui::TEXT_INPUT_TYPE_DATE:
  case ui::TEXT_INPUT_TYPE_MONTH:
  case ui::TEXT_INPUT_TYPE_WEEK:
    return Qt::ImhDate;
  case ui::TEXT_INPUT_TYPE_TIME:
    return Qt::ImhTime;
  case ui::TEXT_INPUT_TYPE_TEXT_AREA:
  case ui::TEXT_INPUT_TYPE_CONTENT_EDITABLE:
    return Qt::ImhMultiLine | Qt::ImhPreferLowercase;
  default:
    LOG(WARNING) << "Unhandled text input type: " << input_type;
    return Qt::ImhNone;
  }
}

} // namespace

InputMethodQt::InputMethodQt(internal::InputMethodDelegate* delegate)
    : input_method_(*qApp->inputMethod()) {
  SetDelegate(delegate);
}

InputMethodQt::~InputMethodQt() {
}

bool InputMethodQt::OnUntranslatedIMEMessage(
    const base::NativeEvent& event,
    NativeEventResult* result) {
  NOTIMPLEMENTED();
  return false;
}

void InputMethodQt::DispatchKeyEvent(ui::KeyEvent* event) {
  DCHECK(event->type() == ET_KEY_PRESSED || event->type() == ET_KEY_RELEASED);

  // If no text input client, do nothing.
  if (!GetTextInputClient()) {
    ignore_result(DispatchKeyEventPostIME(event));
    return;
  }

  // Insert the character.
  ignore_result(DispatchKeyEventPostIME(event));
  if (event->type() == ET_KEY_PRESSED && GetTextInputClient()) {
    const uint16 ch = event->GetCharacter();
    if (ch) {
      GetTextInputClient()->InsertChar(ch, event->flags());
      event->StopPropagation();
    }
  }
}

void InputMethodQt::OnCaretBoundsChanged(const TextInputClient* client) {
}

void InputMethodQt::CancelComposition(const TextInputClient* client) {
  input_method_.reset();
}

void InputMethodQt::OnInputLocaleChanged() {
}

std::string InputMethodQt::GetInputLocale() {
  return input_method_.locale().name().toStdString();
}

bool InputMethodQt::IsCandidatePopupOpen() const {
  return false;
}

void InputMethodQt::OnTextInputTypeChanged(const TextInputClient* client) {
  input_method_.update(Qt::ImQueryInput | Qt::ImEnabled | Qt::ImHints);
  input_method_.setVisible(!IsTextInputTypeNone());
  InputMethodBase::OnTextInputTypeChanged(client);
}

void InputMethodQt::ProcessEvent(QInputMethodEvent& event) {
  QString commit_string = event.commitString();
  QString preedit_string = event.preeditString();
  int replacement_start = event.replacementStart();
  int replacement_length = event.replacementLength();

	if (!GetTextInputClient()) {
    return;
	}

  bool commit = false;
  CompositionText composition_text;
  if (!preedit_string.isEmpty()) {
    composition_text.text = toString16(preedit_string);
  } else if (!commit_string.isEmpty()) {
    composition_text.text = toString16(commit_string);
    commit = true;
  }

  if (replacement_length > 0) {
    composition_text.selection = gfx::Range(replacement_start,
        replacement_start + replacement_length);
  } else {
    composition_text.selection = gfx::Range(composition_text.text.length(),
                                            composition_text.text.length());
  }

  GetTextInputClient()->SetCompositionText(composition_text);
  if (commit) {
    GetTextInputClient()->ConfirmCompositionText();
  }
}

QVariant InputMethodQt::Query(Qt::InputMethodQuery query) const {
  switch (query) {
  case Qt::ImEnabled:
    return QVariant(!IsTextInputTypeNone());
  case Qt::ImHints:
    return QVariant(ImeHintsFromInputType(GetTextInputType()));
  default:
    return QVariant();
  }
}

}  // namespace ui

