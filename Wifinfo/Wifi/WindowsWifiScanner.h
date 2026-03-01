#pragma once
#include <vector>
#include <string>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <wlanapi.h>

#include "WifiAccessPoint.h"


class WindowsWifiScanner {
public:
	WindowsWifiScanner();
	~WindowsWifiScanner();
    std::vector<WifiAccessPoint> ScanNetworks();
private:
    struct ScanContext {
        std::atomic<int> pendingScans = 0 ;
        std::mutex mtx;
        std::condition_variable cv;
    };

    HANDLE clientHandle;
    DWORD negotiatedVersion;
    PWLAN_INTERFACE_INFO_LIST interfaceList = NULL;
    ScanContext ctx;

    static void WlanNotificationCallback(PWLAN_NOTIFICATION_DATA data, PVOID context);
};