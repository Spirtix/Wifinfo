#import <Cocoa/Cocoa.h>
#import <MetalKit/MetalKit.h>

#include <imgui.h>
#include "Window.h"

@interface WindowDelegate : NSObject<NSWindowDelegate>
{
@public
    Window* cppWindow;
}
@end

@implementation WindowDelegate
- (void)windowWillClose:(NSNotification *)notification {
    cppWindow->shouldClose = true;
}
- (void)windowDidResize:(NSNotification *)notification {
    if (cppWindow->resizeCallback) {
        NSWindow* win = (NSWindow*)notification.object;
        NSRect frame = [win contentRectForFrameRect:win.frame];
        cppWindow->resizeCallback(frame.size.width, frame.size.height);
    }
}
@end

#import <MetalKit/MetalKit.h>

Window::Window() {
    @autoreleasepool {
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

        NSRect frame = NSMakeRect(0, 0, 1280, 720);
        NSWindow* win = [[NSWindow alloc]
            initWithContentRect:frame
                      styleMask:(NSWindowStyleMaskTitled |
                                 NSWindowStyleMaskClosable |
                                 NSWindowStyleMaskResizable)
                        backing:NSBackingStoreBuffered
                          defer:NO];
        [win setTitle:@"Wifinfo"];

        id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        MTKView* v = [[MTKView alloc] initWithFrame:frame device:device];
        [win setContentView:v];

        WindowDelegate* d = [WindowDelegate new];
        d->cppWindow = this;
        [win setDelegate:d];
        [win makeKeyAndOrderFront:nil];
        [NSApp activateIgnoringOtherApps:YES];

        nsWindow = win;
        view = v;
        delegate = d;
        shouldClose = false;
    }
}

Window::~Window() {
    @autoreleasepool {
        [(NSWindow*)nsWindow close];
        [(WindowDelegate*)delegate release];
    }
}

bool Window::ShouldClose() {
    @autoreleasepool {
        NSEvent* event;
        while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                          untilDate:[NSDate distantPast]
                                             inMode:NSDefaultRunLoopMode
                                            dequeue:YES])) {
            [NSApp sendEvent:event];
        }
    }
    return shouldClose;
}

void* Window::GetNativeHandle() const noexcept {
    return view;
}

void Window::SetResizeCallback(std::function<void(int, int)> callback) {
    resizeCallback = callback;
}