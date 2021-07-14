#ifndef __KILL_BALL_H__
#define __KILL_BALL_H__

#include <stdint.h>
#include <future>
#include <unordered_map>
#include <string>

#ifndef KILLBALL_SERVER
#include "render/material.h"
#endif

#define P_NONE  -1
#define P_GOLDE  0
#define P_PEELY  1
#define P_GRANA  2
#define P_SLABO  3

namespace killball {

    const int32_t kMaxConnections = 32;
    const int32_t kMaxPlayers = 4;
    const int32_t kLogSize = 64;

    enum class ConDataType
    {
        NONE = 0,
        CONNECT,
        PLAY,
        START,
        SPAWN_BALL,
        DEATH,
        LOG,
        UPDATE_STATUS,
    };

    enum StateCode
    {
        S_MATCH_PLAYING = -10,
        S_MATCH_WAITING_FOR_PLAYERS = -11,
        S_MATCH_FINISHED = -12,

        S_PLAYER_NONE = -19,
        S_PLAYER_CONNECTED = -20,
        S_PLAYER_READY = -21,
        S_PLAYER_PLAYING = -22,
        S_PLAYER_DEAD = -23,
        S_PLAYER_WINNER = -24,
        S_PLAYER_DISCONNECTED = -25,
    };

    struct State
    {
        State()
        {
            match_state_ = S_MATCH_WAITING_FOR_PLAYERS;
            memset(players_, '\0', 64);
            for (int i = 0; i < 4; ++i)
            {
                player_states_[i] = S_PLAYER_NONE;
            }
        }
        void reset()
        {
            match_state_ = S_MATCH_WAITING_FOR_PLAYERS;
            memset(players_, '\0', 64);
            for (int i = 0; i < 4; ++i)
            {
                player_states_[i] = S_PLAYER_NONE;
            }
        }
        char players_[4][16];
        StateCode match_state_;
        StateCode player_states_[4];
    };

    union PkgData
    {
        PkgData() { match_state_ = State(); }
        char log_[kLogSize];
        State match_state_;
    };

    struct ConPkg
    {
        ConDataType type_;
        int32_t handle_;
        PkgData data_;
    };

    struct Match
    {
        Match()
        {
            for (int32_t i = 0; i < kMaxPlayers; ++i)
            {
                handles_[i] = -1;
            }
            start_count_ = 0;
            num_players_ = 0;
        }

        void reset()
        {
            for (int32_t i = 0; i < kMaxPlayers; ++i)
            {
                handles_[i] = -1;
            }
            start_count_ = 0;
            num_players_ = 0;
        }
        int32_t handles_[kMaxPlayers];
        int32_t start_count_;
        int32_t num_players_;

        int32_t getPlayer(int32_t handle)
        {
            for (int32_t i = 0; i < num_players_; ++i)
            {
                if (handles_[i] == handle)
                {
                    return i;
                }
            }
            return -1;
        }
    };

    struct Client
    {
        std::future<void> fut_;
        void* socket_;
        int32_t handle_ = -1;
        bool in_match_ = false;
        std::atomic_bool connected_;
    };
#ifndef KILLBALL_SERVER
    struct GameData
    {
        ::leep::Material mat_cubes_[4];
        ::leep::Material mat_ball_;
    };

    class KillBallGame
    {
    public:
        GameData data_;
        int32_t player_ = -1;
        State state_;
        bool playing_ = false;
        bool new_match_ = false;
    };

    inline KillBallGame& GetGame()
    {
        static KillBallGame game_;
        return game_;
    }
#endif // KILLBALL_SERVER

}// namespace killball

#endif // __KILL_BALL_H__