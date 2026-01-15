#pragma once
#include <string>
#include <Windows.h>

struct WifiAccessPoint {
	std::string ssid;
	int signalQuality;
	long signalStrength;
	unsigned long channel;
	unsigned long frequency;
	std::string band;
};