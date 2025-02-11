/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

class DocumentData;
class PhotoData;
class Image;

namespace HistoryView {
class Element;
} // namespace HistoryView

namespace Data {
class Media;
} // namespace Data

namespace Media::Streaming {
struct ExpandDecision;
} // namespace Media::Streaming

namespace HistoryView {

class Media;

void PaintInterpolatedIcon(
	QPainter &p,
	const style::icon &a,
	const style::icon &b,
	float64 b_ratio,
	QRect rect);

[[nodiscard]] std::unique_ptr<Media> CreateAttach(
	not_null<Element*> parent,
	DocumentData *document,
	PhotoData *photo);
[[nodiscard]] std::unique_ptr<Media> CreateAttach(
	not_null<Element*> parent,
	DocumentData *document,
	PhotoData *photo,
	const std::vector<std::unique_ptr<Data::Media>> &collage,
	const QString &webpageUrl);
[[nodiscard]] int UnitedLineHeight();

[[nodiscard]] inline QSize NonEmptySize(QSize size) {
	return QSize(std::max(size.width(), 1), std::max(size.height(), 1));
}

[[nodiscard]] inline QSize DownscaledSize(QSize size, QSize box) {
	return NonEmptySize(
		((size.width() > box.width() || size.height() > box.height())
			? size.scaled(box, Qt::KeepAspectRatio)
			: size));
}

[[nodiscard]] QImage PrepareWithBlurredBackground(
	QSize outer,
	::Media::Streaming::ExpandDecision resize,
	Image *large,
	Image *blurred);
[[nodiscard]] QImage PrepareWithBlurredBackground(
	QSize outer,
	::Media::Streaming::ExpandDecision resize,
	QImage large,
	QImage blurred);

[[nodiscard]] QSize CountDesiredMediaSize(QSize original);
[[nodiscard]] QSize CountMediaSize(QSize desired, int newWidth);
[[nodiscard]] QSize CountPhotoMediaSize(
	QSize desired,
	int newWidth,
	int maxWidth);

} // namespace HistoryView
