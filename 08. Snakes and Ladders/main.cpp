/*
*REQUIREMENTS*:

*DYNAMIC*
-> Do we have fixed no. of snakes in our game? (Dynamic)
-> Do we have fixed no. of ladder in our game? (Dynamic)
-> Is the size of the board fixed? (Dynamic)
-> Is the game played between fixed no. of players? (Dynamic)
-> Is the no. of dice fixed? (Dynamic)

*EXTENSIBLE*
-> Should I assume that the game would just have snakes and ladder? (No)

*HIGH TRAFFIC*
-> Do we need to build a system where multiple games are running simultaneously? (No)

*ENTITIES*
-> Snake, Ladder, Cell, Board, Player, Dice, Game, Obstacle, ObstacleFactory, ObstacleType
*/

#include <iostream>
#include <vector>
#include <deque>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

// ==================== Obstacle (abstract) ====================
enum class ObstacleType {
    SNAKE,
    LADDER
};

class Obstacle {
protected:
    int src;
    int dest;

public:
    Obstacle(int src, int dest) : src(src), dest(dest) {}
    virtual ~Obstacle() = default;

    virtual ObstacleType getObstacleType() const = 0;

    // moves the player from src to dest and returns the new position
    virtual int movePlayer() const {
        return dest;
    }

    int getSrc() const { return src; }
    int getDest() const { return dest; }
};

// ==================== Ladder ====================
class Ladder : public Obstacle {
public:
    Ladder(int src, int dest) : Obstacle(src, dest) {}

    ObstacleType getObstacleType() const override {
        return ObstacleType::LADDER;
    }
};

// ==================== Snake ====================
class Snake : public Obstacle {
public:
    Snake(int src, int dest) : Obstacle(src, dest) {}

    ObstacleType getObstacleType() const override {
        return ObstacleType::SNAKE;
    }
};

// ==================== ObstacleFactory ====================
class ObstacleFactory {
public:
    static shared_ptr<Obstacle> createObstacle(ObstacleType type, int src, int dest) {
        switch (type) {
            case ObstacleType::LADDER:
                return make_shared<Ladder>(src, dest);
            case ObstacleType::SNAKE:
                return make_shared<Snake>(src, dest);
            default:
                throw invalid_argument("Unknown obstacle type");
        }
    }
};

// ==================== Cell ====================
class Cell {
private:
    int position;
    shared_ptr<Obstacle> obstacle; // nullptr if no obstacle

public:
    Cell() : position(0), obstacle(nullptr) {}
    explicit Cell(int position) : position(position), obstacle(nullptr) {}

    bool hasObstacle() const {
        return obstacle != nullptr;
    }

    void setObstacle(shared_ptr<Obstacle> obs) {
        obstacle = obs;
    }

    int getFinalPosition() const {
        if (hasObstacle()) {
            return obstacle->movePlayer();
        }
        return position;
    }

    int getPosition() const { return position; }
};

// ==================== Player ====================
class Player {
private:
    string name;
    int position;

public:
    explicit Player(const string& name) : name(name), position(0) {}

    string getName() const { return name; }
    int getPosition() const { return position; }
    void setPosition(int pos) { position = pos; }
};

// ==================== Dice ====================
class Dice {
private:
    int noOfDice;

public:
    explicit Dice(int noOfDice = 1) : noOfDice(noOfDice) {
        srand(static_cast<unsigned int>(time(nullptr)));
    }

    int roll() const {
        int total = 0;
        for (int i = 0; i < noOfDice; i++) {
            total += (rand() % 6) + 1; // 1 to 6
        }
        return total;
    }
};

// ==================== Board ====================
class Board {
private:
    int size;       // total number of cells (e.g. 100)
    int sideLength; // e.g. 10 for a 10x10 board
    vector<vector<Cell>> grid;

public:
    explicit Board(int sideLength) : sideLength(sideLength) {
        size = sideLength * sideLength;
        grid.resize(sideLength, vector<Cell>(sideLength));

        // initialize cells with positions 1..size in boustrophedon (snake) order
        int pos = 1;
        for (int row = sideLength - 1; row >= 0; row--) {
            if ((sideLength - 1 - row) % 2 == 0) {
                // left to right
                for (int col = 0; col < sideLength; col++) {
                    grid[row][col] = Cell(pos++);
                }
            } else {
                // right to left
                for (int col = sideLength - 1; col >= 0; col--) {
                    grid[row][col] = Cell(pos++);
                }
            }
        }
    }

