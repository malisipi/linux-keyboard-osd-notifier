all: clone_depencies compile

clone_depencies:
	git clone https://github.com/malisipi/lunaris || (cd lunaris && git pull && cd -)

compile:
	clang++ -O3 src/main.cpp -o linux_keyboard_osd_notifier.out -Ilunaris `pkg-config --cflags --libs appindicator3-0.1 wayland-client xkbcommon x11`

compile_fast:
	clang++ -O0 src/main.cpp -o linux_keyboard_osd_notifier.out -Ilunaris `pkg-config --cflags --libs appindicator3-0.1 wayland-client xkbcommon x11`