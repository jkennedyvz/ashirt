// Copyright 2020, Verizon Media
// Licensed under the terms of GPLv3. See LICENSE file in project root for terms.

#include "hotkeymanager.h"

#include <QString>
#include <iostream>

#include "appconfig.h"
#include "appsettings.h"
#include "helpers/screenshot.h"

HotkeyManager::HotkeyManager(Screenshot* ss) {
  hotkeyManager = new UGlobalHotkeys();
  screenshotTool = ss;
  connect(hotkeyManager, &UGlobalHotkeys::activated, this, &HotkeyManager::hotkeyTriggered);
}

HotkeyManager::~HotkeyManager() { delete hotkeyManager; }

void HotkeyManager::registerKey(const QString& binding, GlobalHotkeyEvent evt) {
  hotkeyManager->registerHotkey(binding, size_t(evt));
}

void HotkeyManager::unregisterKey(GlobalHotkeyEvent evt) {
  hotkeyManager->unregisterHotkey(size_t(evt));
}

void HotkeyManager::hotkeyTriggered(size_t hotkeyIndex) {
  if (hotkeyIndex == ACTION_CAPTURE_AREA) {
    screenshotTool->captureArea();
  }
  else if (hotkeyIndex == ACTION_CAPTURE_WINDOW) {
    screenshotTool->captureWindow();
  }
}

void HotkeyManager::updateHotkeys() {
  hotkeyManager->unregisterAllHotkeys();
  if (!AppSettings::getInstance().isOperationPaused()) {
    auto shortcut = AppConfig::getInstance().screenshotShortcutCombo;
    if (shortcut != "") {
      registerKey(shortcut, ACTION_CAPTURE_AREA);
    }

    shortcut = AppConfig::getInstance().captureWindowShortcut;
    if (shortcut != "") {
      registerKey(shortcut, ACTION_CAPTURE_WINDOW);
    }
  }
}