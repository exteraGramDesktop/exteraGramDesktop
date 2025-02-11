/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

namespace ChatHelpers {
class Show;
} // namespace ChatHelpers

namespace Api {

void ToggleFavedSticker(
	std::shared_ptr<ChatHelpers::Show> show,
	not_null<DocumentData*> document,
	Data::FileOrigin origin);

void ToggleFavedSticker(
	std::shared_ptr<ChatHelpers::Show> show,
	not_null<DocumentData*> document,
	Data::FileOrigin origin,
	bool faved);

void ToggleRecentSticker(
	not_null<DocumentData*> document,
	Data::FileOrigin origin,
	bool saved);

void ToggleSavedGif(
	std::shared_ptr<ChatHelpers::Show> show,
	not_null<DocumentData*> document,
	Data::FileOrigin origin,
	bool saved);

void ToggleSavedRingtone(
	not_null<DocumentData*> document,
	Data::FileOrigin origin,
	Fn<void()> &&done,
	bool saved);

} // namespace Api
