#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

char m[3][3], p1, p2;
int initialize();
void display();
int input(char, int, int);
char check_gamestatus();

// Initializing the game matrix
int initialize()
{
    int i, j;
    for (i=0; i<3; i++)
        for (j=0; j<3; j++)
            m[i][j] = '-';
    return  EXIT_SUCCESS;
}

// Display the Grid co-ordinates and the current matrix status
void display()
{

    int i, j;
    cout<<"\nGrid Co-ordinates : "<<endl<<endl;

    for (i=0; i<3; i++)
    {
        cout<<"\t";
        for (j=0; j<3; j++)
            cout<<i<<" "<<j<<"    ";
        cout<<endl;
    }

    cout<<"\nThe Tic-Tac-Toe board : "<<endl<<endl;
    for (i=0; i<3; i++)
    {
        cout<<"\t";
        for (j=0; j<3; j++)
            cout<<" "<<m[i][j]<<"     ";
        cout<<endl;
    }
}

// checking the input received and update the matrix
int input(char choice, int x, int y)
{
    if (x >= 0 && x <= 2 && y >= 0 && y<= 2)
        if (m[x][y] == '-')
        {
            m[x][y] = choice;
            return 0;
        }
        else
        {
            cout<<endl<<"Co-ordinates "<<x<<" "<<y<<" are already used!"<<endl<<"Press ENTER to continue..."<<endl;
            do{}
            while(cin.get() != '\n');
            return 1;
        }
    else
    {
        cout<<"\nInvalid Co-ordinates!"<<endl<<"Press ENTER to continue..."<<endl;
        do{}
        while(cin.get() != '\n');
        return 1;
    }

}

// Check the current game status
char check_gamestatus()
{
    int i, j;
    char temp;

    for (j=0; j<3; j++)
    {
        if (m[0][j] != '-')
        {
            temp = m[0][j];
            if (j == 0)
            {
                if (m[0][1] == temp)
                    if (m[0][2] == temp)
                        return temp;
                if (m[1][0] == temp)
                    if (m[2][0] == temp)
                        return temp;
                if (m[1][1] == temp)
                    if (m[2][2] == temp)
                        return temp;
            }
            else if (j == 1)
            {
                if (m[1][1] == temp)
                    if (m[2][1] == temp)
                        return temp;
            }
            else
            {
                if (m[1][2] == temp)
                    if (m[2][2] == temp)
                        return temp;
            }
        }
    }
    for (i=1; i<3; i++)
    {
        if (m[i][0] != '-')
        {
            temp = m[i][0];
            if (i == 1)
            {
                if (m[1][1] == temp)
                    if (m[1][2] == temp)
                        return temp;
            }
            else
            {
                if (m[2][1] == temp)
                    if (m[2][2] == temp)
                        return temp;
                if (m[1][1] == temp)
                    if (m[0][2] == temp)
                        return temp;
            }
        }
    }

    return 'd';

}
