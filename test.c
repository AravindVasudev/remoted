#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <stdio.h>
#include <stdlib.h>
 
int main() {
    Display *display;
    unsigned int keycode;
    display = XOpenDisplay(NULL);

    keycode = XKeysymToKeycode(display, 'A');
    XTestFakeKeyEvent(display, keycode, True, 0);
    XTestFakeKeyEvent(display, keycode, False, 0);
    XFlush(display);
}