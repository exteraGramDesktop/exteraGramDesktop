/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

class PeerData;

namespace Ui {
class VerticalLayout;
} // namespace Ui

namespace Data {
struct Boost;
struct BoostsListSlice;
struct PublicForwardsSlice;
struct RecentPostId;
struct SupergroupStatistics;
} // namespace Data

namespace Info::Statistics {

void AddPublicForwards(
	const Data::PublicForwardsSlice &firstSlice,
	not_null<Ui::VerticalLayout*> container,
	Fn<void(FullMsgId)> showPeerHistory,
	not_null<PeerData*> peer,
	Data::RecentPostId contextId);

void AddMembersList(
	Data::SupergroupStatistics data,
	not_null<Ui::VerticalLayout*> container,
	Fn<void(not_null<PeerData*>)> showPeerInfo,
	not_null<PeerData*> peer,
	rpl::producer<QString> title);

void AddBoostsList(
	const Data::BoostsListSlice &firstSlice,
	not_null<Ui::VerticalLayout*> container,
	Fn<void(const Data::Boost &)> boostClickedCallback,
	not_null<PeerData*> peer,
	rpl::producer<QString> title);

} // namespace Info::Statistics
