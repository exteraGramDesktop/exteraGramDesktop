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
    auto sectionHeight = st::rndPreviewSize;
    setMinimumSize(st::boxWidth, sectionHeight);
}

void RoundnessPreview::paintEvent(QPaintEvent *e)  {
    Painter p(this);
    PainterHighQualityEnabler hq(p);

    auto size = st::rndPreviewSize;
    auto radius = size * (RabbitSettings::JsonSettings::GetInt("userpic_roundness") / 100.);

    p.setPen(Qt::NoPen);
    p.setBrush(QBrush(st::rndPreviewFill));
    p.drawRoundedRect(
        0, 0,
        size, size, 
        radius, radius
    );

    p.setBrush(QBrush(st::rndSkeletonFill));
    auto skeletonWidth = st::boxWidth - (3 * st::rndPreviewSize);
    auto skeletonHeight = st::rndPreviewSize / 5;
    p.drawRoundedRect(
        st::rndPreviewSize * 1.33,
        skeletonHeight,
        skeletonWidth / 2,
        skeletonHeight,
        skeletonHeight / 2,
        skeletonHeight / 2
    );

    p.drawRoundedRect(
        st::rndPreviewSize * 1.33,
        skeletonHeight * 3,
        skeletonWidth,
        skeletonHeight,
        skeletonHeight / 2,
        skeletonHeight / 2
    );
}
