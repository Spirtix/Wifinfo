#pragma once
#include <string>
#include <Windows.h>

struct WifiAccessPoint {
	std::string ssid;
	int signalQuality;
	unsigned long channel;
	unsigned long frequency;
	std::string band;
};