#include "ships.h"
#include "boards.h"
#include <string>
#include <vector>
#include"utils.h"
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68
#define KEY_ENTER 10
#pragma once
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
        void checkSunkShips(int x, int y) {
            for (auto& ship : ships) {
                for (const auto& pos : ship.getPositions()) {
                    if (pos.x == x && pos.y == y) {
                        if (ship.isSunk(board) && !ship.isCounted()) {
                            ship.markCounted();  
                            cout << "\033[31mShip is completely sunk!\033[0m\n";
        
                            int remaining = 0;
                            int length = ship.getLen();
                            for (const auto& s : ships) {
                                if (s.getLen() == length && !s.isSunk(board)) {
                                    remaining++;
                                }
                            }
        
                            cout << "Remaining ships of length " << length << ": " << remaining << "\n";
                            cout << "Press any key to continue...\n";
                            getKeyPress();
                        } else if (!ship.isSunk(board)) {
                            cout << "\033[33mHit, but ship is still afloat.\033[0m\n";
                            cout << "Press any key to continue...\n";
                            getKeyPress();
                        }
                        return;
                    }
                }
            }
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