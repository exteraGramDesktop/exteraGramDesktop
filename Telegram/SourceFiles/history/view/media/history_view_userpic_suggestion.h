/*
This file is part of exteraGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/exteraGramDesktop/exteraGramDesktop/blob/dev/LEGAL
*/
#pragma once

#include "history/view/media/history_view_media.h"
#include "history/view/media/history_view_media_unwrapped.h"
#include "history/view/media/history_view_photo.h"
#include "history/view/media/history_view_service_box.h"

namespace Data {
class MediaGiftBox;
} // namespace Data

namespace HistoryView {

class UserpicSuggestion final : public ServiceBoxContent {
public:
	UserpicSuggestion(
		not_null<Element*> parent,
		not_null<PeerData*> chat,
		not_null<PhotoData*> photo,
		int width);
	~UserpicSuggestion();

	int top() override;
	QSize size() override;
	QString title() override;
	TextWithEntities subtitle() override;
	QString button() override;
	void draw(
		Painter &p,
		const PaintContext &context,
		const QRect &geometry) override;
	ClickHandlerPtr createViewLink() override;

	bool hideServiceText() override {
		return true;
	}

	void stickerClearLoopPlayed() override;
	std::unique_ptr<StickerPlayer> stickerTakePlayer(
		not_null<DocumentData*> data,
		const Lottie::ColorReplacements *replacements) override;

	bool hasHeavyPart() override;
	void unloadHeavyPart() override;

private:
	Photo _photo;

};

} // namespace HistoryView
