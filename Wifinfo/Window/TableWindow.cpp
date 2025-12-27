#include <imgui.h>

#include "TableWindow.h"

void TableWindow::Update() {
	ImGui::Begin("Table");
    if (ImGui::BeginTable("WiFi Table", 4,
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_Sortable |
        ImGuiTableFlags_ScrollY)) {

        ImGui::TableSetupColumn("SSID", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Channel", ImGuiTableColumnFlags_WidthFixed, 60.0f);
        ImGui::TableSetupColumn("Signal Quality", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Band", ImGuiTableColumnFlags_WidthFixed, 40.0f);
        ImGui::TableHeadersRow();

        ImGui::EndTable();
    }
    ImGui::End();
}
