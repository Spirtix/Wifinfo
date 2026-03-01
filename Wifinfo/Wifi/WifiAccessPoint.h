#pragma once
#include <string>

struct WifiAccessPoint {
	std::string ssid;
	int signalQuality;
	long signalStrength;
	int primaryChannel;
	int centerChannel;
	unsigned long frequency;
	int widthMHz;
	std::string band;
};