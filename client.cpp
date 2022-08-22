#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "tictactoe.h"
#define  PORT 9987

using namespace std;

int main(int argc, char *argv[])
{
    int sockfd, connectfd, bytes_sent, bytes_recvd;
    char cbuffer[512], opp_name[64], cname[64];
    char *ptr = &cbuffer[0];
    struct sockaddr_in serv_addr;
    struct hostent *he;

    int count = 0, inp, x, y, ni, inp_true = 0, toss;
    char opp_choice, cli_choice;
    char choice_buffer[2], coord_buffer[2], toss_buffer,p_no, g_size;
    char alive;
    
    he = gethostbyname(argv[1]);
    if (he == NULL)
    {
        perror("Sorry! No Such Host!");
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("Sorry! Socket could not be created!");
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr = *((struct in_addr *)he->h_addr);

    connectfd = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (connectfd == -1)
    {
        perror("Sorry! Could not connect to server.");
        return 1;
    }

    cout<<"Enter your name : ";
    cin>>cname;
    cout<<"Waiting for other players..."<<endl;
    do
    {
        static int flag = 0;
        bytes_sent = send(sockfd, &cname, sizeof(cname), 0);
        if (bytes_sent == -1 && flag == 0)
        {
            cout<<"PLAYER DATA NOT SENT!"<<endl<<"Trying Again...";
            continue;
        }
        else
        {		cli_choice = 'X';

            flag = 1;
            memset(&opp_name, 0, sizeof(opp_name));
            bytes_recvd = recv(sockfd, &g_size, sizeof(g_size), 0);
            if (bytes_recvd == -1)
            cout << "COULD NOT ACQUIRE INFORMATION!" << endl << "Trying Again..." << endl;
            if(g_size='2') {
                memset(&opp_name, 0, sizeof(opp_name));
                bytes_recvd = recv(sockfd, &opp_name, sizeof(opp_name), 0);
                if (bytes_recvd == -1)
                    cout << "COULD NOT ACQUIRE PLAYER INFORMATION!" << endl << "Trying Again..." << endl;
                else
                    cout << "You have joined " << opp_name << " for a game of Tic-Tac-Toe." << endl;
            }
        }
    }while(bytes_sent == -1 || bytes_recvd == -1);

    if(g_size=='1')
    {
        bytes_recvd=recv(sockfd, &opp_name, sizeof(opp_name), 0);
        if (bytes_recvd == -1)
            cout << "COULD NOT ACQUIRE INFORMATION!" << endl << "Trying Again..." << endl;
        else
            cout<<opp_name<<" has joined the game"<< endl;
    }

    bytes_recvd= recv(sockfd,&p_no, sizeof(p_no),0);
    if (bytes_recvd == -1)
        cout << "COULD NOT ACQUIRE INFORMATION!" << endl << "Trying Again..." << endl;

    char g_rep='1';

    while(g_rep=='1') {
        cout << "Creating game. Please wait..." << endl;
        sleep(2);
        cout << endl << "Game created!" << endl << endl << "Doing a toss...";

        bytes_recvd = recv(sockfd, &toss_buffer, sizeof(toss_buffer), 0);

        if (bytes_recvd == -1) {
            perror("TOSS BUFFER not received");
            return 1;
        }

        if (toss_buffer != p_no) {
            cout << endl << opp_name << " WON the toss." << endl;
            cout << opp_name << " is choosing. Please wait..." << endl << endl;
            memset(&choice_buffer, 0, sizeof(choice_buffer));
            bytes_recvd = recv(sockfd, &choice_buffer, sizeof(choice_buffer), 0);
            if (bytes_recvd == -1) {
                perror("CHOICE BUFFER not received!");
                return 1;
            } else {
                opp_choice = choice_buffer[1];
                cli_choice = choice_buffer[0];
                cout << opp_name << " has chosen " << opp_choice << endl << endl << "You will play with " << cli_choice
                     << endl;
                cout << endl << "Lets Play!" << endl << endl;
            }
        } else {
            cout << endl << "You won the TOSS!" << endl;
            do {
                cout << cname << " Enter Your Choice (X or O): ";
                cin >> cli_choice;
                if (cli_choice == 'X' || cli_choice == 'x') {
                    opp_choice = 'O';
                    cli_choice = 'X';
                    inp_true = 1;
                    cout << endl << opp_name << " gets O." << endl << endl << "Lets Play!" << endl << endl;
                } else if (cli_choice == 'O' || cli_choice == 'o' || cli_choice == '0') {
                    opp_choice = 'X';
                    cli_choice = 'O';
                    inp_true = 1;
                    cout << endl << opp_name << " gets X." << endl << endl << "Lets Play!" << endl << endl;
                } else {
                    cout << "\nInvalid Choice! Please Choose Again..." << endl;
                    inp_true == 0;
                }
            } while (inp_true == 0);

            memset(&choice_buffer, 0, sizeof(choice_buffer));
            choice_buffer[0] = opp_choice;
            choice_buffer[1] = cli_choice;

            bytes_sent = send(sockfd, &choice_buffer, sizeof(choice_buffer), 0);
            if (bytes_sent == -1) {
                perror("CHOICE BUFFER could not be sent.");
                return 1;
            }

        }

        if (opp_choice == 'X') {
            inp = 1;
            cout << opp_name << " will play first." << endl << endl;

        } else {
            inp = 2;
            cout << "You will play first." << endl << endl;
        }


        initialize();
        cout << endl << "Starting Game..." << endl;
        sleep(3);
        display();

        while (count < 9) {
            memset(&coord_buffer, 0, sizeof(coord_buffer));
            memset(&alive, 0, sizeof(alive));

            if (inp % 2 != 0) {

                cout << endl << opp_name << "'s turn. Please wait..." << endl;

                bytes_recvd = recv(sockfd, &alive, sizeof(alive), 0);
                if(alive != '1')
                {
                    cout<<opp_name << " has disconnected... "<< endl << " Ending game ..." << endl;
                    break;
                }

                bytes_recvd = recv(sockfd, &coord_buffer, sizeof(coord_buffer), 0);
                if (bytes_recvd == -1) {
                    perror("CO-ORDINATES BUFFER not recieved!");
                    return 1;
                }
                x = coord_buffer[0] - '0';
                y = coord_buffer[1] - '0';

                ni = input(opp_choice, x, y);
                if (ni == 0) {
                    inp++;
                    cout << endl << opp_name << " has played. Updating Matrix..." << endl;
                }
            } else {
                cout << endl << "Your turn. Enter co-ordinates separated by a space : ";
                cin >> x >> y;

                ni = input(cli_choice, x, y);
                if (ni == 0) {
                    inp++;
                    sprintf(&coord_buffer[0], "%d", x);
                    sprintf(&coord_buffer[1], "%d", y);
                    cout << endl << "Updating Matrix..." << endl;

                    bytes_sent = send(sockfd, &coord_buffer, sizeof(coord_buffer), 0);
                    if (bytes_sent == -1) {
                        perror("CO-ORDINATES BUFFER could not be sent!");
                        return 1;
                    }
                }
            }

            count++;
            sleep(2);
            display();

            if (count >= 5) {

                if (check_gamestatus() == 'd')
                    continue;
                else if (opp_choice == check_gamestatus()) {
                    cout << endl << "You loose." << endl << opp_name << " has won." << endl;
                    break;
                } else if (cli_choice == check_gamestatus()) {
                    cout << endl << "Congrats! You have won!!!" << endl;
                    break;
                }
            }

        }

        if (check_gamestatus() == 'd')
            cout << endl << "Game ends in a draw." << endl;

        char g_resp;
        cout<<"Do you want to replay!"<<endl<<"Type in 1 for YES and 0 for NO"<<endl;
        cin>>g_resp;
        bytes_sent= send(sockfd,&g_resp,sizeof(g_size),0);
        if(bytes_sent==-1)
        {
            perror("DATA not sent!");
            return 1;
        }
        if(g_resp == '0')
            break;
        cout << "Waiting for response from server..." << endl;

        bytes_recvd = recv(sockfd,&g_resp,sizeof(g_resp),0);
        if(bytes_recvd==-1)
        {
            perror("DATA not sent!");
            return 1;
        }
        if(g_resp=='0')
        {
            cout<<opp_name<<" has decided to quit"<<endl;
            break;
        }
        else
            cout<<"The game will be replayed!"<<endl;
    }

    cout<<endl<<"Thank You for playing Tic-tac-Toe"<<endl;

    close(sockfd);
    return 0;
}