#include <fstream>
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////
// IO funkcijos
void get_board_from_file(vector<int>[9][9], char[]);
void print_board_to_file(vector<int>[9][9], char[]);
void print_error(vector<int>[9][9], char[], int, int);

// Funkcijos sprendziancios sudoku
void initialize_board(vector<int>[9][9]);
void logic_solve(vector<int>[9][9]);
bool backtrack(vector<int>[9][9]);
bool check_if_doable(vector<int>[9][9], int&, int&);

// Pagalbines funkcijos, dazniausiai dirbancios su konkreciais langeliais
bool is_number_available_at(vector<int>[9][9], int rows, int cols, int);
void write_number_at(vector<int>[9][9], int rows, int cols, int);
int unknown_squares(vector<int>[9][9]);
///////////////////////////////////////////////////////////////////////////////////////

int main(){
    vector<int> board[9][9];
    get_board_from_file(board, "sudokuIn.txt");             // Input

    initialize_board(board);                                // Sudoku tvarkomas, atmetami kai kurie
    logic_solve(board);                                     // logiskai netinkantys variantai

    int rows, cols;
    if (!check_if_doable(board, rows, cols))                // Jeigu paduoti duomenys akivaizdziai neteisingi isvedamas erroras
        print_error(board, "sudokuOut.txt", rows, cols);
    else if (backtrack(board))                              // Kitu atveju naudojamas brute force ir gautas atsakymas isvedamas
            print_board_to_file(board, "sudokuOut.txt");

    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////
// IO funkcijos
// Nuskaito pradinius duotuosius sudoku skaiciukus
void get_board_from_file(vector<int> board[9][9], char filename[]){
    ifstream sudokuIn;
    sudokuIn.open(filename);

    int tempval;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++){
            sudokuIn >> tempval;
            board[i][j].push_back(tempval);
        }

    sudokuIn.close();
}

// Isveda Sudoku i faila, jei pradiniai duomenys yra teisingi
void print_board_to_file(vector<int> board[9][9], char filename[]){
    ofstream sudokuOut;
    sudokuOut.open(filename);

    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++)
            sudokuOut << board[i][j][0] << " " ;
        sudokuOut << endl;
    }

    sudokuOut.close();
}

// Jei sudoku yra klaidingas tam tikrame langelyje, tai isvedama klaida ir langelio koordinates
void print_error(vector<int> board[9][9], char filename[], int i, int j){
    ofstream sudokuOut;
    sudokuOut.open(filename);
    sudokuOut << "Klaida " << i+1 << ", " << j+1 << " langelyje";
    sudokuOut.close();
}
///////////////////////////////////////////////////////////////////////////////////////
// Funkcijos sprendziancios sudoku
// Suranda visas galimas reiksmes tusciuose langeliuose
void initialize_board(vector<int> board[9][9]){
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if(board[i][j][0] == 0)
                for(int k = 1; k < 10; k++)
                    if(is_number_available_at(board, i, j, k))
                        board[i][j].push_back(k);
}

// Isprendzia visus langelius, kuriuose galima tik viena reiksme
void logic_solve(vector<int> board[9][9]){
    int unsolved;
    do{
        unsolved = unknown_squares(board);
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                if(board[i][j].size() == 2)
                    write_number_at(board, i, j, board[i][j][1]);
    } while(unsolved != unknown_squares(board));
}

// Rekursine funkcija, kuri ima logiskas reiksmes ir backtrackina
bool backtrack(vector<int> board[9][9]){
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if(board[i][j][0] == 0){
                for(int k = 1; k < board[i][j].size(); k++){
                    if(is_number_available_at(board, i, j, board[i][j][k])){
                        board[i][j][0] = board[i][j][k];
                        board[i][j][k] = 0;
                        if(backtrack(board)){
                            return true;
                        } else {
                            board[i][j][k] = board[i][j][0];
                            board[i][j][0] = 0;
                        }
                    }
                }
                return false;
            }
    return true;
}

// Patikrina ar lenta nera akivaizdziai neissprendziama
bool check_if_doable(vector<int> board[9][9], int& rows, int& cols){
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if(board[i][j][0] != 0){
                if(!is_number_available_at(board, i, j, board[i][j][0])){
                    rows = i;
                    cols = j;
                    return false;
                }
            } else if(board[i][j].size() == 1){
                rows = i;
                cols = j;
                return false;
            }

    return true;
}
///////////////////////////////////////////////////////////////////////////////////////
// Pagalbines funkcijos, dazniausiai dirbancios su konkreciais langeliais
// Suskaiciuoja kiek boarde yra nuliuku
int unknown_squares(vector<int> board[9][9]){
    int counter = 0;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if(board[i][j][0] == 0)
                counter++;
    return counter;
}

// I langeli iraso skaiciu ir pasalina reiksmes, kurios kertasi su siuo skaiciu eiluteje, stulpelyje ir devynete
void write_number_at(vector<int> board[9][9], int rows, int cols, int number){
    board[rows][cols].clear();
    board[rows][cols].push_back(number);
    for(int i = 0; i < 9; i++)
        if(board[i][cols][0] == 0)
            for(int k = board[i][cols].size() - 1; k > 0; k--)
                if(board[i][cols][k] == number)
                    board[i][cols].erase(board[i][cols].begin()+k);
    for(int i = 0; i < 9; i++)
        if(board[rows][i][0] == 0)
            for(int k = board[rows][i].size() - 1; k > 0; k--)
                if(board[rows][i][k] == number)
                    board[rows][i].erase(board[rows][i].begin()+k);
    rows = (rows / 3) * 3;
    cols = (cols / 3) * 3;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(board[rows+i][cols+j][0] == 0)
                for(int k = board[rows+i][cols+j].size() - 1; k > 0; k--)
                    if(board[rows+i][cols+j][k] == number)
                        board[rows+i][cols+j].erase(board[rows+i][cols+j].begin()+k);
}

// Patikrina ar reiksme galima konkreciame langelyje
bool is_number_available_at(vector<int> board[9][9], int rows, int cols, int number){
    for(int i = 0; i < 9; i++)
        if(board[i][cols][0] == number && i != rows)
            return false;

    for(int i = 0; i < 9; i++)
        if(board[rows][i][0] == number && i != cols)
            return false;

    int top_row_pt = (rows / 3) * 3;
    int top_col_pt = (cols / 3) * 3;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(board[top_row_pt+i][top_col_pt+j][0] == number)
                if(top_row_pt+i != rows && top_col_pt+j != cols)
                    return false;

    return true;
}
