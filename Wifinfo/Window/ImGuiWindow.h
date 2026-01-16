#pragma once

class ImGuiWindow {
	public:
		virtual ~ImGuiWindow() = default;
		virtual void Update() = 0;
};