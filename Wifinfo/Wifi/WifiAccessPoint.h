#pragma once
#include <string>

struct WifiAccessPoint {
	std::string ssid;
	std::string bssid;
	int signalQuality;
	long signalStrength;
	int primaryChannel;
	int centerChannel;
	unsigned long frequency;
	int widthMHz;
	std::string band;
	std::string phyType;
	std::string security;
};