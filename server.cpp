#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <array>
#include <time.h>

#include "server.h"
#define PORT 9987

using namespace std;

int main(int argc, char *argv[])
{
    int sockfd, p1sockfd,p2sockfd, bindfd, bytes_sent, bytes_recvd, i=0;
    char sbuffer[512], p1_ip[16],p2_ip[16], p2_name[64], p1_name[64];
    char *ptr_buff;
    const char *p1_ptr_ip,*p2_ptr_ip;
    struct sockaddr_in serv_addr, p1_addr, p2_addr;
    socklen_t serv_size, p1_size, p2_size;

    std::array<std::unique_ptr<std::thread>, 10> th;
    
    char p1_no, p2_no, g_size;

    freopen( "log.txt", "w", stdout );

    ptr_buff = sbuffer;

    // Creating sever side socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        error_detected("Server side listening Socket could not be created!");
        
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Binding socket
    bindfd = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (bindfd == -1)
    {
        error_detected("Failed to bind!");
        
    }

    // Listening for incoming connections
    cout << timedisp() << "Server created!" << endl << "Waiting for Players" << endl;

    // Server can handle a maximum of 10 games
    // A game is alloted to each pair of incoming connections
    // Multithreading is used
    while(i<10) {

        struct arg_struct args;

        if (listen(sockfd, 5) < 0) {
            error_detected("Failed to listen!");
            
        }

        serv_size = sizeof(serv_addr);
        p1_size = sizeof(p1_addr);

        // Awaiting a connection on socket FD from Player 1
        p1sockfd = accept(sockfd, (struct sockaddr *) &p1_addr, &p1_size);

        if (p1sockfd == -1) {
            error_detected("Failed to accept from client!");
        }

        p1_ptr_ip = inet_ntop(AF_INET, &p1_addr.sin_addr, p1_ip, p1_size);
        cout << endl << timedisp() << "Server received connections from " << p1_ip << endl;

        // Acquiring basic information from the client and assign their player ID
        memset(&p1_name, 0, sizeof(p1_name));
        do {
            static int flag = 0;
            bytes_recvd = recv(p1sockfd, &p1_name, sizeof(p1_name), 0);
            if (bytes_recvd == -1 && flag == 0) {
                memset(&p1_name, 0, sizeof(p1_name));
                cout << "Could not ACQUIRE Player Information!" << endl << "Trying again..." << endl;
                continue;
            } else {
                flag = 1;
                g_size = '1';
                bytes_sent = send(p1sockfd, &g_size, sizeof(g_size), 0);
                if (bytes_sent == -1)
                    cout << "Could not receive any Data!" << "Trying Again..." << endl;
                else
                    cout << endl << timedisp() << p1_name << " has joined the game." << endl;
                p1_no = '1';
            }
        } while (bytes_recvd == -1 || bytes_sent == -1);

        g_size = '1';

        p2_size = sizeof(p2_addr);

        // Awaiting a connection on socket FD from Player 2
        p2sockfd = accept(sockfd, (struct sockaddr *) &p2_addr, &p2_size);

        if (p2sockfd == -1) {
            error_detected("Failed to accept from client!");
        }


        p2_ptr_ip = inet_ntop(AF_INET, &p2_addr.sin_addr, p2_ip, p2_size);
        cout << endl << timedisp() << "Server received connections from " << p2_ip << endl;

        memset(&p2_name, 0, sizeof(p2_name));
        do {
            static int flag = 0;
            bytes_recvd = recv(p2sockfd, &p2_name, sizeof(p2_name), 0);
            if (bytes_recvd == -1 && flag == 0) {
                memset(&p2_name, 0, sizeof(p2_name));
                cout << "Could not ACQUIRE Player Information!" << endl << "Trying again..." << endl;
                continue;
            } else {
                flag = 1;
                bytes_sent = send(p2sockfd, &p1_name, sizeof(p1_name), 0);
                if (bytes_sent == -1)
                    cout << "Could not SEND Player Data!" << "Trying Again..." << endl;
                else
                    bytes_sent = send(p1sockfd, &p2_name, sizeof(p2_name), 0);
                if (bytes_sent == -1)
                    cout << "Could not SEND Player Data!" << "Trying Again..." << endl;
                else {
                    cout << endl << timedisp() << p2_name << " has joined the game." << endl;
                    p2_no = '0';
                }
            }
        } while (bytes_recvd == -1 || bytes_sent == -1);

        // Sending the client his/her player ID
        bytes_sent = send(p1sockfd, &p1_no, sizeof(p1_no), 0);
        if (bytes_sent == -1) {
            error_detected("Player No not sent!");
            
        }

        bytes_sent = send(p2sockfd, &p2_no, sizeof(p2_no), 0);
        if (bytes_sent == -1) {
            error_detected("Player No not sent!");
            
        }

        args.game_no=i+1;
        args.a1=p1sockfd;
        args.a2=p2sockfd;
        args.s1=p1_name;
        args.s2=p2_name;

        // Game is created between the active clients and assigned to a thread
        th[i] = std::make_unique<std::thread>(startgame, args);

        // Game size is incremented
        ++i;
    }

    // Wait for the threads to finish
    for (int i = 0; i < th.size(); ++i) {
        th[i]->join();
    }

    close(sockfd);
    return 0;
}