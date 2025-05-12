#include <X11/Xlib.h>

#define THE_KEY_CAPSLOCK 66
#define THE_KEY_NUMLOCK 77

void send_key_event(int keycode, bool state);

void keyboard_event_handler() {
    XEvent event;

    Display* display = XOpenDisplay(NULL);
    if (display == NULL) exit(1);

    Window root = DefaultRootWindow(display);
    XGrabKey(display, THE_KEY_CAPSLOCK, AnyModifier, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(display, THE_KEY_NUMLOCK, AnyModifier, root, True, GrabModeAsync, GrabModeAsync);

    while (1) {
        XNextEvent(display, &event);
        if (event.type == KeyPress) {
            if(event.xkey.keycode == THE_KEY_CAPSLOCK) send_key_event(THE_KEY_CAPSLOCK, (event.xkey.state&0x02) == 0);
            if(event.xkey.keycode == THE_KEY_NUMLOCK) send_key_event(THE_KEY_NUMLOCK, (event.xkey.state&0x10) == 0);
        };
    };

    XCloseDisplay(display);
}
