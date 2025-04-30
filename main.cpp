#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <vector>

#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68
#define KEY_ENTER 10

using namespace std;

int getKeyPress() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();
    if (ch == 27) {
        int ch1 = getchar();
        int ch2 = getchar();
        if (ch1 == 91) {
            ch = ch2;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

class Board {
private:
    vector<vector<string> > table;
    const int size = 10;
    string emptyCell = "*";
    string shipCell = "#";
    string hitCell = "S";
    string missCell = "M";

public:
    Board() {
        table.resize(size, vector<string>(size, emptyCell));
    }

    void displayBoard(int cursorX = -1, int cursorY = -1) const {
        cout << "  ";
        for (int i = 0; i < size; ++i) {
            cout << i << " ";
        }
        cout << endl;

        for (int i = 0; i < size; ++i) {
            cout << i << " ";
            for (int j = 0; j < size; ++j) {
                if (i == cursorX && j == cursorY)
                    cout << "@" << " ";
                else
                    cout << table[i][j] << " ";
            }
            cout << endl;
        }
    }

    string getCell(int x, int y) const {
        return table[x][y];
    }

    bool placeShip(int x, int y, int length, bool isHorizontal) {
        if (isHorizontal) {
            if (y + length > size) return false;

            for (int i = y; i < y + length; ++i) {
                if (table[x][i] == shipCell) return false;
            }

            for (int i = y; i < y + length; ++i) {
                table[x][i] = shipCell;
            }
        } else {
            if (x + length > size) return false;

            for (int i = x; i < x + length; ++i) {
                if (table[i][y] == shipCell) return false;
            }

            for (int i = x; i < x + length; ++i) {
                table[i][y] = shipCell;
            }
        }
        return true;
    }

    bool attack(int x, int y) {
        if (table[x][y] == shipCell) {
            table[x][y] = hitCell;
            return true;
        } else if (table[x][y] == emptyCell) {
            table[x][y] = missCell;
            return false;
        }
        return false;
    }

    bool isGameOver() const {
        for (const auto& row : table) {
            for (const auto& cell : row) {
                if (cell == shipCell) return false;
            }
        }
        return true;
    }
};

class Ship {
private:
    int x, y, length;
    bool isHorizontal;
    int hitsTaken;

public:
    Ship(int startX, int startY, int len, bool horizontal)
        : x(startX), y(startY), length(len), isHorizontal(horizontal), hitsTaken(0) {}

    static bool place(Board& board, vector<Ship>& ships, int x, int y, int len, const string& direction,
                      int& ones, int& twos, int& threes, int& fours) {
        if ((len == 1 && ones >= 4) || (len == 2 && twos >= 3) ||
            (len == 3 && threes >= 2) || (len == 4 && fours >= 1)) {
            cout << "You can't place more ships of this length!\n";
            return false;
        }

        int dx = 0, dy = 0;
        if (direction == "right") dy = 1;
        else if (direction == "left") dy = -1;
        else if (direction == "down") dx = 1;
        else if (direction == "up") dx = -1;
        else {
            cout << "Invalid direction input.\n";
            return false;
        }

        for (int i = 0; i < len; ++i) {
            int nx = x + dx * i;
            int ny = y + dy * i;
            if (nx < 0 || nx >= 10 || ny < 0 || ny >= 10) {
                cout << "Ship goes out of bounds.\n";
                return false;
            }
            if (board.getCell(nx, ny) == "#") {
                cout << "Collision detected. Ship can't be placed.\n";
                return false;
            }
        }

        bool isHorizontal = (dx == 0);
        if (!board.placeShip(x, y, len, isHorizontal)) {
            cout << "Ship placement failed!\n";
            return false;
        }

        ships.emplace_back(x, y, len, isHorizontal);
        if (len == 1) ones++;
        else if (len == 2) twos++;
        else if (len == 3) threes++;
        else if (len == 4) fours++;
        return true;
    }
};

int main() {
    Board board;
    vector<Ship> ships;

    int x = 0, y = 0;
    string direction = "right";
    int ones = 0, twos = 0, threes = 0, fours = 0;
    int shipLength = 1;

    while (true) {
        system("clear");
        cout << "    ███████╗██╗  ██╗██╗██████╗     ██████╗ ██╗      █████╗  ██████╗███████╗\n";
        cout << "    ██╔════╝██║  ██║██║██╔══██╗    ██╔══██╗██║     ██╔══██╗██╔════╝██╔════╝\n";
        cout << "    ███████╗███████║██║██████╔╝    ██████╔╝██║     ███████║██║     █████╗  \n";
        cout << "    ╚════██║██╔══██║██║██╔═══╝     ██╔═══╝ ██║     ██╔══██║██║     ██╔══╝  \n";
        cout << "    ███████║██║  ██║██║██║         ██║     ███████╗██║  ██║╚██████╗███████╗\n";
        cout << "    ╚══════╝╚═╝  ╚═╝╚═╝╚═╝         ╚═╝     ╚══════╝╚═╝  ╚═╝ ╚═════╝╚══════╝\n\n";

        cout << "Use arrow keys to move\n";
        cout << "Press 'r' to rotate | '1'-'4' to set ship length | Enter to place\n";
        cout << "Ship Length: " << shipLength << " | Direction: " << direction
             << " | Cursor: (" << x << ", " << y << ")\n";
        cout << "Placed: 1x" << ones << "  2x" << twos << "  3x" << threes << "  4x" << fours << "\n\n";
        board.displayBoard(x, y);
        int key = getKeyPress();
        if (key == KEY_UP && x > 0) x--;
        else if (key == KEY_DOWN && x < 9) x++;
        else if (key == KEY_LEFT && y > 0) y--;
        else if (key == KEY_RIGHT && y < 9) y++;
        else if (key == KEY_ENTER) {
            if (Ship::place(board, ships, x, y, shipLength, direction, ones, twos, threes, fours)) {
                sleep(1);
            } else {
                sleep(1);
            }
        } else if (key == 'r') {
            if (direction == "right") direction = "down";
            else if (direction == "down") direction = "left";
            else if (direction == "left") direction = "up";
            else direction = "right";
        } else if (key == 'n') {
            shipLength++;
            if (shipLength > 4) shipLength = 1;
        } else if (key >= '1' && key <= '4') {
            shipLength = key - '0';
        }}


}
