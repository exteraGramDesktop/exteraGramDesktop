/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#include "icon_picker.h"
#include "rabbit/settings/rabbit_settings.h"
#include "core/application.h"
#include "styles/style_layers.h"
#include "tray.h"

#include "rabbit/ui/rabbit_assets.h"
#include "main/main_domain.h"
#include "styles/style_rabbit_styles.h"
#include "ui/painter.h"
#include "window/main_window.h"

#ifdef Q_OS_WIN
#include "rabbit/utils/windows_utils.h"
#endif

const QVector<QString> icons{
		RabbitSettings::DEFAULT_ICON,
		RabbitSettings::ANGEL_ICON,
		RabbitSettings::COLOR_ICON,
		RabbitSettings::IMPOSTOR_ICON,
		RabbitSettings::MATRIX_ICON,
		RabbitSettings::MOON_ICON,
		RabbitSettings::OLD_ICON,
};

const auto rows = static_cast<int>(icons.size()) / 4 + std::min(1, static_cast<int>(icons.size()) % 4);

void drawIcon(QPainter &p, const QImage &icon, int xOffset, int yOffset, bool selected)
{
	xOffset += st::cpPadding;

	if (selected) {
		p.save();
		p.setPen(QPen(st::iconPreviewStroke, st::cpPenSize));
		p.drawEllipse(xOffset + st::cpSelectedPadding,
					  yOffset + st::cpSelectedPadding,
					  st::cpIconSize + st::cpSelectedPadding * 2,
					  st::cpIconSize + st::cpSelectedPadding * 2);
		p.restore();
	}

	auto rect = QRect(xOffset + st::cpImagePadding, yOffset + st::cpImagePadding, st::cpIconSize, st::cpIconSize);
	p.drawImage(rect, icon);
}

IconPicker::IconPicker(QWidget *parent)
		: RpWidget(parent)
{
	setMinimumSize(st::boxWidth, (st::cpIconSize + st::cpPadding) * rows);
}

void IconPicker::paintEvent(QPaintEvent *e)
{
	Painter p(this);
	PainterHighQualityEnabler hq(p);

	auto offset = st::boxWidth / 2 - (st::cpIconSize + st::cpSpacingX) * 2;

	for (int row = 0; row < rows; row++) {
		const auto columns = std::min(4, static_cast<int>(icons.size()) - row * 4);
		for (int i = 0; i < columns; i++) {
			auto const idx = i + row * 4;

			const auto &iconName = icons[idx];
			auto icon = loadPreview(iconName)
				.scaled(st::cpIconSize, st::cpIconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			drawIcon(p,
					 icon,
					 (st::cpIconSize + st::cpSpacingX) * i + offset,
					 row * (st::cpIconSize + st::cpSpacingY),
					 currentAppLogoName() == iconName);
		}
	}
}

void IconPicker::mousePressEvent(QMouseEvent *e)
{
	auto appIcon = RabbitSettings::JsonSettings::GetString("app_icon");
	auto changed = false;

	auto x = e->pos().x();
	for (int row = 0; row < rows; row++) {
		const auto columns = std::min(4, static_cast<int>(icons.size()) - row * 4);
		for (int i = 0; i < columns; i++) {
			auto const idx = i + row * 4;
			auto const xOffset = (st::cpIconSize + st::cpSpacingX) * i + st::cpPadding;
			auto const yOffset = row * (st::cpIconSize + st::cpSpacingY);

			if (x >= xOffset && x <= xOffset + st::cpIconSize && e->pos().y() >= yOffset
				&& e->pos().y() <= yOffset + st::cpIconSize) {
				const auto &iconName = icons[idx];
				if (appIcon != iconName) {
					RabbitSettings::JsonSettings::Set("app_icon", iconName);
					changed = true;
					break;
				}
			}
		}
	}

	if (changed) {
#ifdef Q_OS_WIN
		loadAppIco();
		reloadAppIconFromTaskBar();
#endif

		Window::OverrideApplicationIcon(currentAppLogo());
		Core::App().refreshApplicationIcon();
		Core::App().tray().updateIconCounters();
		Core::App().domain().notifyUnreadBadgeChanged();

		repaint();
	}
}