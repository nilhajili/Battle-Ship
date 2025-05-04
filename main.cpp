#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include <utility>
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68
#define KEY_ENTER 10
using namespace std;
struct Position {
    int x;
    int y;
};
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
    string emptyPick = "*";
    string shipPick = "\033[36m#\033[0m";
    string hitPick = "\033[32mS\033[0m";
    string missPick= "\033[31mM\033[0m";

public:
    Board() {
        table.resize(size, vector<string>(size, emptyPick));
    }
    void displayBoard(int cursorX = -1, int cursorY = -1, int length = 1,
        const string& direction = "right", const string& mode = "owner") const {
        cout << "  ";
        for (int i = 0; i < size; ++i) {
        cout << i << " ";
        }
        cout << endl;

        for (int i = 0; i < size; ++i) {
        cout << i << " ";
        for (int j = 0; j < size; ++j) {
            bool isCursor = false;
            for (int k = 0; k < length; ++k) {
                int xx = cursorX, yy = cursorY;
                if (direction == "right") yy = cursorY + k;
                else if (direction == "left") yy = cursorY - (length - 1) + k;
                else if (direction == "down") xx = cursorX + k;
                else if (direction == "up") xx = cursorX - (length - 1) + k;
                if (i == xx && j == yy) {
                    isCursor = true;
                    break;
                }
            }

            string cell = table[i][j];
            if (mode == "opponent" && cell == shipPick) {
                // Opponent's ships are hidden
                cell = emptyPick;
            }

            if (isCursor)
                cout << "\033[35m" << "+" << ' ' << "\033[0m";
            else
                cout << cell << " ";
        }
        cout << endl;
        }
}

    string getKey(int x, int y) const {
        return table[x][y];
    }

    bool placeShip(int x, int y, int length, bool isHorizontal) {
        if (isHorizontal) {
            if (y + length > size) return false;

            for (int i = y; i < y + length; ++i) {
                if (table[x][i] == shipPick) return false;
            }

            for (int i = y; i < y + length; ++i) {
                table[x][i] = shipPick;
            }
        } else {
            if (x + length > size) return false;

            for (int i = x; i < x + length; ++i) {
                if (table[i][y] == shipPick) return false;
            }

            for (int i = x; i < x + length; ++i) {
                table[i][y] = shipPick;
            }
        }
        return true;
    }
    bool attack(int x, int y) {
        if (x < 0 || x >= size || y < 0 || y >= size)
            return false;
    
        if (table[x][y] == shipPick) {
            table[x][y] = hitPick;
            return true;
        } else if (table[x][y] == emptyPick) {
            table[x][y] = missPick;
        }
        return false;
    }
    
    bool isGameOver() const {
        for (const auto& row : table) {
            for (const auto& cell : row) {
                if (cell == shipPick)
                    return false;
            }
        }
        return true;
    }
};
class Ship {
private:
    int _x, _y, _len, _hitsTaken;
    bool _isHorizontal;
public:
    Ship(int x, int y, int len, bool horizontal){
        setX(x);
        setY(y);
        setLen(len);
        setIsHorizontal(horizontal);
    }
    void setX(int x){
        _x=x;
    }
    void setY(int y){
        _y=y;
    }
    void setLen(int len){
        _len=len;
    }
    void setIsHorizontal(bool horizontal){
        _isHorizontal=horizontal;
    }
    int getY(){
        return _y;
    }
    int getX(){
        return _x;
    }
    int getLen(){
        return _len;
    }
    bool getIsHorizontal(){
        return _isHorizontal;
    }

