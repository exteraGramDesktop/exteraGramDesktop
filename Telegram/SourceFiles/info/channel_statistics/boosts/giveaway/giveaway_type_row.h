/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "ui/empty_userpic.h"
#include "ui/widgets/buttons.h"

namespace Ui {
template <typename Enum>
class RadioenumGroup;
} // namespace Ui

namespace Giveaway {

class GiveawayTypeRow final : public Ui::RippleButton {
public:
	enum class Type {
		Random,
		SpecificUsers,

		AllMembers,
		OnlyNewMembers,

		Prepaid,
	};

	GiveawayTypeRow(
		not_null<Ui::RpWidget*> parent,
		Type type,
		rpl::producer<QString> subtitle,
		bool group);

	GiveawayTypeRow(
		not_null<Ui::RpWidget*> parent,
		Type type,
		int colorIndex,
		rpl::producer<QString> title,
		rpl::producer<QString> subtitle,
		QImage badge);

	void addRadio(std::shared_ptr<Ui::RadioenumGroup<Type>> typeGroup);

protected:
	void paintEvent(QPaintEvent *e) override;

	int resizeGetHeight(int) override;

private:
	const Type _type;
	const style::PeerListItem _st;

	Ui::EmptyUserpic _userpic;
	Ui::Text::String _status;
	Ui::Text::String _name;

	QImage _badge;

};

} // namespace Giveaway
