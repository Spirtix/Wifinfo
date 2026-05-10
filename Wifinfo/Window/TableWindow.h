#pragma once
#include "View.h"
#include "../AppContext.h"

class TableWindow : public View {
	public:
		TableWindow(AppContext& appContext) : View(), appContext(appContext) { }
		void Update() override;
	private:
		void Sort(ImGuiTableSortSpecs* sortSpecs);
		AppContext& appContext;
		std::vector<size_t> sortedIndices;
};