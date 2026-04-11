#pragma once
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <wlanapi.h>
#include <iostream>

#include "WifiScanner.h"
#include "WifiAccessPoint.h"


class WindowsWifiScanner : public WifiScanner {
public:
	WindowsWifiScanner();
    [[nodiscard]] std::vector<WifiAccessPoint> ScanNetworks() override;
private:
    struct ScanContext {
        std::atomic<int> pendingScans = 0;
        std::mutex mtx;
        std::condition_variable cv;
    };
    struct WlanHandle {
        WlanHandle() = default;
        ~WlanHandle() { if (h) WlanCloseHandle(h, NULL); }
        WlanHandle(const WlanHandle&) = delete;
        WlanHandle& operator=(const WlanHandle&) = delete;
        operator HANDLE() const { return h; }
        HANDLE* operator&() { return &h; }
    private:
        HANDLE h = NULL;
    };
    template<typename T>
    struct WlanPointer {
        WlanPointer() = default;
        ~WlanPointer() { if (p) WlanFreeMemory(p); }
        WlanPointer(const WlanPointer&) = delete;
        WlanPointer& operator=(const WlanPointer&) = delete;
        operator T* () const { return p; }
        T** operator&() { return &p; }
        T* operator->() const { return p; }
    private:
        T* p = NULL;
    };

    WlanHandle clientHandle;
    DWORD negotiatedVersion = 0;
    WlanPointer<WLAN_INTERFACE_INFO_LIST> interfaceList;
    ScanContext ctx;

    static void WlanNotificationCallback(PWLAN_NOTIFICATION_DATA data, PVOID context);
};