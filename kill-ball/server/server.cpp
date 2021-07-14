#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "../include/kill-ball.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <assert.h>
#include <stdio.h>
#include <future>
#include <mutex>
#include <list>
#include <string>
#include <unordered_map>

#pragma comment(lib, "ws2_32.lib")

using namespace killball;

std::list<Client> clients;
Match current_match;
int32_t g_count = 0;
State g_state;
std::mutex clients_mtx;
std::unordered_map<int32_t, std::string> players;


void EndConnection(SOCKET s)
{
    clients_mtx.lock();
    for (auto &c : clients)
    {
        if (c.socket_ == (void*)s)
        {
            c.connected_ = false;
        }
    }
    clients_mtx.unlock();
}

// Generic send does not need return value since EndConnection handles
// the error
void GenericSend(SOCKET s, ConPkg* pkg)
{
    if (SOCKET_ERROR == send(s, (char*)pkg, sizeof(ConPkg), 0))
    {
        printf("Socket error sending, closing connection %d.\n", pkg->handle_);
        EndConnection(s);
    }
}

// Generic recv does not need return value since EndConnection handles
// the error
void GenericRecv(SOCKET s, ConPkg *pkg)
{
    if (SOCKET_ERROR == recv(s, (char*)pkg, sizeof(ConPkg), 0))
    {
        printf("Socket error reciving, closing connection.\n");
        EndConnection(s);
    }
}

void SendToAllClients(ConPkg *pkg, int32_t not_to_this = -1)
{
    std::list<void*> close_s;
    clients_mtx.lock();
    for (auto &c : clients)
    {
        if (c.handle_ == not_to_this)
        {
            continue;
        }
        // GenericSend here can not be done since the clients mutex is already
        // locked and EndConnection uses it
        if (SOCKET_ERROR == send((SOCKET)c.socket_, (char*)pkg, sizeof(ConPkg), 0))
        {
            printf("Client %d: Socket error sending all, closing connection.\n", c.handle_);
            close_s.push_back(c.socket_);
        }
    }
    clients_mtx.unlock();
    for (auto &s : close_s)
    {
        EndConnection((SOCKET)s);
    }
}

// Send spawn ball pkg to all alive players but the one who killed the ball
void SpawnBall(int32_t emitter)
{
    clients_mtx.lock();
    for (auto &c : clients)
    {
        int32_t player_index = current_match.getPlayer(c.handle_);
        if (player_index >= 0 && 
            player_index != emitter &&
            g_state.player_states_[player_index] == S_PLAYER_PLAYING)
        {
            ConPkg pkg;
            pkg.type_ = ConDataType::SPAWN_BALL;
            pkg.handle_ = emitter;
            GenericSend((SOCKET)c.socket_, &pkg);
        }
    }
    clients_mtx.unlock();
}

void StartGame()
{
    for (auto &c : clients)
    {
        if (c.in_match_)
        {
            ConPkg pkg;
            pkg.type_ = ConDataType::START;
            pkg.handle_ = current_match.getPlayer(c.handle_);
            GenericSend((SOCKET)c.socket_, &pkg);
        }
    }
}

