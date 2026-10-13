#import <Cocoa/Cocoa.h>
#import <MetalKit/MetalKit.h>

#include "MacWindow.h"

struct MacWindow::Impl {
    NSWindow* window = nil;
    MTKView* view = nil;
};

MacWindow::MacWindow() : impl(std::make_unique<Impl>()) {
    @autoreleasepool {
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        [NSApp finishLaunching];

        NSRect frame = NSMakeRect(0, 0, 1280, 720);
        impl->window = [[NSWindow alloc]
            initWithContentRect:frame
                      styleMask:(NSWindowStyleMaskTitled |
                                 NSWindowStyleMaskClosable |
                                 NSWindowStyleMaskMiniaturizable |
                                 NSWindowStyleMaskResizable)
                        backing:NSBackingStoreBuffered
                          defer:NO];
        [impl->window setTitle:@"Wifinfo"];
        [impl->window setReleasedWhenClosed:NO];

        id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        impl->view = [[MTKView alloc] initWithFrame:frame device:device];
        [device release];
        [impl->window setContentView:impl->view];
        [impl->view release];

        [impl->window center];
        [impl->window makeKeyAndOrderFront:nil];
        [NSApp activateIgnoringOtherApps:YES];

        shouldClose = false;

        CGSize size = impl->view.drawableSize;
        lastWidth = (int)size.width;
        lastHeight = (int)size.height;
    }
}

MacWindow::~MacWindow() {
    @autoreleasepool {
        [impl->window close];
        [impl->window release];
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

        if (![impl->window isVisible])
            shouldClose = true;

        CGSize size = impl->view.drawableSize;
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
    return (NativeHandle)impl->view;
}

void MacWindow::SetResizeCallback(std::function<void(int, int)> callback) {
    resizeCallback = std::move(callback);
}
