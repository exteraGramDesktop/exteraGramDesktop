/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitGramDesktop/rabbitGramDesktop/blob/dev/LEGAL
*/
#include <ui/boxes/single_choice_box.h>

#include "rabbit/rabbit_settings.h"
#include "rabbit/rabbit_lang.h"
#include "rabbit/rabbit_settings_menu.h"
#include "rabbit/openai/rabbit_settings_openai.cpp"
#include "lang_auto.h"
#include "mainwindow.h"
#include "settings/settings_common.h"
#include "ui/wrap/vertical_layout.h"
#include "ui/wrap/slide_wrap.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/widgets/checkbox.h"
#include "boxes/connection_box.h"
#include "platform/platform_specific.h"
#include "window/window_session_controller.h"
#include "lang/lang_instance.h"
#include "core/application.h"
#include "storage/localstorage.h"
#include "data/data_session.h"
#include "main/main_session.h"
#include "styles/style_settings.h"
#include "styles/style_menu_icons.h"
#include "apiwrap.h"
#include "api/api_blocked_peers.h"
#include "ui/widgets/continuous_sliders.h"

namespace Settings {

    rpl::producer<QString> RabbitGPT::title() {
        return rktr("rtg_settings_chatgpt");
    }

    RabbitGPT::RabbitGPT(
            QWidget *parent,
            not_null<Window::SessionController *> controller)
            : Section(parent) {
        setupContent(controller);
    }

