#pragma once
#include <string>
#include <Windows.h>
#include <wlanapi.h>

struct ChannelInfo {
    int primaryChannel = 0;
    int centerChannel = 0;
    int widthMHz = 20;
};

class IEParser {
public:
    IEParser() = delete;
    static ChannelInfo ParseChannelInfo(const WLAN_BSS_ENTRY& bss);
    static std::string GetSSIDString(const DOT11_SSID& ssid);
    static ULONG FrequencyToChannel(ULONG frequency);
};
