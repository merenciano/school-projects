#include "connection.h"
#include "core/common-defs.h"
#include "ecs/systems/ball-spawner.h"
#include "kill-ball.h"

namespace leep {

Connection::Connection()
{
    handle_ = -1;
    connected_ = false;
}

Connection::~Connection()
{
    connected_ = false;
}

void Connection::connect(const char *ip)
{
    if (connected_)
    {
        disconnect();
        // Making sure previous socket is closed
        fut_.get();
    }
    fut_ = std::async(std::launch::async, &Connection::platformConnection, this, ip);
}

void Connection::disconnect()
{
    connected_ = false;
    handle_ = -1;
    send_mtx_.lock();
    recv_mtx_.lock();
    send_data_.clear();
    recv_data_.clear();
    send_mtx_.unlock();
    recv_mtx_.unlock();
}

void Connection::send(killball::ConPkg pkg)
{
    send_mtx_.lock();
    send_data_.emplace_back(pkg);
    send_mtx_.unlock();
}

void Connection::dispatchRecv()
{
    using namespace killball;
    recv_mtx_.lock();
    while (!recv_data_.empty())
    {
        killball::ConPkg pkg = recv_data_.front();
        recv_data_.pop_front();
        switch(pkg.type_)
        {
            case ConDataType::CONNECT:
            {
                killball::ConPkg reply;
                reply.type_ = ConDataType::CONNECT;
                reply.handle_ = pkg.handle_;
                send(reply);
                break;
            }
            case ConDataType::LOG:
            {
                LEEP_INFO("Server: {0}", pkg.data_.log_);
                break;
            }

            case ConDataType::UPDATE_STATUS:
            {
                GetGame().state_ = pkg.data_.match_state_;
                if (GetGame().state_.match_state_ == S_MATCH_FINISHED)
                {
                    GetGame().playing_ = false;
                    GetGame().player_ = -1;
                }
                if (GetGame().player_ >= 0 && GetGame().state_.player_states_[GetGame().player_] == S_PLAYER_DEAD)
                {
                    GetGame().playing_ = false;
                }
                break;
            }

            case ConDataType::START:
            {
                // Reset match
                GetGame().new_match_ = true;
                GetGame().player_ = pkg.handle_; // In this particular case handle is player index
                GetGame().playing_ = true;
                break;
            }

            case ConDataType::SPAWN_BALL:
            {
                BallSpawner().spawnBall(GetGame().data_.mat_cubes_[pkg.handle_], 18.0f);
                break;
            }
        }
    }
    recv_mtx_.unlock();
}

bool Connection::connected() const
{
    return (bool)connected_;
}

} // namespace leep

#ifdef LEEP_WIN
#include "networking/socket/windows/win-connection.cpp"
#endif
#ifdef LEEP_LINUX
#include "networking/socket/linux/linux-connection.cpp"
#endif