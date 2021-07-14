#ifndef __LEEP_NETWORKING_SOCKET_CONNECTION_H__
#define __LEEP_NETWORKING_SOCKET_CONNECTION_H__

#include "kill-ball.h"
#include <list>
#include <mutex>
#include <future>

namespace leep
{
    class Connection
    {
    public:
        Connection();
        ~Connection();

        void connect(const char *ip);
        void disconnect();
        // TODO: Pass by reference or ptr since the struct has grown with
        // the log message
        void send(killball::ConPkg pkg);
        void dispatchRecv();
        bool connected() const;

    private:
        void platformConnection(const char *ip);
        void platformRecv(void *con_data); // con_data is SOCKET on windows
        std::mutex send_mtx_;
        std::mutex recv_mtx_;
        std::list<killball::ConPkg> send_data_;
        std::list<killball::ConPkg> recv_data_;
        std::future<void> fut_;
        int32_t handle_;
        std::atomic_bool connected_;
    };
}

#endif // __LEEP_NETWORKING_SOCKET_CONNECTION_H__