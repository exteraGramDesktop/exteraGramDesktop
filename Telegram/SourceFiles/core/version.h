/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "base/const_string.h"

#define TDESKTOP_REQUESTED_ALPHA_VERSION (0ULL)

#ifdef TDESKTOP_ALLOW_CLOSED_ALPHA
#define TDESKTOP_ALPHA_VERSION TDESKTOP_REQUESTED_ALPHA_VERSION
#else // TDESKTOP_ALLOW_CLOSED_ALPHA
#define TDESKTOP_ALPHA_VERSION (0ULL)
#endif // TDESKTOP_ALLOW_CLOSED_ALPHA

// used in Updater.cpp and Setup.iss for Windows
<<<<<<< HEAD
constexpr auto AppId = "{4356CE01-4137-4C55-9F8B-FB4EEBB6EC0C}"_cs;
constexpr auto AppNameOld = "rabbitGram Win (Unofficial)"_cs;
constexpr auto AppName = "rabbitGram Desktop"_cs;
constexpr auto AppFile = "rabbitGram"_cs;
constexpr auto AppVersion = 5000001;
constexpr auto AppVersionStr = "5.0.1";
=======
constexpr auto AppId = "{53F49750-6209-4FBF-9CA8-7A333C87D1ED}"_cs;
constexpr auto AppNameOld = "Telegram Win (Unofficial)"_cs;
constexpr auto AppName = "Telegram Desktop"_cs;
constexpr auto AppFile = "Telegram"_cs;
constexpr auto AppVersion = 5001005;
constexpr auto AppVersionStr = "5.1.5";
>>>>>>> 6c0ea0eb9f666fce471ed32efe0e83d9c62aba48
constexpr auto AppBetaVersion = false;
constexpr auto AppAlphaVersion = TDESKTOP_ALPHA_VERSION;
