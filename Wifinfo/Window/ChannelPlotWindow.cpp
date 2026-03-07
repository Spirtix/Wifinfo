#include <imgui.h>
#include <implot.h>
#include <implot_internal.h>

#include "ChannelPlotWindow.h"
#include "../Application.h"

void ChannelPlotWindow::Update() {
	if (ImGui::Begin("Channel Plot")) {
		if (ImPlot::BeginPlot("Channel Plot", ImVec2(-1, -1))) {

			ImPlot::SetupAxis(ImAxis_X1, "Channel");
			ImPlot::SetupAxis(ImAxis_Y1, "Signal (dBm)");

			ImPlot::SetupAxisLimits(ImAxis_X1, -2, 18, ImPlotCond_Once);
			ImPlot::SetupAxisLimits(ImAxis_Y1, -100, -20, ImPlotCond_Once);
			ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, -100, -20);
			ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, -2, 170);
			ImPlot::SetupAxisZoomConstraints(ImAxis_X1, 0, 15);

			for (const auto& ap : appContext.wifiAPs) {
				const double halfWidth = ap.widthMHz / 10.0; // width / 5 MHz / 2.0
				const double minChannel = ap.centerChannel - halfWidth;
				const double maxChannel = ap.centerChannel + halfWidth;

				for (int i = 0; i < numPoints; i++) {
					double x = minChannel + (maxChannel - minChannel) * i / (numPoints - 1);
					xData[i] = x;

					double t = (x - minChannel) / (maxChannel - minChannel);
					double sineValue = sin(t * 3.14159);

					yData[i] = -100.0 + (ap.signalStrength + 100.0) * sineValue;
				}

				ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.5f);
				ImPlot::PlotShaded(ap.ssid.c_str(), xData.data(), yData.data(), numPoints, -100.0);
				ImPlot::PopStyleVar();

				ImPlot::PushStyleColor(ImPlotCol_Line, ImPlot::GetLastItemColor());
				ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 2);
				ImPlot::PlotLine(ap.ssid.c_str(), xData.data(), yData.data(), numPoints);
				ImPlot::PopStyleColor();

				if (ImPlot::BeginItem(ap.ssid.c_str())) {
					ImPlot::PlotText(ap.ssid.c_str(), ap.centerChannel, ap.signalStrength, ImVec2(0, -10));
					ImPlot::EndItem();
				}
			}
			ImPlot::EndPlot();
		}
	}
	ImGui::End();
}