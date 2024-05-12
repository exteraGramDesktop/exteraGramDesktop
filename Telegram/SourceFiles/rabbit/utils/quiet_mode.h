/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "rabbit/lang/rabbit_lang.h"


#include <vector>
#include <QString>
#include "rpl/producer.h"

inline auto durations_seconds = std::vector<int>{
    30 * 60,
    1 * 60 * 60,
    3 * 60 * 60,
    8 * 60 * 60,
    24 * 60 * 60,
};

inline std::vector<rpl::producer<QString>> durationss_strings = {
    rktr("rtg_quiet_30min"),
    rktr("rtg_quiet_1h"),
    rktr("rtg_quiet_3h"),
    rktr("rtg_quiet_8h"),
    rktr("rtg_quiet_24h"),
};

void setQuietMode(int duration);
void resetQuietMode();
bool isQuietNow();