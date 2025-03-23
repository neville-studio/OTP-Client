/**
* @file sntp.cpp
* @brief SNTP client implementation
*	
**/
//#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

#include "sntp.h"

#define INVALID_SNTP_TIMESTAMP_TIME 1200000
SNTPClient::SNTPClient()
{
	
}

SNTPClient::~SNTPClient()
{
	
}
std::string hexPad0(int n)
{
	if (n < 16)
		return "0" + std::to_string(n);
	return std::to_string(n);
}

void SNTPClient::transformNTPPackage(BYTE* recvBuf, int recvBufLen, int64_t send, int64_t recv)
{
	this->delay = (recvBuf[4] << 24) | (recvBuf[5] << 16) | (recvBuf[6] << 8) | recvBuf[7];
	this->dispersion = (recvBuf[8] << 24) | (recvBuf[9] << 16) | (recvBuf[10] << 8) | recvBuf[11];
	this->stratum = recvBuf[2];
	this->precision = recvBuf[3];
	

	uint32_t server_recv = (recvBuf[32] << 24) | (recvBuf[33] << 16) | (recvBuf[34] << 8) | recvBuf[35];
	int64_t server_recv_sec = (recvBuf[36] << 24) | (recvBuf[37] << 16) | (recvBuf[38] << 8) | recvBuf[39];

	uint32_t server_send = (recvBuf[40] << 24) | (recvBuf[41] << 16) | (recvBuf[42] << 8) | recvBuf[43];
	int64_t server_send_sec = (recvBuf[44] << 24) | (recvBuf[45] << 16) | (recvBuf[46] << 8) | recvBuf[47];

	this->resultTimestamp = (0LL + server_recv + int64_t(server_send - server_recv) / 2)* 1000 + (recv - send) / 2  + (server_send_sec - server_recv_sec) * 1000LL / (1LL << 32);

	this->resultTimestamp -= DEFAULT_NTP_TIMESTAMP_DELTA*1000 - (1LL<<32)*epoch;
	if (flag_of_sign == 1 && !(server_recv & 0x80000000)) {
		this->resultTimestamp += 1ULL << 32;
	}




	this->resultSource = std::to_string(recvBuf[16]) + "." + std::to_string(recvBuf[17]) + "." + std::to_string(recvBuf[18]) + "." + std::to_string(recvBuf[19]);
	if(stratum == 1)
		this->resultSource = std::string({(char)recvBuf[8],(char)recvBuf[9],(char)recvBuf[10],(char)recvBuf[11] });
	else if (ipv == 4)
		this->resultSource = std::to_string(recvBuf[8]) + "." + std::to_string(recvBuf[9]) + "." + std::to_string(recvBuf[10]) + "." + std::to_string(recvBuf[11]);
	else
		this->resultSource ="["+ std::to_string(recvBuf[8]) + hexPad0(recvBuf[9])+":" + std::to_string(recvBuf[10]) + hexPad0(recvBuf[11]) + "...]";
	this->last_update = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();;

}

int64_t SNTPClient::updateSNTPTimeStamp(const char* server, uint16_t port, bool enableIpV6)
{
	SOCKET m_socket;
	WSADATA m_wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData))
		std::cout << "start up failed." << std::endl;
	lastError = 0;
	SOCKET connectSocket = INVALID_SOCKET;
	BYTE recvBuf[DEFAULT_NTP_PACKET_SIZE] = {};
	int recvBufLen = DEFAULT_NTP_PACKET_SIZE;
	//struct sockaddr_in serverAddr;
	int iResult;

	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;

	BYTE sendBuf[DEFAULT_NTP_PACKET_SIZE] = { 0b00100011 };
	uint64_t timestamp = 0;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = enableIpV6 ? AF_INET6 : AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;


	iResult = getaddrinfo(server, std::to_string(port).c_str(), &hints, &result);
	if (iResult != 0) {
		std::cout << "getaddrinfo failed with error: " << GetLastError() << std::endl;
		status = -1;
		WSACleanup();
		return -1;
	}
	ptr = result;
	connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (connectSocket == INVALID_SOCKET) {
		std::cout << "socket failed with error: " << GetLastError() << std::endl;
		freeaddrinfo(result);
		status = -1;
		closesocket(connectSocket);
		WSACleanup();
		return -1;
		
	}

	DWORD timeout = 10000;
	setsockopt(connectSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	setsockopt(connectSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));


	std::chrono::time_point send_time = std::chrono::system_clock::now();


	iResult = sendto(connectSocket, (const char*)sendBuf, (int)sizeof(sendBuf), 0, ptr->ai_addr, ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(connectSocket);
		connectSocket = INVALID_SOCKET;
		status = -1;
		std::cout << "send socket failed with error: " << GetLastError() << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		return -1;
	}

	/*iResult = shutdown(connectSocket, SD_SEND);*/

	iResult = recvfrom(connectSocket, (char*)recvBuf, recvBufLen, 0, NULL, NULL);
	std::chrono::time_point recv_time = std::chrono::system_clock::now();
	if (iResult > 0) {
		// calculate the time difference between the time the packet was sent and the time the packet was received
		transformNTPPackage(recvBuf, recvBufLen, std::chrono::duration_cast<std::chrono::milliseconds>(send_time.time_since_epoch()).count(), std::chrono::duration_cast<std::chrono::milliseconds>(recv_time.time_since_epoch()).count());

		timestamp = resultTimestamp;

	}
	else {
		std::cout << "reveiving socket failed with error: " << GetLastError() << std::endl;
		timestamp = -1;
		status = -1;


	}
	freeaddrinfo(result);
	
	closesocket(connectSocket);
	WSACleanup();
	this->status = 0;
	return timestamp;
}


int64_t SNTPClient::getSNTPTimestamp()
{
	int64_t currentTime = this->last_update = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	if (currentTime - this->last_update < 12000000)
	{
		return currentTime - this->last_update + this->resultTimestamp;
	}
	else
	{
		return -1;
	}

	
}