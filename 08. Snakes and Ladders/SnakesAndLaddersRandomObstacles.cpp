#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

enum ObstacleType { SNAKE, LADDER };

class Obstacle {
public:
    int src, dest;

    Obstacle(int src, int dest) : src(src), dest(dest) {}

    virtual ObstacleType getObstacleType() = 0;

    virtual int movePlayer() {
        return dest;
    }
};

class Ladder : public Obstacle {
public:
    Ladder(int src, int dest) : Obstacle(src, dest) {}

    ObstacleType getObstacleType() override {
        return LADDER;
    }
};

class Snake : public Obstacle {
public:
    Snake(int src, int dest) : Obstacle(src, dest) {}

    ObstacleType getObstacleType() override {
        return SNAKE;
    }
};

class ObstacleFactory {
public:
    Obstacle* createObstacle(ObstacleType type, int src, int dest) {
        if (type == LADDER) {
            return new Ladder(src, dest);
        } else {
            return new Snake(src, dest);
        }
    }
};

class Cell {
public:
    int position;
    Obstacle* obstacle; // nullptr if no obstacle

    Cell() {
        position = 0;
        obstacle = nullptr;
    }

    Cell(int position) {
        this->position = position;
        obstacle = nullptr;
    }

    bool hasObstacle() {
        return obstacle != nullptr;
    }

    int getFinalPosition() {
        if (hasObstacle()) {
            return obstacle->movePlayer();
        }
        return position;
    }
};

class Player {
public:
    string name;
    int position;

    Player(string name) {
        this->name = name;
        this->position = 0;
    }
};

class Dice {
public:
    int noOfDice;

    Dice(int noOfDice) {
        this->noOfDice = noOfDice;
    }

    int roll() {
        int total = 0;
        for (int i = 0; i < noOfDice; i++) {
            total += (rand() % 6) + 1; // 1 to 6
        }
        return total;
    }
};

class Board {
public:
    int size;
    int sideLength;
    vector<vector<Cell>> grid;

    Board(int sideLength) {
        this->sideLength = sideLength;
        size = sideLength * sideLength;
        grid.resize(sideLength, vector<Cell>(sideLength));

        // number cells 1..size in boustrophedon (zig-zag) order, bottom to top
        int pos = 1;
        for (int row = sideLength - 1; row >= 0; row--) {
            if ((sideLength - 1 - row) % 2 == 0) {
                for (int col = 0; col < sideLength; col++) {
                    grid[row][col] = Cell(pos++);
                }
            } else {
                for (int col = sideLength - 1; col >= 0; col--) {
                    grid[row][col] = Cell(pos++);
                }
            }
        }
    }

    // converts a linear position (1..size) into (row, col) in the grid
    pair<int, int> getCoordinates(int position) {
        int idx = position - 1;
        int rowFromBottom = idx / sideLength;
        int row = sideLength - 1 - rowFromBottom;
        int offset = idx % sideLength;
        int col = (rowFromBottom % 2 == 0) ? offset : (sideLength - 1 - offset);
        return {row, col};
    }

    bool addObstacle(Obstacle* obstacle) {
        pair<int, int> coord = getCoordinates(obstacle->src);
        int row = coord.first, col = coord.second;

        if (grid[row][col].hasObstacle()) return false; // avoid overlap

        grid[row][col].obstacle = obstacle;
        return true;
    }

    int getNewPosition(Player* player, int diceValue) {
        int newPos = player->position + diceValue;
        if (newPos > size) {
            return player->position; // overshoot, stay in place
        }
        pair<int, int> coord = getCoordinates(newPos);
        return grid[coord.first][coord.second].getFinalPosition();
    }

    void printBoard() {
        for (int row = 0; row < sideLength; row++) {
            for (int col = 0; col < sideLength; col++) {
                cout << grid[row][col].position << "\t";
            }
            cout << endl;
        }
    }
};

class Game {
public:
    deque<Player*> players;
    Board* board;
    int noOfSnakes;
    int noOfLadders;
    Dice* dice;
    ObstacleFactory factory;

    Game(int sideLength, int noOfSnakes, int noOfLadders, int noOfDice) {
        this->noOfSnakes = noOfSnakes;
        this->noOfLadders = noOfLadders;
        board = new Board(sideLength);
        dice = new Dice(noOfDice);
        setupObstacles();
    }

    void addPlayer(string name) {
        players.push_back(new Player(name));
    }

    void setupObstacles() {
        int size = board->size;

        int placed = 0;
        while (placed < noOfLadders) {
            int src = (rand() % (size - 1)) + 1;
            int dest = src + (rand() % (size - src));
            if (dest <= src) continue;
            Obstacle* ladder = factory.createObstacle(LADDER, src, dest);
            if (board->addObstacle(ladder)) placed++;
        }

        placed = 0;
        while (placed < noOfSnakes) {
            int dest = (rand() % (size - 1)) + 1;
            int src = dest + (rand() % (size - dest));
            if (src <= dest) continue;
            Obstacle* snake = factory.createObstacle(SNAKE, src, dest);
            if (board->addObstacle(snake)) placed++;
        }
    }

    void startGame() {
        while (true) {
            Player* currentPlayer = players.front();
            players.pop_front();

            int diceValue = dice->roll();
            int newPos = board->getNewPosition(currentPlayer, diceValue);
            currentPlayer->position = newPos;

            cout << currentPlayer->name << " rolled a " << diceValue
                 << " and moved to position " << newPos << endl;

            if (newPos == board->size) {
                cout << currentPlayer->name << " wins the game!" << endl;
                break;
            }

            players.push_back(currentPlayer);
        }
    }
};

int main() {
    srand(time(0));

    // Game(sideLength, noOfSnakes, noOfLadders, noOfDice)
    Game game(10, 5, 5, 1);

    game.addPlayer("Chaitanya");
    game.addPlayer("Player2");

    game.startGame();

    return 0;
}