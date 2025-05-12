# Linux Keyboard Tray Indicator

> A basic tray indicator that notifies capslock and numlock states

![Showcase Tray](./assets/showcase_tray.png)

## Features

- System consistent design (for KDE)
- Works with user-space permissions
- A tray indicator that shows active states of modifier keys

## Why?

Some keyboards have no light for indicating capslock and numlock states *(like mine)*. And there's no way to understand the active state until press the key. And most OEMs won't provide some mini-tools like that for Linux. So I made a Tray Indicator for Linux.

## Working Mechanism

The application is hooking modifier keys via X11 to handle state changes (since Wayland doesn't provide a way without requiring root permissions).

## FAQ

- I am using Wayland however indicator is not working.
    > Please ensure your system is supporting XWayland and ensure a composer setting is not blocking listening modifier key events to prevent data leaks.

    > If you are using KDE, you can change the behavior via `System Settings > Security & Privacy > Application Permission > Legacy X11 App Support`.
    >
    > ![Legacy X11 App Support](./assets/legacy_x11_app_support.png)

- Why repo name includes OSD, does it supports a OSD view?
    > Previously was supporting OSD however it was working badly. And limiting target desktop environment and display servers. Even inside working desktop environments, it may steal window focus if you're working on windows that use XWayland. And it was a deal breaker for X11 display servers since OSD was created by Wayland protocols.

    > It may reimplemented in future however not promised.

## License

This project is licensed by [Mozilla Public License 2.0](./LICENSE)
