#pragma once
#include <vector>

#include "View.h"
#include "../AppContext.h"

class ChannelPlotWindow : public View {
	public:
		ChannelPlotWindow(AppContext& appContext) : View(), appContext(appContext), xData(numPoints), yData(numPoints) {}
		void Update() override;
	private:
		std::vector<double> xData, yData;
		AppContext& appContext;
		static constexpr int numPoints = 50;
};