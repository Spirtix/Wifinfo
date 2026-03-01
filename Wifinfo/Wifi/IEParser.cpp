#include "IEParser.h"

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

ULONG IEParser::FrequencyToChannel(ULONG frequency) {
	ULONG freqMHz = frequency / 1000;

	if (freqMHz >= 2412 && freqMHz <= 2484) {
		if (freqMHz == 2484) return 14;
		return (freqMHz - 2407) / 5;
	}

	if (freqMHz >= 5170 && freqMHz <= 5825)
		return (freqMHz - 5000) / 5;

	return 0;
}
