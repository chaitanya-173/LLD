#include <iostream>
#include <vector>
using namespace std;

enum CellType { EMPTY, CROSS, CIRCLE };
enum GameStatus { IN_PROGRESS, WIN, DRAW };

class Player {
private:
    int id;
    string name;
    CellType symbol;

public:
    Player(int id, string name, CellType type) : id(id), name(name), symbol(type) {}

    int getId() { return id; }
    string getName() { return name; }
    CellType getSymbol() { return symbol; }
};

class Cell {
private:
    CellType symbol;

public:
    Cell() : symbol(EMPTY) {}

    bool isEmpty() {
        return symbol == EMPTY;
    }

    void setType(CellType symbol) { this->symbol = symbol; }
    CellType getType() { return symbol; }
};  

class Board {
private:
    int rows, cols, filledCells;
    vector<vector<Cell>> cells;

public:
    Board(int rows, int cols) : rows(rows), cols(cols), filledCells(0) {
        cells.resize(rows, vector<Cell>(cols));
    }

    bool isValidCell(int row, int col) { return row >= 0 && row < rows && col >= 0 && col < cols; }

    bool placeMove(int row, int col, CellType symbol) {
        if (!isValidCell(row, col) || !cells[row][col].isEmpty()) return false;

        cells[row][col].setType(symbol);
        filledCells++;

        return true;
    }

    CellType getCellType(int row, int col) { 
        return cells[row][col].getType(); 
    }

    bool isFull() { 
        return filledCells == rows * cols; 
    }

    int getRows() { return rows; }
    int getCols() { return cols; }

    void printBoard() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                CellType type = cells[i][j].getType();

                if (type == EMPTY) cout << '.';
                else if (type == CROSS) cout << 'X';
                else cout << 'O';

                if (j != cols - 1) cout << " | ";
            }
            cout << '\n';
        }
        cout << '\n';
    }
};

class WinningStrategy {
public:
    virtual bool checkWinner(Board& board, CellType symbol) = 0;
    virtual ~WinningStrategy() {}
};

class ConnectKWinningStrategy : public WinningStrategy {
private:
    int k;

public:
    ConnectKWinningStrategy(int k) : k(k) {}

    bool checkWinner(Board& board, CellType symbol) override {
        int rows = board.getRows(), cols = board.getCols();

        vector<int> dr = {-1, -1, -1, 0, 0, 1, 1, 1};
        vector<int> dc = {-1, 0, 1, -1, 1, -1, 0, 1};

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {

                if (board.getCellType(r, c) != symbol) continue;

                for (int d = 0; d < 8; d++) {
                    int nr = r + dr[d];
                    int nc = c + dc[d];
                    int count = 1;

                    while (nr >= 0 && nr < rows && nc >= 0 && nc < cols && board.getCellType(nr, nc) == symbol) {
                        count++;

                        if (count >= k) return true;

                        nr += dr[d];
                        nc += dc[d];
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

    void addPlayer(Player player) {
        players.push_back(player);
    }

    void startGame(Player player) {
        for (int i = 0; i < players.size(); i++) {
            if (players[i].getId() == player.getId()) {
                currentPlayerIndex = i;
                return;
            }
        }
    }

    void printCurrentPlayer() {
        cout << players[currentPlayerIndex].getName() << "'s turn" << endl;
    }

    GameStatus makeMove(int row, int col) {
        if (status != IN_PROGRESS || currentPlayerIndex == -1) return status;

        Player& player = players[currentPlayerIndex];

        if (!board.placeMove(row, col, player.getSymbol())) {
            cout << "Invalid move. Try again." << endl;
            return status;
        }

        board.printBoard();

        if (winningStrategy->checkWinner(board, player.getSymbol())) {
            status = WIN;
            cout << player.getName() << " wins!" << endl;
            return status;
        }

        if (board.isFull()) {
            status = DRAW;
            cout << "Game is a draw!" << endl;
            return status;
        }

        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();

        return status;
    }
};

int main() {
    ConnectKWinningStrategy strategy(3);
    Game game(3, 3, &strategy);

    Player p1(1, "Player 1", CROSS);
    Player p2(2, "Player 2", CIRCLE);

    game.addPlayer(p1);
    game.addPlayer(p2);

    game.startGame(p1);

    int row, col;

    while (true) {
        game.printCurrentPlayer();

        cout << "Enter row and column: ";
        cin >> row >> col;

        GameStatus status = game.makeMove(row, col);

        if (status == WIN || status == DRAW) break;
    }

    return 0;
}

// int main() {
//     ConnectKWinningStrategy strategy(3);

//     Game game(3, 3, &strategy);

//     Player p1(1, "Player 1", CROSS);
//     Player p2(2, "Player 2", CIRCLE);

//     game.addPlayer(p1);
//     game.addPlayer(p2);

//     game.startGame(p1);

//     game.makeMove(0, 0);
//     game.makeMove(1, 0);
//     game.makeMove(0, 1);
//     game.makeMove(1, 1);
//     game.makeMove(0, 2);

//     return 0;
// }