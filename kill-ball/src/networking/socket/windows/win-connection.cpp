#include "networking/socket/connection.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "core/common-defs.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <future>

#pragma comment(lib, "ws2_32.lib")

namespace leep {

void Connection::platformRecv(void *con_data)
{
    SOCKET s = (SOCKET)con_data;
    int32_t result = 0;
    while (connected_)
    {
        killball::ConPkg pkg;
        result = recv(s, (char*)&pkg, sizeof(killball::ConPkg), 0);
        if (result > 0)
        {
            recv_mtx_.lock();
            recv_data_.emplace_back(pkg);
            recv_mtx_.unlock();
        }
        else
        {
            printf("Client disconnected!\n");
            connected_ = false;
        }
    }
}

void Connection::platformConnection(const char *ip)
{
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    assert(result == 0);

    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* info;
    result = getaddrinfo(ip, "8888", &hints, &info);
    if (result != 0)
    {
        connected_ = false;
        LEEP_CORE_ERROR("getaddrinfo failed: {0}", result);
        WSACleanup();
        return;
    }

    // Create socket
    SOCKET connect_socket =
        socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (connect_socket == INVALID_SOCKET)
    {
        LEEP_CORE_ERROR("Unable to create client socket: {0}", WSAGetLastError());
        connected_ = false;
        freeaddrinfo(info);
        WSACleanup();
        return;
    }

    result = ::connect(connect_socket, info->ai_addr, (int)info->ai_addrlen);
    if (result == SOCKET_ERROR)
    {
        LEEP_CORE_ERROR("Unable to connect to server.");
        int err = WSAGetLastError();
        connected_ = false;
        freeaddrinfo(info);
        closesocket(connect_socket);
        WSACleanup();
        return;
    }
    else
    {
        LEEP_CORE_INFO("Connected to server.\n");
        connected_ = true;
        freeaddrinfo(info);
    }

    std::future<void> fut = std::async(std::launch::async, &Connection::platformRecv, this, (void*)connect_socket);
    while (connected_)
    {
        send_mtx_.lock();
        if (!send_data_.empty())
        {
            killball::ConPkg pkg = send_data_.front();
            send_data_.pop_front();
            send_mtx_.unlock();
            result = ::send(connect_socket, (char*)&pkg, sizeof(killball::ConPkg), 0);
            if (result == SOCKET_ERROR)
            {
                connected_ = false;
                LEEP_CORE_INFO("Disconnected from server on: %s.\n", ip);
            }
        }
        else
        {
            send_mtx_.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }

    LEEP_CORE_INFO("Client disconnect");
    closesocket(connect_socket);
}

} // namespace leep