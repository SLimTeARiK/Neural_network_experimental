

#if defined(_WIN32)

#include <windows.h>

void simulate_click(int x, int y) {
    SetCursorPos(x, y);

    INPUT input_down = {0};
    input_down.type = INPUT_MOUSE;
    input_down.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input_down, sizeof(INPUT));

    INPUT input_up = {0};
    input_up.type = INPUT_MOUSE;
    input_up.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input_up, sizeof(INPUT));
}

#elif defined(__linux__) || defined(__APPLE__)

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

void simulate_click(int x, int y) {
    Display *display = XOpenDisplay(NULL);
    if (!display) return;

    XWarpPointer(display, None, DefaultRootWindow(display), 0, 0, 0, 0, x, y);
    XFlush(display);

    XTestFakeButtonEvent(display, 1, True, CurrentTime);
    XTestFakeButtonEvent(display, 1, False, CurrentTime);
    XFlush(display);

    XCloseDisplay(display);
}

#else

void simulate_click(int x, int y) {
    std::cerr << "Unsupported platform!\n";
}

#endif
