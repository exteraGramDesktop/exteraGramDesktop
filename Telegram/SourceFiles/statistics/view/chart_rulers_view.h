/*
This file is part of rabbitGram Desktop,
the unofficial app based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/rabbitgramdesktop/rabbitgramdesktop/blob/dev/LEGAL
*/
#pragma once

#include "statistics/chart_rulers_data.h"

namespace Data {
struct StatisticalChart;
} // namespace Data

namespace Statistic {

enum class ChartViewType;
struct Limits;
class LinesFilterController;

struct ChartRulersView final {
public:
	ChartRulersView();

	void setChartData(
		const Data::StatisticalChart &chartData,
		ChartViewType type,
		std::shared_ptr<LinesFilterController> linesFilter);

	void paintRulers(QPainter &p, const QRect &r);

	void paintCaptionsToRulers(QPainter &p, const QRect &r);

	void computeRelative(int newMaxHeight, int newMinHeight);
	void setAlpha(float64 value);
	void add(Limits newHeight, bool animated);

private:
	bool _isDouble = false;
	QPen _leftPen;
	QPen _rightPen;
	int _leftLineId = 0;
	int _rightLineId = 0;
	const style::icon *_currencyIcon = nullptr;

	Fn<QString(float64)> _leftCustomCaption = nullptr;
	Fn<QString(float64)> _rightCustomCaption = nullptr;

	std::vector<ChartRulersData> _rulers;

	std::shared_ptr<LinesFilterController> _linesFilter;

	float64 _scaledLineRatio = 0.;
	bool _isLeftLineScaled = false;

};

} // namespace Statistic
