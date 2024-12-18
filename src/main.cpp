#include "lunaris/windowing.hpp"

#define POPUP_VISIBLE_TIME 600 /*ms*/

lunaris::window* win;
bool is_caps_lock = true;
bool is_pressed = true;
uint64_t last_notify_id = 0;
lunaris::styles::color_palette* colors;
bool caps_lock_state = false;
bool num_lock_state = false;

#include "event_listener.cpp"
#include "tray.cpp"

void* hide_notify(void* notify_id_ptr){
    uint64_t* notify_id = (uint64_t*)notify_id_ptr;
    usleep(POPUP_VISIBLE_TIME * 1000);
    if(*notify_id == last_notify_id) win->set_visibility(false);
    free(notify_id_ptr);
    return NULL;
}

void send_key_event(int keycode, bool state){
    win->set_visibility(true);
    is_caps_lock = THE_KEY_CAPSLOCK == keycode;
    is_pressed = state;
    if(is_caps_lock){
        caps_lock_state = state;
    } else {
        num_lock_state = state;
    };
    tray_update_state(caps_lock_state, num_lock_state);

    pthread_t hide_notify_thread;
    uint64_t* notify_id = (uint64_t*)malloc(sizeof(uint64_t));
    *notify_id = ++last_notify_id;
    if (pthread_create(&hide_notify_thread, NULL, hide_notify, notify_id) != 0) exit(1);
    pthread_detach(hide_notify_thread);
};

int main() {
    tray_loop();
    win = lunaris::new_window();
    colors = lunaris::styles::generate_color_palette_from_system();
    win->set_decoration(false);
    win->set_title((char*)"<Keyboard-OSD>");

    /* Set clickable through window, (window will not handle mouse events) */
    lunaris::wl_region* empty_region = wl_compositor_create_region(win->__compositor);

    wl_surface_set_input_region(win->__surface, empty_region);
    wl_surface_set_opaque_region(win->__surface, empty_region);
    wl_surface_commit(win->__surface);

    wl_region_destroy(empty_region);

    win->set_fullscreen(true);
    win->draw_handler = [](lunaris::window* win, uint32_t* buffer){
        win->graphics.rect(0, 0, win->width, win->height, 0x00000000); // Clear previous buffer

        const int popup_pos_x = win->width/2-150;
        const int popup_pos_y = win->height/2+125;
        const int popup_pos_w = 300;
        const int popup_pos_h = 50;

        uint32_t the_color = colors->hover_color;
        if(is_pressed) the_color = colors->text_color;

        char osd_description[256];
        char osd_symbol[4];

        if(is_caps_lock){
            sprintf(osd_description, "%s", (char*)"Caps Lock");
            sprintf(osd_symbol, "%s", (char*)"A");
        } else {
            sprintf(osd_description, "%s", (char*)"Num Lock");
            sprintf(osd_symbol, "%s", (char*)"1");
        };
        if(is_pressed){
            sprintf(osd_description, "%s %s", osd_description, (char*)"On");
        } else {
            sprintf(osd_description, "%s %s", osd_description, (char*)"Off");
        };

        std::pair<int, int> osd_description_ba = win->graphics.text_bounding_area(30, osd_description);
        std::pair<int, int> osd_symbol_ba = win->graphics.text_bounding_area(24, osd_symbol);

        // Popup area
        win->graphics.rounded_rect(popup_pos_x, popup_pos_y, popup_pos_w, popup_pos_h, 10, colors->background_color);
        // Symbol
        win->graphics.rounded_rect_empty(popup_pos_x+(popup_pos_h-30)/2, popup_pos_y+(popup_pos_h-30)/2, 30, 30, 2, 4, the_color);
        win->graphics.text(popup_pos_x+(popup_pos_h-30)/2+(30-osd_symbol_ba.first)/2, popup_pos_y+(popup_pos_h-30)/2+(30-osd_symbol_ba.second)/2, 24, osd_symbol, the_color);
        // Description
        win->graphics.text(popup_pos_x+50+(popup_pos_w-50-osd_description_ba.first)/2, popup_pos_y+(popup_pos_h-osd_description_ba.second)/2, 30, osd_description, the_color);
    };

    pthread_t keyboard_event_thread;
    if (pthread_create(&keyboard_event_thread, NULL, keyboard_event_handler, NULL) != 0) exit(1);
    pthread_detach(keyboard_event_thread);
    win->set_visibility(false);

    win->loop();
    return 0;
};