    void Rabbit::SetupGeneral(not_null<Ui::VerticalLayout *> container)
    {
	    AddSubsectionTitle(container, rktr("rtg_settings_chatgpt"));

        if (RabbitSettings::JsonSettings::GetString("openai_key") == "") {
            void Input::setupContent() {
                const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
                auto currentStepData = stepData();
                const auto currentStepDataPassword = base::take(currentStepData.password);
                const auto currentStepProcessRecover = base::take(
                    currentStepData.processRecover);
                setStepData(currentStepData);

                const auto currentState = cloudPassword().stateCurrent();
                const auto hasPassword = !currentStepProcessRecover.setNewPassword
                    && (currentState ? currentState->hasPassword : false);
                const auto isCheck = currentStepData.currentPassword.isEmpty()
                    && hasPassword
                    && !currentStepProcessRecover.setNewPassword;

                if (currentStepProcessRecover.setNewPassword) {
                    _removesFromStack = std::vector<Type>{
                        CloudPasswordEmailConfirmId()
                    };
                }

                const auto icon = CreateInteractiveLottieIcon(
                    content,
                    {
                        .name = u"cloud_password/password_input"_q,
                        .sizeOverride = {
                            st::settingsCloudPasswordIconSize,
                            st::settingsCloudPasswordIconSize
                        },
                    },
                    st::settingLocalPasscodeIconPadding);

                SetupHeader(
                    content,
                    QString(),
                    rpl::never<>(),
                    isCheck
                        ? tr::lng_settings_cloud_password_check_subtitle()
                        : hasPassword
                        ? tr::lng_settings_cloud_password_manage_password_change()
                        : tr::lng_settings_cloud_password_password_subtitle(),
                    tr::lng_cloud_password_about());

                AddSkip(content, st::settingLocalPasscodeDescriptionBottomSkip);

                const auto newInput = AddPasswordField(
                    content,
                    isCheck
                        ? tr::lng_cloud_password_enter_old()
                        : tr::lng_cloud_password_enter_new(),
                        currentStepDataPassword);
                const auto reenterInput = isCheck
                    ? (Ui::PasswordInput*)(nullptr)
                    : AddPasswordField(
                        content,
                        tr::lng_cloud_password_confirm_new(),
                        currentStepDataPassword).get();
                const auto error = AddError(content, newInput);
                if (reenterInput) {
                    QObject::connect(reenterInput, &Ui::MaskedInputField::changed, [=] {
                        error->hide();
                    });
                }

                if (isCheck) {
                    AddSkipInsteadOfField(content);

                    const auto hint = currentState ? currentState->hint : QString();
                    const auto hintInfo = Ui::CreateChild<Ui::FlatLabel>(
                        error->parentWidget(),
                        tr::lng_signin_hint(tr::now, lt_password_hint, hint),
                        st::defaultFlatLabel);
                    hintInfo->setVisible(!hint.isEmpty());
                    error->geometryValue(
                    ) | rpl::start_with_next([=](const QRect &r) {
                        hintInfo->setGeometry(r);
                    }, hintInfo->lifetime());
                    error->shownValue(
                    ) | rpl::start_with_next([=](bool shown) {
                        if (shown) {
                            hintInfo->hide();
                        } else {
                            hintInfo->setVisible(!hint.isEmpty());
                        }
                    }, hintInfo->lifetime());

                    auto recoverCallback = [=] {
                        if (_requestLifetime) {
                            return;
                        }
                        const auto state = cloudPassword().stateCurrent();
                        if (!state) {
                            return;
                        }
                        if (state->hasRecovery) {
                            _requestLifetime = cloudPassword().requestPasswordRecovery(
                            ) | rpl::start_with_next_error([=](const QString &pattern) {
                                _requestLifetime.destroy();

                                auto data = stepData();
                                data.processRecover = currentStepProcessRecover;
                                data.processRecover.emailPattern = pattern;
                                setStepData(std::move(data));
                                showOther(CloudPasswordEmailConfirmId());
                            }, [=](const QString &type) {
                                _requestLifetime.destroy();

                                error->show();
                                if (MTP::IsFloodError(type)) {
                                    error->setText(tr::lng_flood_error(tr::now));
                                } else {
                                    error->setText(Lang::Hard::ServerError());
                                }
                            });
                        } else {
                            const auto callback = [=](Fn<void()> close) {
                                if (_requestLifetime) {
                                    return;
                                }
                                close();
                                _requestLifetime = cloudPassword().resetPassword(
                                ) | rpl::start_with_error_done([=](const QString &type) {
                                    _requestLifetime.destroy();
                                }, [=] {
                                    _requestLifetime.destroy();
                                });
                            };
                            controller()->show(Ui::MakeConfirmBox({
                                .text = tr::lng_cloud_password_reset_no_email(),
                                .confirmed = callback,
                                .confirmText = tr::lng_cloud_password_reset_ok(),
                                .cancelText = tr::lng_cancel(),
                                .confirmStyle = &st::attentionBoxButton,
                            }));
                        }
                    };

                    const auto recover = AddLinkButton(content, newInput);
                    const auto resetInfo = Ui::CreateChild<Ui::FlatLabel>(
                        content,
                        QString(),
                        st::boxDividerLabel);
                    recover->geometryValue(
                    ) | rpl::start_with_next([=](const QRect &r) {
                        resetInfo->moveToLeft(r.x(), r.y() + st::passcodeTextLine);
                    }, resetInfo->lifetime());

                    setupRecoverButton(
                        content,
                        recover,
                        resetInfo,
                        std::move(recoverCallback));
                } else if (currentStepProcessRecover.setNewPassword && reenterInput) {
                    const auto skip = AddLinkButton(content, reenterInput);
                    skip->setText(tr::lng_settings_auto_night_disable(tr::now));
                    skip->setClickedCallback([=] {
                        if (_requestLifetime) {
                            return;
                        }
                        _requestLifetime = cloudPassword().recoverPassword(
                            currentStepProcessRecover.checkedCode,
                            QString(),
                            QString()
                        ) | rpl::start_with_error_done([=](const QString &type) {
                            _requestLifetime.destroy();

                            error->show();
                            if (MTP::IsFloodError(type)) {
                                error->setText(tr::lng_flood_error(tr::now));
                            } else {
                                error->setText(Lang::Hard::ServerError());
                            }
                        }, [=] {
                            _requestLifetime.destroy();

                            controller()->show(
                                Ui::MakeInformBox(tr::lng_cloud_password_removed()));
                            setStepData(StepData());
                            showBack();
                        });
                    });
                    AddSkip(content);
                }

                if (!newInput->text().isEmpty()) {
                    icon.icon->jumpTo(icon.icon->framesCount() / 2, icon.update);
                }

                const auto checkPassword = [=](const QString &pass) {
                    if (_requestLifetime) {
                        return;
                    }
                    _requestLifetime = cloudPassword().check(
                        pass
                    ) | rpl::start_with_error_done([=](const QString &type) {
                        _requestLifetime.destroy();

                        newInput->setFocus();
                        newInput->showError();
                        newInput->selectAll();
                        error->show();
                        if (MTP::IsFloodError(type)) {
                            error->setText(tr::lng_flood_error(tr::now));
                        } else if (type == u"PASSWORD_HASH_INVALID"_q
                            || type == u"SRP_PASSWORD_CHANGED"_q) {
                            error->setText(tr::lng_cloud_password_wrong(tr::now));
                        } else {
                            error->setText(Lang::Hard::ServerError());
                        }
                    }, [=] {
                        _requestLifetime.destroy();

                        if (const auto state = cloudPassword().stateCurrent()) {
                            if (state->pendingResetDate > 0) {
                                auto lifetime = rpl::lifetime();
                                lifetime = cloudPassword().cancelResetPassword(
                                ) | rpl::start_with_next([] {});
                            }
                        }

                        auto data = stepData();
                        data.currentPassword = pass;
                        setStepData(std::move(data));
                        showOther(CloudPasswordManageId());
                    });
                };

                const auto button = AddDoneButton(
                    content,
                    isCheck ? tr::lng_passcode_check_button() : tr::lng_continue());
                button->setClickedCallback([=] {
                    const auto newText = newInput->text();
                    const auto reenterText = isCheck ? QString() : reenterInput->text();
                    if (newText.isEmpty()) {
                        newInput->setFocus();
                        newInput->showError();
                    } else if (reenterInput && reenterText.isEmpty()) {
                        reenterInput->setFocus();
                        reenterInput->showError();
                    } else if (reenterInput && (newText != reenterText)) {
                        reenterInput->setFocus();
                        reenterInput->showError();
                        reenterInput->selectAll();
                        error->show();
                        error->setText(tr::lng_cloud_password_differ(tr::now));
                    } else if (isCheck) {
                        checkPassword(newText);
                    } else {
                        auto data = stepData();
                        data.processRecover = currentStepProcessRecover;
                        data.password = newText;
                        setStepData(std::move(data));
                        showOther(CloudPasswordHintId());
                    }
                });

                base::qt_signal_producer(
                    newInput.get(),
                    &QLineEdit::textChanged // Covers Undo.
                ) | rpl::map([=] {
                    return newInput->text().isEmpty();
                }) | rpl::distinct_until_changed(
                ) | rpl::start_with_next([=](bool empty) {
                    const auto from = icon.icon->frameIndex();
                    const auto to = empty ? 0 : (icon.icon->framesCount() / 2 - 1);
                    icon.icon->animate(icon.update, from, to);
                }, content->lifetime());

                const auto submit = [=] {
                    if (!reenterInput || reenterInput->hasFocus()) {
                        button->clicked({}, Qt::LeftButton);
                    } else {
                        reenterInput->setFocus();
                    }
                };
                QObject::connect(newInput, &Ui::MaskedInputField::submitted, submit);
                if (reenterInput) {
                    using namespace Ui;
                    QObject::connect(reenterInput, &MaskedInputField::submitted, submit);
                }

                setFocusCallback([=] {
                    if (isCheck || newInput->text().isEmpty()) {
                        newInput->setFocus();
                    } else if (reenterInput->text().isEmpty()) {
                        reenterInput->setFocus();
                    } else {
                        newInput->setFocus();
                    }
                });

                Ui::ResizeFitChild(this, content);
            }
        }
    }

}