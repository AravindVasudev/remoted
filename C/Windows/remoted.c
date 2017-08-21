#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080

int main(int argc, char *argv[]) {
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c = sizeof(struct sockaddr_in);

	char buffer[2000], bsize;
	char *response = "HTTP/1.1 200 OK\r\nContent-Length:14\r\n\r\n<h1>Done.</h1>\r\n";

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
		if (strstr(buffer, "/close")) {
			// Press Alt + F4
			// TODO
		}
		else if (strstr(buffer, "/shutdown")) {
			// Shutdown
			system("shutdown -s -t 0");
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