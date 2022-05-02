#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <stdlib.h> //used to clear terminal screen each round
#include <Windows.h>
#include <conio.h> 

const char null = '.'; const char x = 'X'; const char o = 'O';
const char ongoing = 'n'; //game state = ongoing if no winner yet and board not full
int winningdirection = -1, winningstart = -1; // used by tracewin() to trace winning path
std::vector<std::vector<char>> board = {
    {null,null,null},
    {null,null,null},
    {null,null,null},
};

void hidecursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
void setCursorPosition(const int row, const int col) {
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)col, (SHORT)row };
    SetConsoleCursorPosition(hOut, coord);
}
bool IsValid(int row, int col) { //validate user input
    if (row > 0 && row < 4 && col>0 && col < 4 && board[row - 1][col - 1] == null)return 1;
    return 0;
}
void OutputGridBoard() { //Alternative BasicTheme
    std::cout << "_____________" << std::endl;
    for (int row = 0;row < 3;row++) {
        for (int col = 0;col < 3;col++) {
            if (col == 2) {
                std::cout << "| " << board[row][col] << " |";
            }
            else {
                std::cout << "| " << board[row][col] << " ";
            }
        }
        std::cout << std::endl;
        if (row == 2) {
            std::cout << "|___|___|___|" << std::endl;

        }
        else {
            std::cout << "|---|---|---|" << std::endl;
        }
    }
};
char GameState() {
    // returns "X" if x won and "O" if o won 
    // returns "n" if there's no winner and board still has empty spots
    // returns "d" if draw 
    char player[2] = { x,o };
    bool HasEmptySpot = 0;

    for (int i = 0;i < 2;i++) {//for each player
        int count[4] = { 0,0,0,0 }; //count along each direction
        for (int r = 0;r < 3;r++) {
            for (int c = 0;c < 3;c++) {
                if (board[r][c] == player[i]) { count[0]++; } //count horizontally
                if (board[c][r] == player[i]) { count[1]++; } //count vertically
                if (r == c && board[r][c] == player[i]) { count[2]++; } //count along - diagonal
                if (r + c == 2 && board[r][c] == player[i]) { count[3]++; }  //count along + diagonal
                if (board[r][c] == null)HasEmptySpot = 1;
            }
            for (int m = 0;m < 4;m++) {//return winner if any
                if (count[m] == 3) {
                    winningdirection = m;
                    winningstart = r;
                    return player[i];
                }
            }
            count[0] = 0;count[1] = 0; //reset only for orthogonal directions
        }
    }
    if (HasEmptySpot)return ongoing;
    return 'd'; // draw
}
void tracewin(bool BasicTheme) {

    for (int row = 0;row < 3;row++) {
        for (int col = 0;col < 3;col++) {

            if ((winningdirection == 0 && winningstart == row) ||
                (winningdirection == 1 && winningstart == col) ||
                (winningdirection == 2 && row == col) ||
                (winningdirection == 3 && row + col == 2)
                ) { //if there's a winner, winning play should be red
                if (!BasicTheme) {
                    setCursorPosition(2 * row + 1, 5 * col + 3);
                    if (board[row][col] == x) { std::cout << "\033[3;42;34mX\033[0m"; } // X green background
                    if (board[row][col] == o) { std::cout << "\033[3;42;34mO\033[0m"; } // O green background
                }
                else {
                    setCursorPosition(2 * row + 1, 4 * col + 2);
                    if (board[row][col] == x) { std::cout << "\x1B[32mX\033[0m"; } //green X black bg
                    if (board[row][col] == o) { std::cout << "\x1B[32mO\033[0m"; } //green O black bg
                }

            }
        }
    }

}
void UpdateBoard(bool BasicTheme, bool turnX, int row, int col) {
    //coordinates have already been validated
    // row n has index n-1 and column k has index k-1
    board[row - 1][col - 1] = (!turnX) ? o : x;
    if (BasicTheme) {
        setCursorPosition(2 * row - 1, 4 * col - 2);
        if (turnX)std::cout << "X";
        else std::cout << "O";
    }
    else {
        setCursorPosition(2 * row - 1, 5 * col - 2);
        if (turnX) std::cout << "\033[42;5;30mX\033[0m";
        else std::cout << "\033[42;5;30mO\033[0m";
    }
}

int minimax(int depth, bool maximizingplayer, int alpha, int beta) {
    char result = GameState();
    //if game over
    if (result == x)return 100;
    if (result == o)return -100;
    if (result == 'd')return 0;

    int MaxEval = -1000; //max score for maximising player
    int MinEval = 1000; //min score for minimising player

    for (int i = 0;i < 3;i++) {
        for (int j = 0;j < 3;j++) {
            if (board[i][j] == null) {
                board[i][j] = maximizingplayer ? x : o;
                if (maximizingplayer) {
                    MaxEval = max(MaxEval, minimax(depth + 1, 0, alpha, beta) - depth);
                    alpha = max(alpha, MaxEval);
                }
                else {
                    MinEval = min(MinEval, minimax(depth + 1, 1, alpha, beta) + depth);
                    beta = min(beta, MinEval);
                }
                board[i][j] = null;
                if (beta <= alpha)break;
            }
        }
    }
    return maximizingplayer == 1 ? MaxEval : MinEval;
}

