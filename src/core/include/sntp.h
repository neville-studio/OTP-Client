#pragma once
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <optional>
#include <cctype>
#include <array>
#include <cstdint>
//#include "common.h"
#define DEFAULT_NTP_PORT 123
#define DEFAULT_NTP_PACKET_SIZE 48
#define DEFAULT_NTP_TIMESTAMP_DELTA 2208988800ull
#define DEFAULT_NTP_VERSION 4
#define DEFAULT_NTP_MODE 3
#define DEFAULT_NTP_STRATUM 0
#define DEFAULT_NTP_POLL 4
#define DEFAULT_NTP_PRECISION -6
#define DEFAULT_NTP_ROOT_DELAY 0


#ifdef WIN32
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif

class SNTPClient
{
public:
	SNTPClient();
	~SNTPClient();
	
	int64_t getSNTPTimestamp(const char* server, uint16_t port = DEFAULT_NTP_PORT, bool EnableIpV6 = false);

	int64_t getLastUpdate() const { return last_update; }
	std::string getResultSource() const { return resultSource; }
	int64_t getResultTimestamp() const { return resultTimestamp; }
	int64_t getLastError() const { return lastError; }
	int32_t getPrecision() const { return precision; }
	int32_t getDelay() const { return delay; }
	int32_t getDispersion() const { return dispersion; }
	int32_t getStratum() const { return stratum; }


private:
	int64_t last_update = 0;
	std::string resultSource = "";
	int64_t resultTimestamp = 0;
	int64_t lastError;
	int32_t precision = 0;
	int32_t delay = 0;
	int32_t dispersion = 0;
	int32_t stratum = 0;
	int8_t ipv = 4;
	const int8_t epoch = 0;
	const bool flag_of_sign = true;
	
#ifdef WIN32
	SOCKET m_socket;
	WSAData m_wsaData;
	void transformNTPPackage(BYTE *recvBuf, int recvBufLen, int64_t send, int64_t recv);
#endif
};