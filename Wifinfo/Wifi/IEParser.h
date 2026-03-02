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
    [[nodiscard]] static ChannelInfo ParseChannelInfo(const WLAN_BSS_ENTRY& bss);
    [[nodiscard]] static std::string GetSSIDString(const DOT11_SSID& ssid);
    [[nodiscard]] static ULONG FrequencyToChannel(ULONG frequency) noexcept;
    [[nodiscard]] static std::string PhyTypeName(DOT11_PHY_TYPE pt);
    [[nodiscard]] static std::string SecurityName(DOT11_AUTH_ALGORITHM auth, DOT11_CIPHER_ALGORITHM cipher);
    [[nodiscard]] static std::string MacToString(const DOT11_MAC_ADDRESS& mac);
};
