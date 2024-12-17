all: clone_depencies compile

clone_depencies:
	git clone https://github.com/malisipi/lunaris || (cd lunaris && git pull && cd -)

compile:
	clang++ -O3 src/main.cpp -o linux_keyboard_osd_notifier.out -Ilunaris -lwayland-client -lX11 -lxkbcommon
