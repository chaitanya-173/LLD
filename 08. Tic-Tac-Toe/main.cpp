#include <iostream>
#include <vector>
using namespace std;

enum CellState { EMPTY, OCCUPIED };
enum GameStatus { IN_PROGRESS, WIN, DRAW };

class Player {
public:
    int id;
    string name;
    char symbol;

    Player(int id, string name, char symbol) : id(id), name(name), symbol(symbol) {}
};

class Cell {
public:
    int row, col;
    CellState state;
    char symbol;

    Cell(int row, int col) : row(row), col(col), state(EMPTY), symbol('\0') {}

    bool isEmpty() { return state == EMPTY; }
    void setSymbol(char symbol) { this->symbol = symbol; state = OCCUPIED; }
};

class Board {
private:
    int rows, cols, filledCells;
    vector<vector<Cell>> cells;

public:
    Board(int rows, int cols) : rows(rows), cols(cols), filledCells(0) {
        for (int i = 0; i < rows; i++) {
            vector<Cell> row;
            for (int j = 0; j < cols; j++) row.push_back(Cell(i, j));
            cells.push_back(row);
        }
    }

    bool isValidCell(int row, int col) {
        return row >= 0 && row < rows && col >= 0 && col < cols;
    }

    bool placeMove(int row, int col, char symbol) {
        if (!isValidCell(row, col) || !cells[row][col].isEmpty()) return false;
        cells[row][col].setSymbol(symbol);
        filledCells++;
        return true;
    }

    bool isFull() { return filledCells == rows * cols; }

    char getSymbol(int row, int col) { return cells[row][col].symbol; }
    int getRows() { return rows; }
    int getCols() { return cols; }

    void printBoard() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                char symbol = cells[i][j].symbol;
                cout << (symbol == '\0' ? '.' : symbol);
                if (j != cols - 1) cout << " | ";
            }
            cout << '\n';
        }
        cout << '\n';
    }
};

class WinningStrategy {
public:
    virtual bool checkWinner(Board& board, char symbol) = 0;
    virtual ~WinningStrategy() {}
};

class ConnectKWinningStrategy : public WinningStrategy {
private:
    int k;

public:
    ConnectKWinningStrategy(int k) : k(k) {}

    bool checkWinner(Board& board, char symbol) override {
        int rows = board.getRows(), cols = board.getCols();

        int directions[4][2] = {{0,1}, {1,0}, {1,1}, {1,-1}};

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (board.getSymbol(r, c) != symbol) continue;

                for (auto& dir : directions) {
                    int count = 1;
                    int nr = r + dir[0], nc = c + dir[1];

                    while (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                           board.getSymbol(nr, nc) == symbol) {
                        count++;
                        if (count >= k) return true;
                        nr += dir[0];
                        nc += dir[1];
                    }
                }
            }
        }

        return false;
    }
};

class Game {
private:
    Board board;
    vector<Player> players;
    int currentPlayerIndex;
    WinningStrategy* winningStrategy;
    GameStatus status;

public:
    Game(int rows, int cols, WinningStrategy* winningStrategy)
        : board(rows, cols), currentPlayerIndex(-1),
          winningStrategy(winningStrategy), status(IN_PROGRESS) {}

    void addPlayer(Player player) { players.push_back(player); }

    void startGame(Player player) {
        for (int i = 0; i < players.size(); i++) {
            if (players[i].id == player.id) {
                currentPlayerIndex = i;
                return;
            }
        }
    }

    GameStatus makeMove(int row, int col) {
        if (status != IN_PROGRESS || currentPlayerIndex == -1) return status;

        Player& player = players[currentPlayerIndex];

        if (!board.placeMove(row, col, player.symbol)) {
            cout << "Invalid move. Try again.\n";
            return status;
        }

        board.printBoard();

        if (winningStrategy->checkWinner(board, player.symbol)) {
            status = WIN;
            cout << player.name << " wins!\n";
            return status;
        }

        if (board.isFull()) {
            status = DRAW;
            cout << "Game is a draw!\n";
            return status;
        }

        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
        return status;
    }
};

int main() {
    ConnectKWinningStrategy strategy(3);

    Game game(3, 3, &strategy);

    Player p1(1, "Player 1", 'X');
    Player p2(2, "Player 2", 'O');

    game.addPlayer(p1);
    game.addPlayer(p2);

    game.startGame(p1);

    game.makeMove(0, 0);
    game.makeMove(1, 0);
    game.makeMove(0, 1);
    game.makeMove(1, 1);
    game.makeMove(0, 2);

    return 0;
}