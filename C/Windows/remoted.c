#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

#define WINVER 0x0500
#include <windows.h>

#define PORT 8080

int main(int argc, char *argv[]) {
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c = sizeof(struct sockaddr_in);

	char buffer[2000], bsize;
	char *response = "HTTP/1.1 200 OK\r\nContent-Length:14\r\n\r\n<h1>Done.</h1>\r\n";
	char *sub_url, extracted_url[512], open_command[512];

	// Set keyboard
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	// Set mouse
	INPUT mouse;
	mouse.type = INPUT_MOUSE;
	mouse.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
	mouse.mi.time = 0; //System will provide the timestamp
	mouse.mi.dwExtraInfo = 0;
	mouse.mi.mouseData = 0;

	// Screen Size
	int centerX = GetSystemMetrics(SM_CXSCREEN) / 2;
	int centerY = GetSystemMetrics(SM_CYSCREEN) / 2;

	int radius = centerX - 10;
	int angle, radians;

	printf("%d:%d\n\n", centerX, centerY);

	// Init Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa)) {
		printf("Failed: Error Code -> %d\n", WSAGetLastError());
		return 1;
	}

	// Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Failed: Could not create socket -> %d\n", WSAGetLastError());
	}

	// Creating sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	// Attaching socket to port 8080
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
		printf("Failed: Bind Failed -> %d\n", WSAGetLastError());
	}

	// Start listening
	listen(s, 3);

	// Accept Client
	while ( (new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET ) {
		
		// Get message from client
		if ((bsize = recv(new_socket, buffer, 2000, 0)) == SOCKET_ERROR) {
			printf("Failed: Recv Failed -> %d\n", WSAGetLastError());
		}
		puts(buffer);

		// Perform action based on request path
		if (strstr(buffer, "/favicon.ico")) { // Ingore /favicon.ico requests
			continue;
		} else if (strstr(buffer, "/close")) { // Press Alt + F4
			// Press Alt
			ip.ki.wVk = VK_MENU;
			ip.ki.dwFlags = 0;
			SendInput(1, &ip, sizeof(INPUT));

			// Press F4
			ip.ki.wVk = VK_F4;
			ip.ki.dwFlags = 0;
			SendInput(1, &ip, sizeof(INPUT));

			// Release F4
			ip.ki.wVk = VK_F4;
			ip.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &ip, sizeof(INPUT));

			// Release Alt
			ip.ki.wVk = VK_MENU;
			ip.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &ip, sizeof(INPUT));

		}
		else if (strstr(buffer, "/shutdown")) {
			// Shutdown
			system("shutdown -s -t 0");
		}
		else if (strstr(buffer, "/open")) {
			// Opens the URL passed via `url` query params
			if (sub_url = strstr(buffer, "?url=")) {
				sprintf(extracted_url, "%s", strtok(sub_url + 5, " "));
			} else { // if `url` is not passed, uses a default URL
				strcpy(extracted_url, "https://google.com");
			}

			sprintf(open_command, "start %s", extracted_url);
			system(open_command);
		}
		else if (strstr(buffer, "/mouse/circle")) {
			// draw a circle
			for (angle = 0; angle < 360; angle++) {
				radians = angle * (M_PI / 180);

				mouse.mi.dx = centerX + radius * cos(radians);
				mouse.mi.dy = centerY + radius * sin(radians);

				mouse.mi.dx *= (65536 / GetSystemMetrics(SM_CXSCREEN));
				mouse.mi.dy *= (65536 / GetSystemMetrics(SM_CYSCREEN));

				SendInput(1, &mouse, sizeof(mouse));
				Sleep(100);
			}
		}

		// Response to Client
		send(new_socket, response, strlen(response), 0);
	}
	if ( new_socket == INVALID_SOCKET ) {
		printf("Failed: Invalid Client -> %d\n", WSAGetLastError());
	}

	closesocket(s);
	WSACleanup();

	return 0;
}