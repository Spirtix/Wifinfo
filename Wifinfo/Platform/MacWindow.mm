#import <Cocoa/Cocoa.h>
#import <MetalKit/MetalKit.h>

#include "MacWindow.h"

MacWindow::MacWindow() {
    @autoreleasepool {
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        [NSApp finishLaunching];

        NSRect frame = NSMakeRect(0, 0, 1280, 720);
        NSWindow* win = [[NSWindow alloc]
            initWithContentRect:frame
                      styleMask:(NSWindowStyleMaskTitled |
                                 NSWindowStyleMaskClosable |
                                 NSWindowStyleMaskMiniaturizable |
                                 NSWindowStyleMaskResizable)
                        backing:NSBackingStoreBuffered
                          defer:NO];
        [win setTitle:@"Wifinfo"];
        [win setReleasedWhenClosed:NO];

        id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        MTKView* v = [[MTKView alloc] initWithFrame:frame device:device];
        [win setContentView:v];
        [v release];

        [win center];
        [win makeKeyAndOrderFront:nil];
        [NSApp activateIgnoringOtherApps:YES];

        nsWindow = win;
        view = v;
        shouldClose = false;

        CGSize size = v.drawableSize;
        lastWidth = (int)size.width;
        lastHeight = (int)size.height;
    }
}

MacWindow::~MacWindow() {
    @autoreleasepool {
        NSWindow* win = (NSWindow*)nsWindow;
        [win close];
        [win release];
        nsWindow = nullptr;
        view = nullptr;
    }
}

bool MacWindow::ShouldClose() {
    @autoreleasepool {
        NSEvent* event;
        while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                           untilDate:[NSDate distantPast]
                                              inMode:NSDefaultRunLoopMode
                                             dequeue:YES])) {
            [NSApp sendEvent:event];
        }

        if (![(NSWindow*)nsWindow isVisible])
            shouldClose = true;

        MTKView* v = (MTKView*)view;
        CGSize size = v.drawableSize;
        int w = (int)size.width;
        int h = (int)size.height;
        if (w != lastWidth || h != lastHeight) {
            lastWidth = w;
            lastHeight = h;
            if (resizeCallback) resizeCallback(w, h);
        }
    }
    return shouldClose;
}

NativeHandle MacWindow::GetNativeHandle() const noexcept {
    return (NativeHandle)view;
}

void MacWindow::SetResizeCallback(std::function<void(int, int)> callback) {
    resizeCallback = std::move(callback);
}