void AImove(bool BasicTheme) {
    int p = -1, q = -1; //best move is to play at (p,q)
    int MaxEval = INT_MIN;

    for (int i = 0;i < 3;i++) {
        for (int j = 0;j < 3;j++) {
            if (board[i][j] == null) {//for each possible move

                board[i][j] = x;
                int eval = minimax(0, 0, -9999, 9999);
                if (eval > MaxEval) {//new best move discovered
                    p = i; q = j;
                    MaxEval = eval;
                }
                board[i][j] = null;

            }
        }
    }
    board[p][q] = x;
    UpdateBoard(BasicTheme, 1, p+1, q+1);

}

void NewColoredOutput() {
    const std::string GreenBlock = "\033[48;5;34m\033[42;5;232m \033";
    //first line of green blocks
    for (int i = 0;i < 14;i++)std::cout << " " << GreenBlock;
    std::cout << " " << GreenBlock <<"[0m\n";

    for (int row = 0;row < 3;row++) {
        //2 green blocks at start of each row 
        std::cout << " " << GreenBlock;
        std::cout << " " << GreenBlock;

        for (int col = 0;col < 3;col++) {
            if (board[row][col] == null) {
                std::cout << "\x1B[97m.\033";
            }
            if (board[row][col] == x) {
                std::cout << "\x1B[88mX\033"; //black X
            }
            if (board[row][col] == o) {
                std::cout << "\x1B[88mO\033"; //black O
            }

            if (col == 2) {//2 green blocks at end of row
                std::cout << " " << GreenBlock << GreenBlock << "[0m";
            }
            else {//4 green blocks between each character
                for(int i=0;i<4;i++) std::cout << " " << GreenBlock;
            }
        }
        std::cout << "\n";
        for (int i = 0;i < 14;i++) {
            std::cout << " " << GreenBlock;
        }
        std::cout << " " << GreenBlock << "[0m\n";
    }
}

void InitialiseTerminal(bool BasicTheme, bool vsAI, bool turnX) {
    system("cls");
    if (BasicTheme) {
        OutputGridBoard();
    }
    else {
        NewColoredOutput();
    }
    if (!vsAI) {
        setCursorPosition(7, 0);
        std::cout << ((turnX == 0) ? "O's turn" : "X's turn")<<"\n";
    }
}
int main(){
    hidecursor();
    bool vsAI = 0;
    bool BasicTheme = 1;
    bool turnX = 0; // 0 = O first ;  1 = X first
    char state = ongoing;
    int row=0, col=0; //user input

    std::cout << "Choose theme :\n0 : Colored theme\n1 : Basic theme\n";
    std::cin >> BasicTheme;
    std::cout << "Game mode :\n0 : Two-player mode\n1 : AI mode\n";
    std::cin >> vsAI;
    std::cout << "Choose starting player : \n0 : You  \n1 : Opponent\n";
    std::cin >> turnX;

    InitialiseTerminal(BasicTheme,vsAI,turnX);

    while (state == ongoing) {
        if (vsAI) {
            if (turnX) { 
                setCursorPosition(8, 0); std::cout << "      "; //clear previous terminal input
                setCursorPosition(7, 0); std::cout << "AI's turn\n";
                AImove(BasicTheme); 
            }
            else {
                setCursorPosition(8, 0); std::cout << "      "; //clear previous terminal input
                setCursorPosition(7, 0); std::cout << "Your turn\n";
                std::cin >> row >> col;
                while (!IsValid(row, col)) {
                    setCursorPosition(9, 0); std::cout << "Invalid coordinates. Enter again.\n";
                    setCursorPosition(8, 0); std::cout << "      "; //clear previous terminal input
                    setCursorPosition(8, 0);
                    std::cin >> row >> col;
                }
                setCursorPosition(9, 0); std::cout << "                                  ";
                setCursorPosition(10, 0); std::cout << "                                  ";
                UpdateBoard(BasicTheme, turnX, row, col);
            }
        }
        else {
            setCursorPosition(8, 0); std::cout << "      "; //clear previous terminal input
            setCursorPosition(7, 0); std::cout << ((turnX) ? "X" : "O")<<"\n";
            std::cin >> row >> col;
            while (!IsValid(row, col)) {
                setCursorPosition(9, 0); std::cout << "Invalid coordinates. Enter again.\n";
                setCursorPosition(8, 0); std::cout << "      "; //clear previous terminal input
                setCursorPosition(8, 0);
                std::cin >> row >> col;
            }
            setCursorPosition(9, 0); std::cout << "                                  ";
            setCursorPosition(10, 0); std::cout << "                                  ";
            UpdateBoard(BasicTheme, turnX, row, col);
        }
        state = GameState();
        turnX = !(turnX); //swap turns 
    }

    if (BasicTheme) {
        setCursorPosition(7, 0); std::cout << "                                  ";
        setCursorPosition(8, 0); std::cout << "                                  ";
        setCursorPosition(7, 0);
    }
    else {
        setCursorPosition(9, 0); std::cout << "                                  ";
        setCursorPosition(10, 0); std::cout << "                                  ";
        setCursorPosition(9, 0);
    }


    if (GameState() != 'd') {
        tracewin(BasicTheme);
        setCursorPosition(7, 0);
        if (state == 0)std::cout << "Player O has won!\n\n";
        else std::cout << "Player X has won!\n\n";
    }
    else {
        std::cout << "Draw !\n\n";
    }
    system("pause");
    return 0;
}
