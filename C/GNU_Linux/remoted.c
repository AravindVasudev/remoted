#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

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
    char *response = "HTTP/1.1 200 OK\r\nContent-Length:14\r\n\r\n<h1>Done.</h1>\r\n";

    regex_t url;
    regmatch_t match;
    char extracted_url[512], open_command[512];

    Display *display;
    KeyCode alt_keycode, f4_keycode;

    // Compile Regex
    if ( regcomp(&url, "?url=", 0) ) {
        perror("Cannot compile regular expression");
        exit(EXIT_FAILURE);
    }    

    // Open X Display
    display = XOpenDisplay(NULL);
    Screen *sc = DefaultScreenOfDisplay(display);

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

    while (1) {
        // Accept Client
        if ( (new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0 ) {
            perror("Accept");
            exit(EXIT_FAILURE);
        }

        valread = read(new_socket, buffer, 1024);
        printf("%s\n", buffer);

        if ( strstr(buffer, "/favicon.ico") ) {
            // Ignore favicon requests
            continue;
        } else if ( strstr(buffer, "/close") ) {
            // Press Alt + F4 in X server
            alt_keycode = XKeysymToKeycode(display, XK_Alt_L);
            f4_keycode = XKeysymToKeycode(display, XK_F4);

            XTestFakeKeyEvent(display, alt_keycode, True, 0);
            XTestFakeKeyEvent(display, f4_keycode, True, 0);

            XTestFakeKeyEvent(display, f4_keycode, False, 0);
            XTestFakeKeyEvent(display, alt_keycode, False, 0);

            XFlush(display);
        } else if ( strstr(buffer, "/shutdown") ) {
            // Shutdown
            FILE *fp = popen("shutdown -r now", "r");
            pclose(fp);
        } else if ( strstr(buffer, "/open") ) {
            // Extract the URL from the Header and open it
            if ( !regexec(&url, buffer, 1, &match, 0) ) {
                sprintf(extracted_url, "%*s", match.rm_eo - match.rm_so, buffer + match.rm_so + 5);
            } else {
                strcpy(extracted_url, "https://google.com");
            }

            snprintf(open_command, sizeof(open_command), "xdg-open %s", strtok(extracted_url, " "));
            system(open_command);
        } else if ( strstr(buffer, "/mouse/circle") ) {
            
            XWarpPointer(display, None, 0, 0, 0, 0, 0, 200, 200);
            XFlush(display);
        }

        // respond to client
        send(new_socket, response, strlen(response), 0);
        printf("Response Sent!\n");
    }

    return 0;
}