#pragma once
#include "View.h"
#include "../AppContext.h"

class TableWindow : public View {
	public:
		TableWindow(AppContext& appContext) : View(), appContext(appContext) { }
		void Update() override;
	private:
		AppContext& appContext;
};