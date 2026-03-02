#include <imgui.h>

#include "TableWindow.h"
#include "../Application.h"

static ImVec4 ColorBySignal(long rssi) {
    if (rssi >= -50) return { 0.00f, 0.90f, 0.20f, 1.f };
    if (rssi >= -60) return { 0.40f, 0.90f, 0.00f, 1.f };
    if (rssi >= -70) return { 1.00f, 0.80f, 0.00f, 1.f };
    if (rssi >= -80) return { 1.00f, 0.50f, 0.00f, 1.f };
    return { 0.90f, 0.10f, 0.10f, 1.f };
}

void TableWindow::Update() {
	ImGui::Begin("Table");
    if (ImGui::BeginTable("WiFi Table", 5,
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_Sortable |
        ImGuiTableFlags_ScrollY,
        ImVec2(-1.f, ImGui::GetContentRegionAvail().y - 4.f))) {

        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("SSID", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("BSSID", ImGuiTableColumnFlags_WidthFixed, 148.f);
        ImGui::TableSetupColumn("Channel", ImGuiTableColumnFlags_WidthFixed, 70.f);
        ImGui::TableSetupColumn("Signal", ImGuiTableColumnFlags_WidthFixed, 150.f);
        ImGui::TableSetupColumn("PHY", ImGuiTableColumnFlags_WidthFixed, 72.f);
        ImGui::TableHeadersRow();

        for (const auto& ap : appContext.wifiAPs) {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", ap.ssid.c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::TextUnformatted(ap.bssid.c_str());

            ImGui::TableSetColumnIndex(2);
            if (ap.primaryChannel == ap.centerChannel)
                ImGui::Text("%d", ap.primaryChannel);
            else
                ImGui::Text("%d(%d)", ap.primaryChannel, ap.centerChannel);

            ImGui::TableSetColumnIndex(3);
            ImGui::PushStyleColor(ImGuiCol_Text, ColorBySignal(ap.signalStrength));
            ImGui::Text("%ld dBm", ap.signalStrength);
            ImGui::PopStyleColor();
            ImGui::SameLine();
            char ov[16]; snprintf(ov, sizeof(ov), "%d%%", ap.signalQuality);
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ColorBySignal(ap.signalStrength));
            ImGui::ProgressBar(ap.signalQuality / 100.f, ImVec2(-1.f, 0.f), ov);
            ImGui::PopStyleColor();

            ImGui::TableSetColumnIndex(4);
            ImGui::TextUnformatted(ap.phyType.c_str());
        }

        ImGui::EndTable();
    }
    ImGui::End();
}
