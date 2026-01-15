#include <imgui.h>

#include "TableWindow.h"
#include "../Application.h"

void TableWindow::Update() {
	ImGui::Begin("Table");
    if (ImGui::BeginTable("WiFi Table", 4,
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_Sortable |
        ImGuiTableFlags_ScrollY)) {

        ImGui::TableSetupColumn("SSID", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Channel", ImGuiTableColumnFlags_WidthFixed, 20.0f);
        ImGui::TableSetupColumn("Signal Quality", ImGuiTableColumnFlags_WidthFixed, 20.0f);
        ImGui::TableSetupColumn("Band", ImGuiTableColumnFlags_WidthFixed, 20.0f);
        ImGui::TableHeadersRow();

        for (const auto& ap : Application::AppContext.wifiAPs) {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", ap.ssid.c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%lu", ap.primaryChannel);

            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%d", ap.signalQuality);

            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%s", ap.band.c_str());
        }


        ImGui::EndTable();
    }
    ImGui::End();
}
