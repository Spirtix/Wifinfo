#pragma once
#include <memory>
#include <vector>

#include "WifiScanner.h"
#include "WifiAccessPoint.h"
#include "WifiAccessPoint.h"

class MacWifiScanner : public WifiScanner {
public:
    MacWifiScanner();
    ~MacWifiScanner() override;
    [[nodiscard]] std::vector<WifiAccessPoint> ScanNetworks() override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
