#pragma once
#include <string>
#include <Windows.h>

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