/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#include "previews.h"
#include "rabbit/settings/rabbit_settings.h"
#include "core/application.h"
#include "styles/style_layers.h"

#include "main/main_domain.h"
#include "styles/style_rabbit_styles.h"
#include "ui/painter.h"
#include "window/main_window.h"

RoundnessPreview::RoundnessPreview(QWidget *parent) : RpWidget(parent) {
    auto size = st::rndPreviewSize + (2 * st::cpPenSize);
    setMinimumSize(st::boxWidth, size);
}

void RoundnessPreview::paintEvent(QPaintEvent *e)  {
    Painter p(this);
    PainterHighQualityEnabler hq(p);

    auto radius = RabbitSettings::JsonSettings::GetInt("userpic_roundness") / 100.0 * st::rndPreviewSize;

    p.setPen(QPen(st::iconPreviewStroke, st::cpPenSize));
    p.drawRoundedRect(
        st::cpPenSize, st::cpPenSize, 
        st::rndPreviewSize, st::rndPreviewSize, 
        radius, radius);
}
