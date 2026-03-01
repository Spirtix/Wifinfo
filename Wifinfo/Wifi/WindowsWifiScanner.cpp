#include <stdexcept>

#include "WindowsWifiScanner.h"
#include "IEParser.h"

WindowsWifiScanner::WindowsWifiScanner() {
	DWORD result = WlanOpenHandle(2, NULL, &negotiatedVersion, &clientHandle);

	if (result != ERROR_SUCCESS)
		throw std::runtime_error("Failed to open WLAN handle.");

	result = WlanEnumInterfaces(clientHandle, NULL, &interfaceList);

	if (result != ERROR_SUCCESS)
		throw std::runtime_error("Failed to enumerate interfaces. Error code: " + std::to_string(result));
}

std::vector<WifiAccessPoint> WindowsWifiScanner::ScanNetworks() {
	std::vector<WifiAccessPoint> accessPoints;

	ctx.pendingScans = interfaceList->dwNumberOfItems;

	DWORD result = WlanRegisterNotification(clientHandle, WLAN_NOTIFICATION_SOURCE_ACM,
		TRUE, WlanNotificationCallback, &ctx, NULL, NULL);
	if (result != ERROR_SUCCESS)
		throw std::runtime_error("Failed to register WLAN notification.");

	for (DWORD i = 0; i < interfaceList->dwNumberOfItems; ++i)
		if (WlanScan(clientHandle, &interfaceList->InterfaceInfo[i].InterfaceGuid, NULL, NULL, NULL) != ERROR_SUCCESS)
			--ctx.pendingScans;

	{
		std::unique_lock<std::mutex> lock(ctx.mtx);
		ctx.cv.wait_for(lock, std::chrono::seconds(5), [this] { return ctx.pendingScans == 0; });
	}

	WlanRegisterNotification(clientHandle, WLAN_NOTIFICATION_SOURCE_NONE, TRUE,NULL, NULL, NULL, NULL);

	for (DWORD i = 0; i < interfaceList->dwNumberOfItems; ++i) {
		WlanPointer<WLAN_BSS_LIST> bssList;

		DWORD result = WlanGetNetworkBssList(clientHandle, &interfaceList->InterfaceInfo[i].InterfaceGuid,
			NULL, dot11_BSS_type_any, FALSE, NULL, &bssList);
		if (result != ERROR_SUCCESS)
			continue;

		for (DWORD j = 0; j < bssList->dwNumberOfItems; ++j) {
			WLAN_BSS_ENTRY& bss = bssList->wlanBssEntries[j];

			WifiAccessPoint ap;
			auto channelInfo = IEParser::ParseChannelInfo(bss);
			ap.primaryChannel = channelInfo.primaryChannel;
			ap.centerChannel = channelInfo.centerChannel;
			ap.widthMHz = channelInfo.widthMHz;
			ap.frequency = bss.ulChCenterFrequency;
			ap.ssid = IEParser::GetSSIDString(bss.dot11Ssid);
			if (ap.ssid.empty())
				ap.ssid = "<Hidden SSID>";
			ap.signalQuality = (int)bss.uLinkQuality;
			ap.signalStrength = bss.lRssi;
			ap.band = (ap.frequency > 5000000) ? "5GHz" : "2.4GHz";
			ap.phyType = IEParser::PhyTypeName(bss.dot11BssPhyType);

			accessPoints.push_back(ap);
		}
	}
	return accessPoints;
}

void WindowsWifiScanner::WlanNotificationCallback(PWLAN_NOTIFICATION_DATA data, PVOID context) {
	if (!context || data->NotificationSource != WLAN_NOTIFICATION_SOURCE_ACM) return;
	ScanContext* ctx = reinterpret_cast<ScanContext*>(context);

	if (data->NotificationCode == wlan_notification_acm_scan_complete ||
		data->NotificationCode == wlan_notification_acm_scan_fail) {
		--ctx->pendingScans;
		ctx->cv.notify_all();
	}
}
