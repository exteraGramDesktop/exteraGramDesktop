/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "base/qt/qt_compare.h"

namespace Data {

struct ReactionId {
	std::variant<QString, DocumentId> data;

	[[nodiscard]] bool empty() const {
		const auto emoji = std::get_if<QString>(&data);
		return emoji && emoji->isEmpty();
	}
	[[nodiscard]] QString emoji() const {
		const auto emoji = std::get_if<QString>(&data);
		return emoji ? *emoji : QString();
	}
	[[nodiscard]] DocumentId custom() const {
		const auto custom = std::get_if<DocumentId>(&data);
		return custom ? *custom : DocumentId();
	}

	explicit operator bool() const {
		return !empty();
	}

	friend inline auto operator<=>(
		const ReactionId &,
		const ReactionId &) = default;
	friend inline bool operator==(
		const ReactionId &a,
		const ReactionId &b) = default;
};

struct MessageReaction {
	ReactionId id;
	int count = 0;
	bool my = false;
};

[[nodiscard]] QString SearchTagToQuery(const ReactionId &tagId);
[[nodiscard]] ReactionId SearchTagFromQuery(const QString &query);
[[nodiscard]] std::vector<ReactionId> SearchTagsFromQuery(
	const QString &query);

[[nodiscard]] QString ReactionEntityData(const ReactionId &id);

[[nodiscard]] ReactionId ReactionFromMTP(const MTPReaction &reaction);
[[nodiscard]] MTPReaction ReactionToMTP(ReactionId id);

} // namespace Data

Q_DECLARE_METATYPE(Data::ReactionId);
