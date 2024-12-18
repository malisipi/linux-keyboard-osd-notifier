#include <libappindicator/app-indicator.h>

AppIndicator* indicator;

void* _tray_inner_loop(void*){
    if(gtk_init_check(0, NULL) == false) return NULL;
    indicator = app_indicator_new("linux_keyboard_osd_notifier", "", APP_INDICATOR_CATEGORY_SYSTEM_SERVICES);
    app_indicator_set_menu(indicator, GTK_MENU(gtk_menu_new()));
    while(1){
        gtk_main_iteration_do(true);
    };
    return NULL;
};

void tray_loop(void){
    pthread_t keyboard_event_thread;
    if (pthread_create(&keyboard_event_thread, NULL, _tray_inner_loop, NULL) != 0) exit(1);
    pthread_detach(keyboard_event_thread);
};

void tray_update_state(bool caps_lock_state, bool num_lock_state){
    app_indicator_set_status(indicator, (caps_lock_state || num_lock_state) ? APP_INDICATOR_STATUS_ACTIVE : APP_INDICATOR_STATUS_PASSIVE);
    if(caps_lock_state && num_lock_state) app_indicator_set_icon(indicator, "input-combo-on");
    else if(caps_lock_state && !num_lock_state) app_indicator_set_icon(indicator, "input-caps-on");
    else if(!caps_lock_state && num_lock_state) app_indicator_set_icon(indicator, "input-num-on");
}