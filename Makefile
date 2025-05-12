all: compile

compile:
	clang++ -O3 src/main.cpp -o linux_keyboard_osd_notifier.out `pkg-config --cflags --libs appindicator3-0.1 wayland-client xkbcommon x11`

compile_fast:
	clang++ -O0 src/main.cpp -o linux_keyboard_osd_notifier.out `pkg-config --cflags --libs appindicator3-0.1 wayland-client xkbcommon x11`
