/*
This file is part of exteraGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/exteraGramDesktop/exteraGramDesktop/blob/dev/LEGAL
*/
#pragma once

#include "platform/platform_specific.h"
#include "base/platform/win/base_windows_h.h"

namespace Data {
class LocationPoint;
} // namespace Data

namespace Platform {

inline void IgnoreApplicationActivationRightNow() {
}

inline bool TrayIconSupported() {
	return true;
}

inline bool SkipTaskbarSupported() {
	return true;
}

inline bool PreventsQuit(Core::QuitReason reason) {
	return false;
}

inline void ActivateThisProcess() {
}

// 1 - secondary, 2 - primary.
void SetWindowPriority(not_null<QWidget*> window, uint32 priority);

[[nodiscard]] uint64 ActivationWindowId(not_null<QWidget*> window);

// Activate window with windowId (if found) or the largest priority.
void ActivateOtherProcess(uint64 processId, uint64 windowId);

inline QString ExecutablePathForShortcuts() {
	return cExeDir() + cExeName();
}

namespace ThirdParty {

void start();

inline void finish() {
}

} // namespace ThirdParty
} // namespace Platform

inline void psCheckLocalSocket(const QString &) {
}

QString psAppDataPath();
QString psAppDataPathOld();
void psSendToMenu(bool send, bool silent = false);

int psCleanup();
int psFixPrevious();

inline QByteArray psDownloadPathBookmark(const QString &path) {
	return QByteArray();
}
inline void psDownloadPathEnableAccess() {
}

bool psLaunchMaps(const Data::LocationPoint &point);
