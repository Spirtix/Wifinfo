#pragma once
#include <vector>

#include "ImGuiWindow.h"
#include "../AppContext.h"

class ChannelPlotWindow : public ImGuiWindow {
	public:
		ChannelPlotWindow(AppContext& appContext) : ImGuiWindow(), appContext(appContext), xData(numPoints), yData(numPoints) {}
		void Update() override;
	private:
		std::vector<double> xData, yData;
		AppContext& appContext;
		static constexpr int numPoints = 50;
};