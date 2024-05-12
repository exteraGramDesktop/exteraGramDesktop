/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#include "quiet_mode.h"

#include "rabbit/settings/rabbit_settings.h"

#include "core/application.h"
#include "core/core_settings.h"

void setQuietMode(int duration)
{
    QDateTime now = QDateTime::currentDateTime();
    QDateTime end = now.addSecs(durations_seconds[duration]);

    QString endStr = end.toString(Qt::ISODate);

    RabbitSettings::JsonSettings::Set("quiet_deadline", endStr);
    RabbitSettings::JsonSettings::Write();

    auto& settings = Core::App().settings();
    settings.setDesktopNotify(false);
    settings.setSoundNotify(false);
}

void resetQuietMode()
{
    QDateTime now = QDateTime::currentDateTime();
    QString nowStr = now.toString(Qt::ISODate);
    
    RabbitSettings::JsonSettings::Set("quiet_deadline", nowStr);
    RabbitSettings::JsonSettings::Write();

    auto& settings = Core::App().settings();
    settings.setDesktopNotify(true);
    settings.setSoundNotify(true);
}

bool isQuietNow()
{
    QString endStr = RabbitSettings::JsonSettings::GetString("quiet_deadline");
    QDateTime end = QDateTime::fromString(endStr, Qt::ISODate);

    return end > QDateTime::currentDateTime();
}