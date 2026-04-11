#pragma once
#include <vector>
#include "WifiAccessPoint.h"

class WifiScanner {
    public:
        virtual ~WifiScanner() = default;
        [[nodiscard]] virtual std::vector<WifiAccessPoint> ScanNetworks() = 0;
};