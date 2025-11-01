#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include <wlanapi.h>

#include "WifiAccessPoint.h"


class WifiScanner {
public:
	WifiScanner();
	~WifiScanner();
    std::vector<WifiAccessPoint> ScanNetworks();
private:
    HANDLE clientHandle;
    DWORD negotiatedVersion;

    std::string GetSSIDString(const DOT11_SSID& ssid);
    ULONG FrequencyToChannel(ULONG frequency);
};