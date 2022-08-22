
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex>
#include "tictactoe.h"

#define SOCKET_READ_TIMEOUT_SEC 10
#define TIME_OUT 0;

struct arg_struct
{
    int a1;
    int a2;
    int game_no;
    string s1;
    string s2;
};

auto timedisp()
{
    auto timenow =
            chrono::system_clock::to_time_t(chrono::system_clock::now());

    return ctime(&timenow);
}

int startgame(arg_struct);

int error_detected(const char *s)
{
    perror(s);
    return EXIT_FAILURE;
}

int timeout(int sockfd)
{
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set); /* clear the set */
    FD_SET(sockfd, &set); /* add our file descriptor to the set */
    timeout.tv_sec = SOCKET_READ_TIMEOUT_SEC;
    timeout.tv_usec = 0;
    int rv = select(sockfd + 1, &set, NULL, NULL, &timeout);
    if (rv == -1)
    {
        error_detected("No Data received");
    }
    else if (rv == 0)
    {
        cout<< endl << timedisp() <<" Move Timed out"<<endl;
        return 0;
    }

    return 1;
}

int startgame(arg_struct args)
{
    int bytes_sent, bytes_recvd;
    int count = 0, inp, ni, x, y, toss;
    char p2_choice, p1_choice;
    char choice_buffer[2], coord_buffer[2], toss_buffer;
    char g_rpl='1';
    char alive;

    struct arg_struct arguments = (struct arg_struct )args;

    int g_no = arguments.game_no;

    int p1sockfd = arguments.a1;
    int p2sockfd = arguments.a2;

    string p1_name = arguments.s1;
    string p2_name = arguments.s2;

    while(g_rpl=='1') {
        cout << timedisp() <<" "<<"Game No: "<<g_no<<"  "<< "Creating Game. Please wait..." << endl;
        sleep(2);
        cout << endl << timedisp() <<" "<<"Game No: "<<g_no<<"  "<< "Game created!" << endl;
        cout << endl << timedisp() <<" "<<"Game No: "<<g_no<<"  "<< "Doing a toss..." << endl;
        srand(time(NULL));
        toss = rand() % 2;
        sleep(1);
        sprintf(&toss_buffer, "%d", toss);
        bytes_sent = send(p1sockfd, &toss_buffer, sizeof(toss_buffer), 0);
        if (bytes_sent == -1) {
            error_detected("TOSS BUFFER not sent!");
        }

        bytes_sent = send(p2sockfd, &toss_buffer, sizeof(toss_buffer), 0);
        if (bytes_sent == -1) {
            error_detected("TOSS BUFFER not sent!");
        }

        if (toss == 0) {
            cout << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< p2_name << " won the toss!" << endl;

            memset(&choice_buffer, 0, sizeof(choice_buffer));

            bytes_recvd = recv(p2sockfd, &choice_buffer, sizeof(choice_buffer), 0);

            if (bytes_recvd == -1) {
                error_detected("CHOICE BUFFER not received!");
            }
            else {
                p2_choice = choice_buffer[1];
                p1_choice = choice_buffer[0];
                cout << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< p2_name << " has chosen " << p2_choice << endl << endl << timedisp() <<" "<<"Game No: "<<g_no<<"  "<< p1_name << " will play with "
                     << p1_choice << endl;
                cout << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< "Lets Play!" << endl << endl;
            }

            bytes_sent = send(p1sockfd, &choice_buffer, sizeof(choice_buffer), 0);
            if (bytes_sent == -1) {
                error_detected("CHOICE BUFFER could not be sent!");
            }
        } else {
            cout << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< p1_name << " WON the toss." << endl;
            cout << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< p1_name << " is choosing. Please wait..." << endl << endl;

            memset(&choice_buffer, 0, sizeof(choice_buffer));
            bytes_recvd = recv(p1sockfd, &choice_buffer, sizeof(choice_buffer), 0);
            if (bytes_recvd == -1) {
                error_detected("CHOICE BUFFER not received!");
            } else {
                p2_choice = choice_buffer[0];
                p1_choice = choice_buffer[1];
                cout << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< p1_name << " has chosen " << p1_choice << endl << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< p2_name << " will play with "
                     << p2_choice << endl;
                cout << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< "Lets Play!" << endl << endl;
            }
            bytes_sent = send(p2sockfd, &choice_buffer, sizeof(choice_buffer), 0);
            if (bytes_sent == -1) {
                error_detected("CHOICE BUFFER could not be sent!");
            }
        }

        if (p2_choice == 'X') {
            inp = 1;
            cout << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< p2_name << "  will play first." << endl << endl;
        } else {
            inp = 2;
            cout << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< p1_name << " will play first." << endl << endl;
        }

        time_t start, end;

        time(&start);
        initialize();
        cout << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< "Starting Game..." << endl;
        sleep(2);

        while (count < 9) {
            memset(&coord_buffer, 0, sizeof(coord_buffer));
            memset(&alive, 0, sizeof(alive));

            if (inp % 2 != 0) {
                cout << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< p2_name << " 's turn. lease wait...";

                timeout(p2sockfd);
                bytes_recvd = recv(p2sockfd, &coord_buffer, sizeof(coord_buffer), 0);
                if (bytes_recvd <= 0) {
                    cout << endl << timedisp() << " " << "Game No: " << p2_name << " has disconnected";
                    alive = '0';
                    bytes_sent = send(p1sockfd, &alive, sizeof(alive), 0);
                    break;
                }
                alive = '1';
                bytes_sent = send(p1sockfd, &alive, sizeof(alive), 0);

                if (bytes_recvd == -1) {
                    error_detected("CO-ORDINATES BUFFER not recieved!");
                }

                x = coord_buffer[0] - '0';
                y = coord_buffer[1] - '0';
                ni = input(p2_choice, x, y);
                if (ni == 0) {
                    inp++;
                    bytes_sent = send(p1sockfd, &coord_buffer, sizeof(coord_buffer), 0);
                    if (bytes_sent == -1) {
                        error_detected("CO-ORDINATES BUFFER not sent!");

                    }
                    cout << endl << timedisp() << " " << "Game No: " << g_no << "  " << p2_name
                         << " has played. Updating Matrix..." << endl;
                }
            }



             else {
                cout << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< p1_name << "'s turn. Please wait..." << endl;

                timeout(p1sockfd);
                bytes_recvd = recv(p1sockfd, &coord_buffer, sizeof(coord_buffer), 0);
                if (bytes_recvd <= 0) {
                    cout << endl << timedisp() << " " << "Game No: " << p1_name << " has disconnected";
                    alive = '0';
                    bytes_sent = send(p2sockfd, &alive, sizeof(alive), 0);
                    break;
                }
                alive = '1';
                bytes_sent = send(p2sockfd, &alive, sizeof(alive), 0);
                if (bytes_recvd == -1) {
                    error_detected("CO-ORDINATES BUFFER not recieved!");

                }
                x = coord_buffer[0] - '0';
                y = coord_buffer[1] - '0';
                ni = input(p1_choice, x, y);
                if (ni == 0) {
                    inp++;
                    bytes_sent = send(p2sockfd, &coord_buffer, sizeof(coord_buffer), 0);
                    if (bytes_sent == -1) {
                        error_detected("CO-ORDINATES BUFFER not sent!");

                    }
                    cout << endl << timedisp() << " " << "Game No: " << g_no << "  " << p1_name
                         << " has played. Updating Matrix..." << endl;
                }

            }

            count++;
            sleep(2);

            if (count >= 5) {
                
                if (check_gamestatus() == 'd')
                    continue;
                else if (p2_choice == check_gamestatus()) {
                    cout << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< p2_name << " has won!!!" << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< p1_name << " lost." << endl;
                    break;
                } else if (p1_choice == check_gamestatus()) {
                    cout << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< p2_name << " lost." << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< p1_name << " has won." << endl;
                    break;
                }
            }
        }

        if (check_gamestatus() == 'd')
            cout << endl << timedisp() <<" "<<"Game No: "<< g_no <<"  "<< "Game ends in a draw." << endl;
        time(&end);

        double time_taken = double(end - start);
        printf("Game took %f seconds to find a winner/finish \n", time_taken);

        char p1_resp,p2_resp;
        bytes_recvd = recv(p1sockfd, &p1_resp, sizeof(p1_resp), 0);
        if (bytes_recvd == -1) {
            error_detected("CHOICE not recieved!");
            
        }
        bytes_recvd = recv(p2sockfd, &p2_resp, sizeof(p2_resp), 0);
        if (bytes_recvd == -1) {
            error_detected("CHOICE not recieved!");
            
        }
        if(p1_resp=='0')
        {
            bytes_sent = send(p2sockfd,&p1_resp, sizeof(p1_resp),0);
            cout<< timedisp() <<" "<<"Game No: "<< g_no <<"  "<<"There will be no replay"<<endl;
            break;
        }
        else
            bytes_sent = send(p2sockfd,&p1_resp, sizeof(p1_resp),0);

        if(p2_resp=='0')
        {
            bytes_sent = send(p1sockfd,&p2_resp, sizeof(p2_resp),0);
            cout<< timedisp() <<" "<<"Game No: "<< g_no <<"  "<<"There will be no replay"<<endl;
            break;
        }
        else
            bytes_sent = send(p1sockfd,&p2_resp, sizeof(p2_resp),0);
        cout<< timedisp() <<" "<<"Game No: "<< g_no <<"  "<<"The game will be replayed"<<endl;

    }
    cout<<endl<< timedisp() <<" "<<"Game No: "<< g_no <<"  "<<"Thank You for playing Tic-tac-Toe"<<endl;

    close(p1sockfd);
    close(p2sockfd);

    return EXIT_SUCCESS;
}

