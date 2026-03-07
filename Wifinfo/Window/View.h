#pragma once

class View {
	public:
		virtual ~View() = default;
		virtual void Update() = 0;
};