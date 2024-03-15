/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#include <ui/boxes/single_choice_box.h>

#include "rabbit/settings/rabbit_settings.h"
#include "rabbit/lang/rabbit_lang.h"
#include "rabbit/settings_menu/sections/rabbit_chats.h"
#include "rabbit/ui/settings/icon_picker.h"

#include "lang_auto.h"
#include "mainwindow.h"
#include "settings/settings_common.h"
#include "ui/wrap/vertical_layout.h"
#include "ui/wrap/slide_wrap.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/widgets/checkbox.h"
#include "ui/vertical_list.h"
#include "boxes/connection_box.h"
#include "platform/platform_specific.h"
#include "window/window_session_controller.h"
#include "lang/lang_instance.h"
#include "core/application.h"
#include "storage/localstorage.h"
#include "data/data_session.h"
#include "main/main_session.h"
#include "styles/style_settings.h"
#include "styles/style_layers.h"
#include "styles/style_menu_icons.h"
#include "apiwrap.h"
#include "api/api_blocked_peers.h"
#include "ui/widgets/continuous_sliders.h"

#define SettingsMenuJsonSwitch(LangKey, Option) container->add(object_ptr<Button>( \
	container, \
	rktr(#LangKey), \
	st::settingsButtonNoIcon \
))->toggleOn( \
	rpl::single(::RabbitSettings::JsonSettings::GetBool(#Option)) \
)->toggledValue( \
) | rpl::filter([](bool enabled) { \
	return (enabled != ::RabbitSettings::JsonSettings::GetBool(#Option)); \
}) | rpl::start_with_next([](bool enabled) { \
	::RabbitSettings::JsonSettings::Set(#Option, enabled); \
	::RabbitSettings::JsonSettings::Write(); \
}, container->lifetime());

namespace Settings {

    rpl::producer<QString> RabbitChats::title() {
        return rktr("rtg_settings_chats");
    }

    RabbitChats::RabbitChats(
            QWidget *parent,
            not_null<Window::SessionController *> controller)
            : Section(parent) {
        setupContent(controller);
    }

    void RabbitChats::SetupChats(not_null<Ui::VerticalLayout *> container) {
        Ui::AddSubsectionTitle(container, rktr("rtg_settings_chats"));

        const auto stickerHeightLabel = container->add(
		    object_ptr<Ui::LabelSimple>(
			    container,
			    st::settingsAudioVolumeLabel),
		    st::settingsAudioVolumeLabelPadding);
	    const auto stickerHeightSlider = container->add(
		    object_ptr<Ui::MediaSlider>(
			    container,
			    st::settingsAudioVolumeSlider),
		    st::settingsAudioVolumeSliderPadding);
	    const auto updateStickerHeightLabel = [=](int value) {
		    const auto pixels = QString::number(value);
		    stickerHeightLabel->setText(ktr("rtg_settings_sticker_height", { "pixels", pixels }));
	    };
	    const auto updateStickerHeight = [=](int value) {
		    updateStickerHeightLabel(value);
		    ::RabbitSettings::JsonSettings::Set("sticker_height", value);
		    ::RabbitSettings::JsonSettings::Write();
	    };
	    stickerHeightSlider->resize(st::settingsAudioVolumeSlider.seekSize);
	    stickerHeightSlider->setPseudoDiscrete(
		    193,
		    [](int val) { return val + 64; },
		    ::RabbitSettings::JsonSettings::GetInt("sticker_height"),
		    updateStickerHeight);
	    updateStickerHeightLabel(::RabbitSettings::JsonSettings::GetInt("sticker_height"));

		AddButtonWithIcon(
			container,
			rktr("rtg_settings_show_seconds"),
			st::settingsButton,
			IconDescriptor{ &st::menuIconReschedule }
		)->toggleOn(
			rpl::single(::RabbitSettings::JsonSettings::GetBool("show_seconds"))
		)->toggledValue(
		) | rpl::filter([](bool enabled) {
			return (enabled != ::RabbitSettings::JsonSettings::GetBool("show_seconds"));
		}) | rpl::start_with_next([](bool enabled) {
			::RabbitSettings::JsonSettings::Set("show_seconds", enabled);
			::RabbitSettings::JsonSettings::Write();
		}, container->lifetime());

		AddButtonWithIcon(
			container,
			rktr("rtg_settings_show_sender_avatar"),
			st::settingsButton,
			IconDescriptor{ &st::menuIconDockBounce }
		)->toggleOn(
			rpl::single(::RabbitSettings::JsonSettings::GetBool("show_sender_userpic"))
		)->toggledValue(
		) | rpl::filter([](bool enabled) {
			return (enabled != ::RabbitSettings::JsonSettings::GetBool("show_sender_userpic"));
		}) | rpl::start_with_next([](bool enabled) {
			::RabbitSettings::JsonSettings::Set("show_sender_userpic", enabled);
			::RabbitSettings::JsonSettings::Write();
		}, container->lifetime());
		
		SettingsMenuJsonSwitch(rtg_settings_comma_after_mention, comma_after_mention);
    }

    void RabbitChats::SetupRabbitChats(not_null<Ui::VerticalLayout *> container, not_null<Window::SessionController *> controller) {
		Ui::AddSkip(container);
    	SetupChats(container);
    }

    void RabbitChats::setupContent(not_null<Window::SessionController *> controller) {
        const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);

        SetupRabbitChats(content, controller);

        Ui::ResizeFitChild(this, content);
    }
} // namespace Settings