    static bool place(Board& board, vector<Ship>& ships, int x, int y, int len, const string& direction,
                      int& one, int& two, int& three, int& four) {
        if ((len == 1 && one >= 4) || (len == 2 && two >= 3) ||
            (len == 3 && three >= 2) || (len == 4 && four >= 1)) {
            cout << "You can't place more ships of this length"<<endl;
            return false;
        }

        int dx = 0, dy = 0;
        if (direction == "right") dy = 1;
        else if (direction == "left") dy = -1;
        else if (direction == "down") dx = 1;
        else if (direction == "up") dx = -1;
        else {
            cout << "Invalid direction input"<<endl;
            return false;
        }

        for (int i = 0; i < len; ++i) {
            int nx = x + dx * i;
            int ny = y + dy * i;
            if (nx < 0 || nx >= 10 || ny < 0 || ny >= 10) {
                cout << "Ship goes out of bounds"<<endl;
                return false;
            }
            if (board.getKey(nx, ny) == "#") {
                cout << "Collision detected. Ship can't be placed"<<endl;
                return false;
            }
        }
        bool isHorizontal = (dx == 0);
        if (!board.placeShip(x, y, len, isHorizontal)) {
            cout << "Ship placement failed!\n";
            return false;
        }

        ships.emplace_back(x, y, len, isHorizontal);
        if (len == 1) one++;
        else if (len == 2) two++;
        else if (len == 3) three++;
        else if (len == 4) four++;
        return true;
    }
};
class Player {
private:
    string _name;
    Board board;
    vector<Ship> ships;
    int one = 0, two = 0, three = 0, four = 0;

public:
    Player(const string& playerName)   {
        setName(playerName);
    }
    void setName(string name){
        _name=name;
    }
    string getName() const {
        return _name;
    }

    Board& getBoard() {
        return board;
    }

    const vector<Ship>& getShips() const {
        return ships;
    }

    bool placeShip(int x, int y, int length, const string& direction) {
        return Ship::place(board, ships, x, y, length, direction, one, two, three, four);
    }

    void displayBoard(int cursorX = -1, int cursorY = -1, int length=1 , const string& direction="right") const {
       board.displayBoard(cursorX, cursorY, length,direction);
    }
    void placeShips() {
    int x = 0, y = 0;
    string direction = "right";
    int shipLength = 1;
    while (true) {
        system("clear");
        cout <<"\033[34m"<<"    ███████╗██╗  ██╗██╗██████╗     ██████╗ ██╗      █████╗  ██████╗███████╗ " <<"\033[0m"<<endl;
        cout <<"\033[34m"<< "    ██╔════╝██║  ██║██║██╔══██╗    ██╔══██╗██║     ██╔══██╗██╔════╝██╔════╝"<<"\033[0m"<<endl;
        cout <<"\033[34m"<< "    ███████╗███████║██║██████╔╝    ██████╔╝██║     ███████║██║     █████╗  "<<"\033[0m"<<endl;
        cout <<"\033[34m"<< "    ╚════██║██╔══██║██║██╔═══╝     ██╔═══╝ ██║     ██╔══██║██║     ██╔══╝  "<<"\033[0m"<<endl;
        cout <<"\033[34m"<< "    ███████║██║  ██║██║██║         ██║     ███████╗██║  ██║╚██████╗███████╗"<<"\033[0m"<<endl;
        cout <<"\033[34m"<< "    ╚══════╝╚═╝  ╚═╝╚═╝╚═╝         ╚═╝     ╚══════╝╚═╝  ╚═╝ ╚═════╝╚══════╝"<<"\033[0m"<<endl;
        cout <<"\033[33m"<< _name << " - Place your ships\n";
        cout <<"\033[33m"<< "Use arrows to move | 'r' rotate | '1'-'4' to set ship length | Enter to place\n";
        cout <<"\033[33m"<< "Ship Length: " <<"\033[0m"<<"\033[36m"<< shipLength <<"\033[0m"<<"\033[33m"<< "  Direction: " <<"\033[0m"<<"\033[36m"<<direction<<"\033[0m"<<"\033[33m"<< "   Cursor: (" <<"\033[36m"<< x <<"\033[0m"<<"\033[33m"<<", "<<"\033[0m"<<"\033[36m"<< y <<"\033[0m"<<"\033[33m"<<")"<<"\033[0m"<<endl;
        printShipCount();
        cout<<endl;
        displayBoard(x, y,shipLength,direction);
        int key = getKeyPress();
        if (key == KEY_UP && x > 0) x--;
        else if (key == KEY_DOWN && x < 9) x++;
        else if (key == KEY_LEFT && y > 0) y--;
        else if (key == KEY_RIGHT && y < 9) y++;
        else if (key == KEY_ENTER) {
            if (placeShip(x, y, shipLength, direction)) {
                if (ships.size() == 10) break;
            }
        } else if (key == 'r') {
            if (direction == "right") direction = "down";
            else if (direction == "down") direction = "left";
            else if (direction == "left") direction = "up";
            else direction = "right";
        } else if (key == 'n') {
            shipLength++;
            if (shipLength > 4) shipLength = 1;
        }
    }
}


