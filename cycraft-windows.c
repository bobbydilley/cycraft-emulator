#include <winsock2.h>
#include <stdio.h>
#include <stdio.h>
#include <stdint.h>
#include <ws2tcpip.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * Cycraft Emulator
 * Written by: Bobby Dilley
 * Converted by: RεtroFΔn⊗
 * 
 * This emulates a very small subset of the functionality of a cycraft for
 * use in F-Zero AX Monster Ride, Initial D 3 Cycraft and Club Kart Cycraft.
 * 
 * Make sure to set your own IP address to 192.168.11.111 (for F-Zero AX).
 * 
 * The command bytes buffer[0] seem to look like this:
 * 
 * 0x90 - Get the status
 * 0x72 - Goto a safe position for the user to get in and out of the craft
 * 0x86 - Position request, look for other bytes to get position
 * 
 * The response status bytes are laid out as follows:
 * 
 * 1,2 Unknown (Pos Bits) (Off 0, 1 Seems to be bad, Initial 2, Neutral 3)
 * 3 required to skip the check (NormalBit)
 * 4 crashes it (SysErr Bit)
 * 5 (Driving Bit)
 * 6,7 Nothing?
 * 8 seatbelt (SeatBelt)
 * 9 motion stop (CabinStop)
 * 10 sensor (safetySensor)
 * 11 emergency (EmergencyStop)
 * 16 IllegalVersion
 **/

    
// The PORT to listen on, this was found by wire sharking the triforce
#define PORT 2000

// Turns debug output on
#define DEBUG_OUTPUT 0

// Represent the state of the cycraft, this isn't yet used.
typedef struct
{
	int CabinStop;
	int Driving;
	int EmergencyStop;
	int IllegalVersion;
	int Normal;
	int Pos;
	int SafetySensor;
	int SeatBelt;
	int SysErr;
} CycraftState;

/**
 * Cycraft Emulation Thread
 * 
 * Simply ignores everything it gets sent, and just tells the cycraft the status is good!
 */
int main(int argc, char *argv[])
{
	
static WSADATA wsaData;
int wsaerr = WSAStartup(MAKEWORD(2, 2), &wsaData);
if (wsaerr)
  exit(1);


	printf("Cycraft Emulator Version 1.0\n");
	printf("Bobby Dilley 2021 ported to windows and modified for dolphin\n");

	char data[0x30] = {0};

	int new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	uint8_t buffer[0x30] = {0};

	// Creating socket file descriptor
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0)
	{
		perror("Error: Failed to create socket\n");
		return EXIT_FAILURE;
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , (char*)&opt, sizeof(opt)))
	{
		perror("Error: Failed to set socket options\n");
		return EXIT_FAILURE;
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		printf("Error: Failed to bind socket\n");
		return EXIT_FAILURE;
	}

	if (listen(server_fd, 3) < 0)
	{
		printf("Error: Listen failed");
		return EXIT_FAILURE;
	}
	while (1)
	{
		printf("Waiting for gameboard to connect on port %d\n", PORT);

		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
		{
			printf("Error: Failed to accept\n");
			return EXIT_FAILURE;
		}

		// Bit 3 (normal mode) and bit 8 for seatbelt are set, and bits 1 and 2 are set to 2 for the position
		uint32_t status = 0x86;

		printf("Gameboard connected\n");

		while (1)
		{
			if (recv(new_socket, buffer, 0x30,0) ==0xffffffff)
			
			{
				printf("Error: Connection lost!\n");
				break;
			}
			
			// If we get the status command, reply with the status
			else if (buffer[0] == 0x90)
			{
				send(new_socket, (char*)&status, 2, 0);
				}
		}
	}

	return EXIT_SUCCESS;
WSACleanup();

}
