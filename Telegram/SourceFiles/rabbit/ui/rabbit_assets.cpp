/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#include "rabbit/ui/rabbit_assets.h"
#include "rabbit/settings/rabbit_settings.h"

static QString LAST_LOADED_NAME;

static QImage LAST_LOADED;

static QImage LAST_LOADED_NO_MARGIN;

void loadAppIco()
{
  QString appDataPath = QDir::fromNativeSeparators(qgetenv("APPDATA"));
  QString tempIconPath = appDataPath + "/rabbitGram.ico";

  // workaround for read-only file
  auto f = QFile(tempIconPath);
  if (f.exists()) {
    f.setPermissions(QFile::WriteOther);
    f.remove();
  }
  f.close();
  QFile::copy(qsl(":/gui/art/rabbit/%1/app_icon.ico").arg(RabbitSettings::JsonSettings::GetString("app_icon")), tempIconPath);
}

void loadIcons()
{
  if (LAST_LOADED_NAME != RabbitSettings::JsonSettings::GetString("app_icon")) {
    LAST_LOADED_NAME = RabbitSettings::JsonSettings::GetString("app_icon");

    LAST_LOADED = QImage(qsl(":/gui/art/rabbit/%1/app.png").arg(RabbitSettings::JsonSettings::GetString("app_icon")));
    LAST_LOADED_NO_MARGIN = QImage(qsl(":/gui/art/rabbit/%1/app_preview.png").arg(RabbitSettings::JsonSettings::GetString("app_icon")));
  }
}

QImage loadPreview(QString name)
{
  return QImage(qsl(":/gui/art/rabbit/%1/app_preview.png").arg(name));
}

QString currentAppLogoName()
{
  return LAST_LOADED_NAME;
}

QImage currentAppLogo()
{
  loadIcons();
  return LAST_LOADED;
}

QImage currentAppLogoNoMargin()
{
  loadIcons();
  return LAST_LOADED_NO_MARGIN;
}