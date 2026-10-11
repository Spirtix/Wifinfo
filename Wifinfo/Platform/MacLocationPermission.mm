#import <CoreLocation/CoreLocation.h>
#include "MacLocationPermission.h"
#include <iostream>
static CLLocationManager* g_manager = nil;

void MacLocationPermission::Request() {
    if (g_manager) return;

    g_manager = [[CLLocationManager alloc] init];
    [g_manager requestWhenInUseAuthorization];

    CFAbsoluteTime deadline = CFAbsoluteTimeGetCurrent() + 60.0;
    while (g_manager.authorizationStatus == kCLAuthorizationStatusNotDetermined &&
           CFAbsoluteTimeGetCurrent() < deadline) {
        CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.1, false);
    }
}