void Connection(SOCKET client_socket, std::atomic_bool &connected)
{
    int result;
    int32_t handle = g_count++;

    ConPkg pkg;
    pkg.type_ = ConDataType::CONNECT;
    pkg.handle_ = handle; 
    GenericSend(client_socket, &pkg);
    // Need the two connected checks because GenericSend and GenericRecv
    // can cause disconexion
    if (connected) 
    {
        GenericRecv(client_socket, &pkg);
    }
    if (connected)
    {
        if (pkg.type_ == ConDataType::CONNECT && pkg.handle_ == handle)
        {
            for (auto &c : clients)
            {
                if ((SOCKET)c.socket_ == client_socket)
                {
                    c.handle_ = handle;
                }
            }
            printf("Client %d connected.\n", handle);
        }
        else
        {
            printf("Error establishing connection with new client.\n");
            closesocket(client_socket);
            return;
        }
    }

    // Client connected
    while (true)
    {
        if (!connected)
        {
            int32_t p = current_match.getPlayer(handle);
            if (p >= 0) 
            {
                g_state.player_states_[p] = S_PLAYER_DISCONNECTED;
            }
            if (g_state.match_state_ == S_MATCH_WAITING_FOR_PLAYERS)
            {
                printf("Resetting match due to disconnection of a player.\n");
                g_state.reset();
                current_match.reset();
            }
            closesocket(client_socket);
            printf("Client %d disconnected.\n", handle);
            return;
        }

        result = recv(client_socket, (char*)&pkg, sizeof(pkg), 0);
        if (result > 0)
        {
            switch(pkg.type_)
            {
                case ConDataType::PLAY:
                {
                    if (g_state.match_state_ == S_MATCH_PLAYING)
                    {
                        pkg.type_ = ConDataType::LOG;
                        pkg.handle_ = handle;
                        strncpy_s(pkg.data_.log_, "There is a match running. Wait until it finishes", kLogSize);
                        GenericSend(client_socket, &pkg);
                    }
                    else if (current_match.num_players_ < kMaxPlayers)
                    {
                        // Do nothing f the player is in the match
                        if (current_match.getPlayer(handle) != P_NONE && g_state.match_state_ != S_MATCH_FINISHED)
                        {
                            pkg.type_ = ConDataType::LOG;
                            pkg.handle_ = handle;
                            strncpy_s(pkg.data_.log_, "You are already on the match", kLogSize);
                            GenericSend(client_socket, &pkg);
                        }
                        else
                        {
                            // If previous game is finished, start new one
                            if (g_state.match_state_ == S_MATCH_FINISHED)
                            {
                                current_match.reset();
                                g_state.reset();
                                g_state.match_state_ = S_MATCH_WAITING_FOR_PLAYERS;
                            }
                            // If the player is not in the match, add him
                            std::string str(pkg.data_.log_, 16);
                            players.emplace(handle, str);
                            strncpy_s(g_state.players_[current_match.num_players_], players[handle].c_str(), 16);
                            g_state.player_states_[current_match.num_players_] = S_PLAYER_CONNECTED;
                            current_match.handles_[current_match.num_players_++] = handle;
                            for (auto& c : clients)
                            {
                                if (c.handle_ == handle)
                                {
                                    c.in_match_ = true;
                                }
                            }
                            pkg.type_ = ConDataType::LOG;
                            pkg.handle_ = handle;
                            strncpy_s(pkg.data_.log_, "Added to the next match. Press Ready when you are ready!", kLogSize);
                            GenericSend(client_socket, &pkg);
                        }
                    }
                    else
                    {
                        // Full match
                        pkg.type_ = ConDataType::LOG;
                        pkg.handle_ = handle;
                        strncpy_s(pkg.data_.log_, "The match is full", kLogSize);
                        GenericSend(client_socket, &pkg);
                    }
                    break;
                }

                case ConDataType::START:
                {
                    int32_t idx = current_match.getPlayer(handle);
                    if (idx >= 0 && g_state.player_states_[idx] == S_PLAYER_CONNECTED)
                    {
                        g_state.player_states_[current_match.getPlayer(handle)] = S_PLAYER_READY;
                        current_match.start_count_++;
                        if (current_match.start_count_ == current_match.num_players_)
                        {
                            g_state.match_state_ = S_MATCH_PLAYING;
                            for (int32_t i = 0; i < kMaxPlayers; ++i)
                            {
                                if (g_state.player_states_[i] == S_PLAYER_READY)
                                    g_state.player_states_[i] = S_PLAYER_PLAYING;
                            }
                            printf("All the connected players are ready, starting game.\n");
                            StartGame();
                        }
                    }
                    else
                    {
                        pkg.type_ = ConDataType::LOG;
                        pkg.handle_ = handle;
                        strncpy_s(pkg.data_.log_, "You must connect to the match first (Play button)", kLogSize);
                        GenericSend(client_socket, &pkg);
                    }
                    break;
                }

                case ConDataType::SPAWN_BALL:
                {
                    int32_t idx = current_match.getPlayer(handle);
                    assert(idx >= 0);
                    SpawnBall(idx);
                    break;
                }

                case ConDataType::DEATH:
                    g_state.player_states_[current_match.getPlayer(handle)] = S_PLAYER_DEAD;
                    break;

                default:
                    printf("Default type package.\n");
                    break;
            }
        }
        else
        {
            connected = false;
        }
    }
}

