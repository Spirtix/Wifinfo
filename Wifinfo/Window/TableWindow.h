#pragma once
#include "ImGuiWindow.h"
#include "../AppContext.h"

class TableWindow : public ImGuiWindow {
	public:
		TableWindow(AppContext& appContext) : ImGuiWindow(), appContext(appContext) { }
		void Update() override;
	private:
		AppContext& appContext;
};