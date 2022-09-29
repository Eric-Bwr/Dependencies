#pragma once

#ifdef _WIN32
#define NETWORK_WIN
#include <ws2tcpip.h>
#include <ws2def.h>
#include <winsock2.h>

#define CloseSocket(fd) closesocket(fd)
typedef int socklen;
typedef WSAPOLLFD PollFD;
#define Poll(x,y,z) WSAPoll(x,y,z)
#else
#define NETWORK_LINUX
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#define INVALID_SOCKET ~0
#define SOCKET_ERROR -1
#define CloseSocket(fd) close(fd)
typedef socklen_t  socklen;
#define htonll(x) htobe64(x)
#define htond(x) htobe64(x)
#define htonf(x) htobe32(x)
#define ntohll(x) be64toh(x)
#define ntohd(x) be64toh(x)
#define ntohf(x) be32toh(x)
typedef pollfd PollFD;
#define Poll(x,y,z) poll(x,y,z)
#endif
