#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    Display *display;
    KeyCode alt_keycode, f4_keycode;

    // Open X Display
    display = XOpenDisplay(NULL);

    // Socket File Descriptor
    if ( !(server_fd = socket(AF_INET, SOCK_STREAM, 0)) ) {
        perror("Socket Failed!");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to port 8080
    if ( (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) ) {
        perror("Setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Attaching socket to port 8080
    if ( bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0 ) {
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if ( listen(server_fd, 3) < 0 ) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    // Accept Client
    if ( (new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0 ) {
        perror("Accept");
        exit(EXIT_FAILURE);
    }

    // Shutdown
    // FILE *fp = popen("shutdown -r now", "r");
    // pclose(fp);

    // Press Alt + F4 in X server
    alt_keycode = XKeysymToKeycode(display, XK_Alt_L);
    f4_keycode = XKeysymToKeycode(display, XK_F4);

    XTestFakeKeyEvent(display, alt_keycode, True, 0);
    XTestFakeKeyEvent(display, f4_keycode, True, 0);

    XTestFakeKeyEvent(display, f4_keycode, False, 0);
    XTestFakeKeyEvent(display, alt_keycode, False, 0);

    XFlush(display);

    return 0;
}