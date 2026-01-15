#pragma once
#include <vector>

#include "ImGuiWindow.h"

class ChannelPlotWindow : public ImGuiWindow {
	public:
		ChannelPlotWindow() : ImGuiWindow(), xData(numPoints), yData(numPoints) {}
		void Update() override;
	private:
		std::vector<double> xData, yData;
		static constexpr int numPoints = 50;
};