void ServerUpdate()
{
    while (true)
    {
        // Remove disconnected clients from future list
        clients_mtx.lock();
        auto i = clients.begin();
        while (i != clients.end())
        {
            auto status = i->fut_.wait_for(std::chrono::milliseconds(0));
            if (status == std::future_status::ready)
            {
                // erase returns next iterator
                i = clients.erase(i);
                if (clients.empty())
                {
                    // It is hard to reach MAX_INT anyway but better reset when server is empty
                    printf("All clients left, resetting server...\n");
                    g_count = 0;
                    g_state.reset();
                    current_match.reset();
                    players.clear();
                }
            }
            else
            {
                ++i;
            }
        }
        clients_mtx.unlock();

        if (g_state.match_state_ == S_MATCH_PLAYING)
        {
            int32_t playing = 0;
            int32_t index = -1;
            for (int32_t i = 0; i < kMaxPlayers; ++i)
            {
                if (g_state.player_states_[i] == S_PLAYER_PLAYING)
                {
                    ++playing;
                    index = i;
                }
            }
            if (playing == 0 || (playing == 1 && current_match.num_players_ > 1))
            {
                g_state.match_state_ = S_MATCH_FINISHED;
                if (index >= 0)
                {
                    g_state.player_states_[index] = S_PLAYER_WINNER;
                }
            }
        }

        ConPkg pkg;
        pkg.type_ = ConDataType::UPDATE_STATUS;
        pkg.data_.match_state_ = g_state;
        SendToAllClients(&pkg);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

int main()
{
    std::future<void> server_update_;
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    assert(result == 0 && "DLL not found. Old windows?");

    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* info;
    result = getaddrinfo(nullptr, "8888", &hints, &info);
    if (result != 0)
    {
        printf("getaddrinfo failed: %d\n", result);
        WSACleanup();
        return false;
    }
    // Create socket
    SOCKET listen_socket =
        socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (listen_socket == INVALID_SOCKET)
    {
        printf("Unable to create server socket: %d\n", WSAGetLastError());
        freeaddrinfo(info);
        WSACleanup();
        return false;
    }

    // Bind socket
    result = bind(listen_socket, info->ai_addr, (int)info->ai_addrlen);
    if (result == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(info);
        closesocket(listen_socket);
        WSACleanup();
        return false;
    }
    else
    {
        freeaddrinfo(info);
    }

    server_update_ = std::async(std::launch::async, &ServerUpdate);

    printf("Server initialized. Waiting for clients...\n");
    while (true)
    {
        result = listen(listen_socket, 8);
        if (result == SOCKET_ERROR)
        {
            printf("listen failed with error: %d\n", WSAGetLastError());
            continue;
        }

        // Accept client
        SOCKET client_socket = accept(listen_socket, nullptr, nullptr);
        if (client_socket == INVALID_SOCKET)
        {
            // No need to close socket here
            printf("accept failed with error: %d\n", WSAGetLastError());
            continue;
        }

        clients_mtx.lock();
        clients.emplace_back();
        clients.back().connected_ = true;
        clients.back().fut_ = std::async(std::launch::async, Connection, client_socket, std::ref(clients.back().connected_));
        clients.back().socket_ = (void*)client_socket;
        clients_mtx.unlock();
    }

    // Closing
    closesocket(listen_socket);
    WSACleanup();
    return true;
}