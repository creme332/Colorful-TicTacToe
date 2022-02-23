#include <iostream>
#include <vector>
#include <climits>
#include <stdlib.h> //used to clear terminal screen each round
using namespace std;

const char null = '.'; const char x = 'X'; const char o = 'O';
const char ongoing = 'n'; //game state = ongoing if no winner yet and board not full
int winningdirection=-1, winningstart=-1; // used by tracewin() to trace winning path
vector<vector<char>> board = {
    {null,null,null},
    {null,null,null},
    {null,null,null},
};
bool IsValid(int row, int col) { //validate user input
    if (row > 0 && row < 4 && col>0 && col < 4 && board[row - 1][col - 1] == null)return 1;
    return 0;
}
void OutputGridBoard() { //Alternative theme
    system("CLS"); //clear previous board from terminal
    cout << "Use format <row><space><column> where row, column are 1-3" << endl;
    cout << "_____________" << endl;
    for (int row = 0;row < 3;row++) {
        for (int col = 0;col < 3;col++) {
            if (col == 2) {
                cout << "| " << board[row][col] << " |";
            }
            else {
                cout << "| " << board[row][col] << " ";
            }
        }
        cout << endl;
        if (row == 2) {
            cout << "|___|___|___|" << endl;

        }
        else {
            cout << "|---|---|---|" << endl;
        }
    }
};
void ColoredOutput() {
    system("CLS"); 
    for (int i = 0;i < 14;i++) {
        std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";
    }
    std::cout << " " << "\033[48;5;34m\033[38;5;232m \033[0m\n";

       for (int row = 0;row < 3;row++) {
           //2 green blocks at start
           std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";
           std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";

           for (int col = 0;col < 3;col++) {
               if (board[row][col] == null) {
                   std::cout << "\x1B[97m.\033";
               }
               if (board[row][col] == x) {
                   std::cout << "\x1B[88mX\033";
               }
               if (board[row][col] == o) {
                   std::cout << "\x1B[88mO\033";
               }

               if (col == 2) {//2 green blocks at end of row
                   std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";
                   std::cout << " " << "\033[48;5;34m\033[38;5;232m \033[0m";
               }
               else {//4 green blocks between each character
                   std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";
                   std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";
                   std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";
                   std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";
               }


        }
        cout << "\n";
        for (int i = 0;i < 14;i++) {
            std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";
        }
        std::cout << " " << "\033[48;5;34m\033[38;5;232m \033[0m\n";
    }
}
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
void tracewin() {
    system("CLS");
    //first green row
    for (int i = 0;i < 14;i++) {std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";}
    std::cout << " " << "\033[48;5;34m\033[38;5;232m \033[0m\n";

    for (int row = 0;row < 3;row++) {
        //2 green blocks at start of each row
        std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";
        std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";

        for (int col = 0;col < 3;col++) {
            
            if ((winningdirection == 0 && winningstart == row)||
                (winningdirection == 1 && winningstart == col)||
                (winningdirection == 2 && row == col) ||
                (winningdirection == 3 && row + col == 2)
                ){ //if there's a winner, winning play should be red
                if (board[row][col] == x) {std::cout << "\x1B[31mX\033";} //red X
                if (board[row][col] == o) {std::cout << "\x1B[31mO\033";} //red O
            }
            else { //no color change
                if (board[row][col] == null) {std::cout << "\x1B[97m.\033";} //white dot
                if (board[row][col] == x) { std::cout << "\x1B[88mX\033"; } //black X
                if (board[row][col] == o) { std::cout << "\x1B[88mO\033"; } //black O
            }

            if (col == 2) {//add 2 green blocks at end of each row
                std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";
                std::cout << " " << "\033[48;5;34m\033[38;5;232m \033[0m";
            }
            else {//add 4 green blocks between each x,o,null
                std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";
                std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";
                std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";
                std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";
            }


        }
        cout << "\n";
        for (int i = 0;i < 14;i++) { //add 1 green row
            std::cout << " " << "\033[48;5;34m\033[38;5;232m \033";
        }
        std::cout << " " << "\033[48;5;34m\033[38;5;232m \033[0m\n";
    }

}
void UpdateBoard(bool turn, int row, int col) {
    //coordinates have already been validated
    // row n has index n-1 and column k has index k-1
    board[row - 1][col - 1] = (turn == 0) ? o : x;
}

int minimax(int depth, bool maximizingplayer,int alpha, int beta) {
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

void AImove() {
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
}

int main()
{
    bool vsAI = 1;
    bool turnX = 1; // 0 = O first ;  1 = X first
    char state = ongoing;
    int row, col;
    cout << "1 player mode? yes->1  No->0 ";
    cin >> vsAI;

    cout << "Who starts?  X->1 O->0 ";
    cin >> turnX;

    if (vsAI) { //1 player mode
        while (state == ongoing) {
           ColoredOutput();
            if (turnX == 0) {
                cout << "Your turn \n" << endl;
                cin >> row >> col;

                while (IsValid(row, col) == 0) {
                    cout << "Invalid coordinates. Enter again. " << endl;
                    cin >> row >> col;
                }
                UpdateBoard(0, row, col);

            }
            else {
                AImove();
            }
            state = GameState();
            turnX = !(turnX); 
        }

    }
    else { //2 player mode
        while (state == ongoing) {
            ColoredOutput();
            cout << ((turnX == 0) ? "O's turn" : "X's turn") << endl;
            cin >> row >> col;

            while (IsValid(row, col) == 0) {
                cout << "Invalid coordinates. Enter again. \n" << endl;
                cin >> row >> col;
            }
            UpdateBoard(turnX, row, col);
            state = GameState();
            turnX = !(turnX); //swap turns 
        }
    }

    if (GameState() != 'd') {
        tracewin();
        if (state == 0) {
            cout << "Player O has won!\n";
        }
        else {
            cout << "Player X has won!\n";
        }
    }
    else {
        ColoredOutput();
        cout << "Draw !\n";
    }
}
