#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

void runSimulation() {
#ifdef _WIN32
    INPUT input = {0};
    input.type = INPUT_MOUSE;

    int distance = 200; // Przesunięcie w pikselach
    int duration = 1000; // Czas trwania ruchu w milisekundach

    POINT startPoint;
    GetCursorPos(&startPoint);

    int simulationDuration = 3600000;

    long startTime = GetTickCount();
    while (GetTickCount() - startTime < simulationDuration) {
        for (int i = 0; i < distance; i++) {
            startPoint.y -= 1;
            input.mi.dx = startPoint.x - startPoint.x;
            input.mi.dy = startPoint.y - startPoint.y;
            input.mi.dwFlags = MOUSEEVENTF_MOVE;
            SendInput(1, &input, sizeof(INPUT));
            Sleep(duration / distance);
        }

        for (int i = 0; i < distance; i++) {
            startPoint.y += 1;
            input.mi.dx = startPoint.x - startPoint.x;
            input.mi.dy = startPoint.y - startPoint.y;
            input.mi.dwFlags = MOUSEEVENTF_MOVE;
            SendInput(1, &input, sizeof(INPUT));
            Sleep(duration / distance);
        }
    }
#else
    // X Window System (Linux, macOS)
    Display* display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Nie można otworzyć ekranu X11\n");
        exit(1);
    }

    int screen = DefaultScreen(display);
    int rootWindow = RootWindow(display, screen);

    int distance = 200; // Przesunięcie w pikselach
    int duration = 1000; // Czas trwania ruchu w milisekundach

    XEvent event;
    XQueryPointer(display, rootWindow, &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

    int simulationDuration = 3600000;

    long startTime = getTimestamp();
    while (getTimestamp() - startTime < simulationDuration) {
        for (int i = 0; i < distance; i++) {
            event.xbutton.y_root--;
            XWarpPointer(display, None, rootWindow, 0, 0, 0, 0, event.xbutton.x_root, event.xbutton.y_root);
            XFlush(display);
            usleep(duration * 1000 / distance);
        }

        for (int i = 0; i < distance; i++) {
            event.xbutton.y_root++;
            XWarpPointer(display, None, rootWindow, 0, 0, 0, 0, event.xbutton.x_root, event.xbutton.y_root);
            XFlush(display);
            usleep(duration * 1000 / distance);
        }
    }

    XCloseDisplay(display);
#endif
}

long getTimestamp() {
#ifdef _WIN32
    return GetTickCount();
#else
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * 1000 + currentTime.tv_usec / 1000;
#endif
}

int main() {
    char response;
    do {
        runSimulation();
        printf("Czy chcesz uruchomić symulację ponownie? (T/N): ");
        scanf(" %c", &response);
    } while (response == 'T' || response == 't);

    printf("Mam nadzieję, że symulator był przydatny.\n");
    printf("\nTo rzekła Anarchy Foxy\n");

    return 0;
}
