#import <CoreWLAN/CoreWLAN.h>
#import <Foundation/Foundation.h>

#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <string>

#include "MacWifiScanner.h"

struct MacWifiScanner::Impl {
    CWWiFiClient* client = nil;
};

namespace {

struct IEInfo {
    int centerChannel = 0;
    std::string phyType;
};


IEInfo ParseIEs(NSData* data, int primaryChannel, CWChannelWidth width) {
    IEInfo info;
    info.centerChannel = primaryChannel;
    int phyRank = 0;
    auto setPhy = [&](int rank, const char* name) {
        if (rank > phyRank) { phyRank = rank; info.phyType = name; }
    };

    if (data) {
        const uint8_t* p = static_cast<const uint8_t*>(data.bytes);
        size_t len = data.length, off = 0;

        while (off + 2 <= len) {
            uint8_t tag = p[off], elen = p[off + 1];
            if (off + 2 + elen > len) break;
            const uint8_t* body = p + off + 2;

            switch (tag) {
            case 45:  setPhy(1, "802.11n"); break;                    // HT Capabilities
            case 191: setPhy(2, "802.11ac"); break;                   // VHT Capabilities
            case 255:                                                 // Extension element
                if (elen >= 1 && body[0] == 35)  setPhy(3, "802.11ax"); // HE
                if (elen >= 1 && body[0] == 108) setPhy(4, "802.11be"); // EHT
                break;
            case 61:                                                  // HT Operation
                if (elen >= 2 && width == kCWChannelWidth40MHz) {
                    int secondary = body[1] & 0x03;                   // 1 = above, 3 = below
                    if (secondary == 1)      info.centerChannel = body[0] + 2;
                    else if (secondary == 3) info.centerChannel = body[0] - 2;
                }
                break;
            case 192:                                                 // VHT Operation
                if (elen >= 3 && body[0] >= 1) {
                    int ccfs0 = body[1], ccfs1 = body[2];
                    if (ccfs1 != 0 && std::abs(ccfs1 - ccfs0) == 8)
                        info.centerChannel = ccfs1;                   // 160 MHz
                    else if (ccfs0 != 0)
                        info.centerChannel = ccfs0;                   // 80 MHz
                }
                break;
            }
            off += 2 + elen;
        }
    }

    if (info.phyType.empty())
        info.phyType = (primaryChannel > 14) ? "802.11a" : "802.11g";
    return info;
}

int WidthToMHz(CWChannelWidth w) {
    switch (w) {
    case kCWChannelWidth20MHz:  return 20;
    case kCWChannelWidth40MHz:  return 40;
    case kCWChannelWidth80MHz:  return 80;
    case kCWChannelWidth160MHz: return 160;
    default:                    return 20;
    }
}

unsigned long ChannelToKHz(int channel, CWChannelBand band) {
    switch (band) {
    case kCWChannelBand5GHz:
        return (5000UL + 5UL * channel) * 1000UL;
    default:
        return (channel == 14 ? 2484000UL : (2407UL + 5UL * channel) * 1000UL);
    }
}

const char* BandName(CWChannelBand band) {
    switch (band) {
    case kCWChannelBand5GHz: return "5GHz";
    default:                 return "2.4GHz";
    }
}

} // namespace

MacWifiScanner::MacWifiScanner() : impl(std::make_unique<Impl>()) {
    @autoreleasepool {
        impl->client = [CWWiFiClient sharedWiFiClient];
        if (!impl->client)
            throw std::runtime_error("Failed to create CWWiFiClient.");

        if (impl->client.interfaces.count == 0)
            throw std::runtime_error("No Wi-Fi interfaces found.");
    }
}

MacWifiScanner::~MacWifiScanner() = default;

std::vector<WifiAccessPoint> MacWifiScanner::ScanNetworks() {
    std::vector<WifiAccessPoint> accessPoints;

    @autoreleasepool {
        for (CWInterface* iface in impl->client.interfaces) {
            NSError* error = nil;
            // Blocking call
            NSSet<CWNetwork*>* networks =
                [iface scanForNetworksWithName:nil includeHidden:YES error:&error];
            if (!networks) continue;

            for (CWNetwork* net in networks) {
                CWChannel* ch = net.wlanChannel;
                if (!ch) continue;

                WifiAccessPoint ap;
                const int primary = (int)ch.channelNumber;
                IEInfo ie = ParseIEs(net.informationElementData, primary, ch.channelWidth);

                ap.bssid = net.bssid ? net.bssid.UTF8String : "";
                ap.ssid = net.ssid ? net.ssid.UTF8String : "";
                if (ap.ssid.empty())
                    ap.ssid = "<Hidden SSID>";

                ap.primaryChannel = primary;
                ap.centerChannel = ie.centerChannel;
                ap.widthMHz = WidthToMHz(ch.channelWidth);
                ap.frequency = ChannelToKHz(primary, ch.channelBand);
                ap.band = BandName(ch.channelBand);
                ap.phyType = ie.phyType;
                ap.signalStrength = (int)net.rssiValue;
                ap.signalQuality = std::clamp(2 * ((int)net.rssiValue + 100), 0, 100);

                accessPoints.push_back(std::move(ap));
            }
        }
    }
    return accessPoints;
}
