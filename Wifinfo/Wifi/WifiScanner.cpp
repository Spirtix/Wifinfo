#include <stdexcept>

#include "WifiScanner.h"

WifiScanner::WifiScanner() : clientHandle(NULL), negotiatedVersion(0) {
	DWORD result = WlanOpenHandle(2, NULL, &negotiatedVersion, &clientHandle);

	if (result != ERROR_SUCCESS)
		throw std::runtime_error("Failed to open WLAN handle.");
}

WifiScanner::~WifiScanner() {
	if (clientHandle != NULL)
		WlanCloseHandle(clientHandle, NULL);
}

std::vector<WifiAccessPoint> WifiScanner::ScanNetworks() {
	std::vector<WifiAccessPoint> accessPoints;

	PWLAN_INTERFACE_INFO_LIST interfaceList = NULL;
	DWORD result = WlanEnumInterfaces(clientHandle, NULL, &interfaceList);

	if (result != ERROR_SUCCESS)
		throw std::runtime_error("Failed to enumerate interfaces. Error code: " + std::to_string(result));

	try {
		for (DWORD i = 0; i < interfaceList->dwNumberOfItems; ++i) {
			WLAN_INTERFACE_INFO& interfaceInfo = interfaceList->InterfaceInfo[i];

			result = WlanScan(clientHandle, &interfaceInfo.InterfaceGuid, NULL, NULL, NULL);

			Sleep(1000); //FIXME: wlanscan requests a scan and doesn't block, implement async scanning instead of busy waiting

			PWLAN_BSS_LIST bssList = NULL;
			result = WlanGetNetworkBssList(clientHandle, &interfaceInfo.InterfaceGuid, NULL, dot11_BSS_type_any, FALSE, NULL, &bssList);

			if (result != ERROR_SUCCESS)
				continue;

			try {
				for (DWORD j = 0; j < bssList->dwNumberOfItems; ++j) {
					WLAN_BSS_ENTRY& bss = bssList->wlanBssEntries[j];

					WifiAccessPoint ap;
					ap.ssid = GetSSIDString(bss.dot11Ssid);
					ap.signalQuality = (int)bss.uLinkQuality;
					ap.channel = FrequencyToChannel(bss.ulChCenterFrequency);
					ap.frequency = bss.ulChCenterFrequency;
					ap.band = (bss.ulChCenterFrequency > 5000000) ? "5GHz" : "2.4GHz";

					accessPoints.push_back(ap);
				}
			} catch (...) {
				if (bssList != NULL)
					WlanFreeMemory(bssList);
				throw;
			}

			if (bssList != NULL)
				WlanFreeMemory(bssList);
		}
	} catch (...) {
		if (interfaceList != NULL)
			WlanFreeMemory(interfaceList);
		throw;
	}

	if (interfaceList != NULL)
		WlanFreeMemory(interfaceList);

	return accessPoints;
}

std::string WifiScanner::GetSSIDString(const DOT11_SSID& ssid) {
	if (ssid.uSSIDLength == 0)
		return "";

	return std::string(reinterpret_cast<const char*>(ssid.ucSSID), ssid.uSSIDLength);
}

ULONG WifiScanner::FrequencyToChannel(ULONG frequency) {
	ULONG freqMHz = frequency / 1000;

	if (freqMHz >= 2412 && freqMHz <= 2484) {
		if (freqMHz == 2484) return 14;
		return (freqMHz - 2407) / 5;
	}

	if (freqMHz >= 5170 && freqMHz <= 5825) {
		return (freqMHz - 5000) / 5;
	}

	return 0;
}

