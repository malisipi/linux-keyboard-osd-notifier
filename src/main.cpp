#include <stdlib.h>

bool is_caps_lock = true;
bool is_pressed = true;
bool caps_lock_state = false;
bool num_lock_state = false;

#include "event_listener.cpp"
#include "tray.cpp"

void send_key_event(int keycode, bool state){
    is_caps_lock = THE_KEY_CAPSLOCK == keycode;
    is_pressed = state;
    if(is_caps_lock){
        caps_lock_state = state;
    } else {
        num_lock_state = state;
    };
    tray_update_state(caps_lock_state, num_lock_state);
};

int main() {
    tray_loop();
    keyboard_event_handler();
    return 0;
};