    void printShipCount() const {
        cout <<"\033[33m"<< "Placed Ships - 1x" <<"\033[0m"<<"\033[36m"<<one<<"\033[0m"
             <<"\033[33m"<< "  2x" <<"\033[0m"<<"\033[36m"<<two<<"\033[0m"
             <<"\033[33m"<< "  3x" <<"\033[0m"<<"\033[36m"<<three<<"\033[0m"
             <<"\033[33m"<< "  4x" <<"\033[0m"<<"\033[36m"<<four <<"\033[0m"<<endl;
    }
};
class Bot : public Player {
    private:
        vector<Position> attackHistory;
        vector<Position> hitTargets;
    
        bool wasAttacked(int x, int y) const {
            for (const auto& pos : attackHistory) {
                if (pos.x == x && pos.y == y)
                    return true;
            }
            return false;
        }
    
    public:
        Bot(const string& name) : Player(name) {
            srand(static_cast<unsigned int>(time(nullptr)));
        }
        void placeShips() {
            char board[10][10] = {};  
            int shipLengths[] = {1, 2, 3, 4};
            int shipCounts[] = {4, 3, 2, 1};  
            for (int i = 0; i < 4; ++i) {
                int len = shipLengths[i];   
                int count = shipCounts[i]; 
        
                while (count > 0) {  
                    bool placed = false;
                    while (!placed) {
                        int x = rand() % 10;
                        int y = rand() % 10;
                        string direction = (rand() % 4 == 0) ? "right" : (rand() % 3 == 0) ? "down" : (rand() % 2 == 0) ? "left" : "up";  
                        if (direction == "right" && y + len <= 10) {
                            bool collision = false;
                            for (int i = 0; i < len; ++i) {
                                if (board[x][y + i] == 'X') {
                                    collision = true;
                                    break;
                                }
                            }
                            if (!collision) {
                                placed = placeShip(x, y, len, "right");
                                for (int i = 0; i < len; ++i) {
                                    board[x][y + i] = 'X';
                                }
                                count--;  
                            }
                        }
                        else if (direction == "down" && x + len <= 10) {
                            bool collision = false;
                            for (int i = 0; i < len; ++i) {
                                if (board[x + i][y] == 'X') {
                                    collision = true;
                                    break;
                                }
                            }
                            if (!collision) {
                                placed = placeShip(x, y, len, "down");
                                for (int i = 0; i < len; ++i) {
                                    board[x + i][y] = 'X';
                                }
                                count--; 
                            }
                        }
                        else if (direction == "left" && y - len >= 0) {
                            bool collision = false;
                            for (int i = 0; i < len; ++i) {
                                if (board[x][y - i] == 'X') {
                                    collision = true;
                                    break;
                                }
                            }
                            if (!collision) {
                                
                                placed = placeShip(x, y, len, "left");
                                for (int i = 0; i < len; ++i) {
                                    board[x][y - i] = 'X';
                                }
                                count--; 
                            }
                        }
                        else if (direction == "up" && x - len >= 0) {
                            bool collision = false;
                            for (int i = 0; i < len; ++i) {
                                if (board[x - i][y] == 'X') {
                                    collision = true;
                                    break;
                                }
                            }
                            if (!collision) {
                                
                                placed = placeShip(x, y, len, "up");
                                
                                for (int i = 0; i < len; ++i) {
                                    board[x - i][y] = 'X';
                                }
                                count--;  
                            }
                        }
                    }
                }
            }
        }
        
        
    };
int main() {
    }