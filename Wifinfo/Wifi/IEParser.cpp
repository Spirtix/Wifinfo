#include "IEParser.h"
#include <format>

ChannelInfo IEParser::ParseChannelInfo(const WLAN_BSS_ENTRY& bss) {
	ChannelInfo channelInfo;
	channelInfo.primaryChannel = FrequencyToChannel(bss.ulChCenterFrequency);
	channelInfo.centerChannel = channelInfo.primaryChannel;
	channelInfo.widthMHz = 20;

	unsigned char* pIeData = (unsigned char*)&bss + bss.ulIeOffset;
	ULONG offset = 0;

	while (offset + 2 < bss.ulIeSize) {
		unsigned char id = pIeData[offset];
		unsigned char length = pIeData[offset + 1];
		unsigned char* data = &pIeData[offset + 2];

		if (offset + 2 + length > bss.ulIeSize) break;

		if (id == 61 && length >= 2) {
			channelInfo.primaryChannel = data[0];
			unsigned char secondaryOffset = data[1] & 0x03;

			if (secondaryOffset == 1) { // SCA: Secondary Channel Above
				channelInfo.widthMHz = 40;
				channelInfo.centerChannel = channelInfo.primaryChannel + 2;
			} else if (secondaryOffset == 3) { // SCB: Secondary Channel Below
				channelInfo.widthMHz = 40;
				channelInfo.centerChannel = channelInfo.primaryChannel - 2;
			}
		} else if (id == 192 && length >= 3) {
			unsigned char widthMode = data[0]; // 0: 20/40, 1: 80, 2: 160
			if (widthMode == 1) {
				channelInfo.widthMHz = 80;
				channelInfo.centerChannel = data[1];
			} else if (widthMode == 2) {
				channelInfo.widthMHz = 160;
				channelInfo.centerChannel = data[1];
			}
		}

		offset += (2 + length);
	}
	return channelInfo;
}

std::string IEParser::GetSSIDString(const DOT11_SSID& ssid) {
	if (ssid.uSSIDLength == 0)
		return std::string();

	return std::string(reinterpret_cast<const char*>(ssid.ucSSID), ssid.uSSIDLength);
}

ULONG IEParser::FrequencyToChannel(ULONG frequency) noexcept {
	ULONG freqMHz = frequency / 1000;

	if (freqMHz >= 2412 && freqMHz <= 2484) {
		if (freqMHz == 2484) return 14;
		return (freqMHz - 2407) / 5;
	}

	if (freqMHz >= 5170 && freqMHz <= 5825)
		return (freqMHz - 5000) / 5;

	return 0;
}

std::string IEParser::PhyTypeName(DOT11_PHY_TYPE pt) {
	switch (pt) {
		case dot11_phy_type_fhss:       return "FHSS";
		case dot11_phy_type_dsss:       return "DSSS";
		case dot11_phy_type_irbaseband: return "IR";
		case dot11_phy_type_ofdm:       return "802.11a";
		case dot11_phy_type_hrdsss:     return "802.11b";
		case dot11_phy_type_erp:        return "802.11g";
		case dot11_phy_type_ht:         return "802.11n";
		case dot11_phy_type_vht:        return "802.11ac";
		case dot11_phy_type_dmg:		return "802.11ad";
		case dot11_phy_type_he:         return "802.11ax";
		case dot11_phy_type_eht:		return "802.11be";
		default:                        return "Unknown";
	}
}

std::string IEParser::SecurityName(DOT11_AUTH_ALGORITHM auth, DOT11_CIPHER_ALGORITHM cipher) {
	std::string s;
	switch (auth) {
		case DOT11_AUTH_ALGO_80211_OPEN:       s = "Open";                break;
		case DOT11_AUTH_ALGO_80211_SHARED_KEY: s = "WEP";                 break;
		case DOT11_AUTH_ALGO_WPA:              s = "WPA-Enterprise";      break;
		case DOT11_AUTH_ALGO_WPA_PSK:          s = "WPA-PSK";             break;
		case DOT11_AUTH_ALGO_WPA_NONE:         s = "WPA-None";            break;
		case DOT11_AUTH_ALGO_RSNA:             s = "WPA2-Enterprise";     break;
		case DOT11_AUTH_ALGO_RSNA_PSK:         s = "WPA2-PSK";            break;
		case DOT11_AUTH_ALGO_WPA3_ENT_192:     s = "WPA3-Enterprise-192"; break;
		case DOT11_AUTH_ALGO_WPA3_SAE:         s = "WPA3-Personal";       break;
		case DOT11_AUTH_ALGO_OWE:              s = "OWE";                 break;
		case DOT11_AUTH_ALGO_WPA3_ENT:         s = "WPA3-Enterprise";     break;
		default:                               s = "Unknown";             break;
	}
	switch (cipher) {
		case DOT11_CIPHER_ALGO_CCMP:     s += "/AES";      break;
		case DOT11_CIPHER_ALGO_CCMP_256: s += "/AES-256";  break;
		case DOT11_CIPHER_ALGO_GCMP:     s += "/GCMP-128"; break;
		case DOT11_CIPHER_ALGO_GCMP_256: s += "/GCMP-256"; break;
		case DOT11_CIPHER_ALGO_TKIP:     s += "/TKIP";     break;
		case DOT11_CIPHER_ALGO_WEP:      s += "/WEP";      break;
		case DOT11_CIPHER_ALGO_WEP40:    s += "/WEP-40";   break;
		case DOT11_CIPHER_ALGO_WEP104:   s += "/WEP-104";  break;
		default:                                           break;
	}
	return s;
}

std::string IEParser::MacToString(const DOT11_MAC_ADDRESS& mac) {
	return std::format("{:02X}:{:02X}:{:02X}:{:02X}:{:02X}:{:02X}",
		mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}