    // maps a linear position (1..size) to (row, col) in the grid
    pair<int, int> getCoordinates(int position) const {
        int idx = position - 1;
        int rowFromBottom = idx / sideLength;
        int row = sideLength - 1 - rowFromBottom;
        int offsetInRow = idx % sideLength;
        int col;
        if (rowFromBottom % 2 == 0) {
            col = offsetInRow;
        } else {
            col = sideLength - 1 - offsetInRow;
        }
        return {row, col};
    }

    bool addObstacle(shared_ptr<Obstacle> obstacle) {
        int srcPos = obstacle->getSrc();
        if (srcPos < 1 || srcPos > size) return false;

        auto [row, col] = getCoordinates(srcPos);
        if (grid[row][col].hasObstacle()) return false; // avoid overlapping obstacles

        grid[row][col].setObstacle(obstacle);
        return true;
    }

    // given a player's current position and a dice roll, returns the final
    // position after applying board boundaries and any obstacle (snake/ladder)
    int getNewPosition(const Player& player, int diceValue) const {
        int newPos = player.getPosition() + diceValue;
        if (newPos > size) {
            // overshoot: player doesn't move (common rule variant)
            return player.getPosition();
        }

        auto [row, col] = getCoordinates(newPos);
        return grid[row][col].getFinalPosition();
    }

    void printBoard() const {
        for (int row = 0; row < sideLength; row++) {
            for (int col = 0; col < sideLength; col++) {
                const Cell& cell = grid[row][col];
                cout << cell.getPosition();
                if (cell.hasObstacle()) cout << "*";
                cout << "\t";
            }
            cout << "\n";
        }
    }

    int getSize() const { return size; }
};

// ==================== Game ====================
class Game {
private:
    deque<shared_ptr<Player>> players;
    unique_ptr<Board> board;
    int noOfSnakes;
    int noOfLadders;
    unique_ptr<Dice> dice;

public:
    Game(int sideLength, int noOfSnakes, int noOfLadders, int noOfDice)
        : noOfSnakes(noOfSnakes), noOfLadders(noOfLadders) {
        board = make_unique<Board>(sideLength);
        dice = make_unique<Dice>(noOfDice);
        setupObstacles();
    }

    void addPlayer(const string& name) {
        players.push_back(make_shared<Player>(name));
    }

    void setupObstacles() {
        int boardSize = board->getSize();
        srand(static_cast<unsigned int>(time(nullptr)));

        int placed = 0;
        while (placed < noOfLadders) {
            int src = (rand() % (boardSize - 1)) + 1;
            int dest = src + (rand() % (boardSize - src));
            if (dest <= src) continue;
            auto ladder = ObstacleFactory::createObstacle(ObstacleType::LADDER, src, dest);
            if (board->addObstacle(ladder)) placed++;
        }

        placed = 0;
        while (placed < noOfSnakes) {
            int dest = (rand() % (boardSize - 1)) + 1;
            int src = dest + (rand() % (boardSize - dest));
            if (src <= dest) continue;
            auto snake = ObstacleFactory::createObstacle(ObstacleType::SNAKE, src, dest);
            if (board->addObstacle(snake)) placed++;
        }
    }

    void startGame() {
        if (players.empty()) {
            cout << "No players in the game!\n";
            return;
        }

        int boardSize = board->getSize();

        while (true) {
            auto currentPlayer = players.front();
            players.pop_front();

            int diceValue = dice->roll();
            int newPos = board->getNewPosition(*currentPlayer, diceValue);
            currentPlayer->setPosition(newPos);

            cout << currentPlayer->getName() << " rolled a " << diceValue
                 << " and moved to position " << newPos << "\n";

            if (newPos == boardSize) {
                cout << currentPlayer->getName() << " wins the game!\n";
                break;
            }

            players.push_back(currentPlayer);
        }
    }
};

// ==================== main ====================
int main() {
    // 10x10 board = 100 cells, 5 snakes, 5 ladders, single dice
    Game game(10, 5, 5, 1);

    game.addPlayer("Chaitanya");
    game.addPlayer("Player2");

    game.startGame();

    return 0;
}