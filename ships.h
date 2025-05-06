#include "boards.h"
#include <vector>
#pragma once
struct Position {
    int x;
    int y;
};
class Ship {
    private:
        vector<Position> _positions;
        int _x, _y, _len, _hitsTaken;
        bool _isHorizontal;
        bool counted = false;
    public:
        Ship(int x, int y, int len, bool horizontal){
            setX(x);
            setY(y);
            setLen(len);
            setIsHorizontal(horizontal);
            for (int i = 0; i < len; ++i) {
                Position pos;
                pos.x = x + (horizontal ? 0 : i);
                pos.y = y + (horizontal ? i : 0);
                _positions.push_back(pos);
            }
        }
    
        void setX(int x){
            _x = x;
        }
    
        void setY(int y){
            _y = y;
        }
    
        void setLen(int len){
            _len = len;
        }
    
        void setIsHorizontal(bool horizontal){
            _isHorizontal = horizontal;
        }
    
        int getY(){
            return _y;
        }
    
        int getX(){
            return _x;
        }
    
        int getLen() const {
            return _len;
        }
    
        bool getIsHorizontal(){
            return _isHorizontal;
        }
    
        vector<Position> getPositions() const {
            return _positions;
        }
    
        bool isCounted() const {
            return counted; 
        }
    
        void markCounted() { 
            counted = true;
        }
    
        static bool place(Board& board, vector<Ship>& ships, int x, int y, int len, const string& direction,
            int& one, int& two, int& three, int& four) {
    
            try {
                if ((len == 1 && one >= 4) || (len == 2 && two >= 3) ||
                    (len == 3 && three >= 2) || (len == 4 && four >= 1)) {
                    cout << "You can't place more ships of this length" << endl;
                    return false;
                }
    
                int dx = 0, dy = 0;
                if (direction == "right") dy = 1;
                else if (direction == "left") dy = -1;
                else if (direction == "down") dx = 1;
                else if (direction == "up") dx = -1;
                else {
                    cout << "Invalid direction input" << endl;
                    return false;
                }
    
                vector<Position> shipPositions;
                for (int i = 0; i < len; ++i) {
                    int nx = x + dx * i;
                    int ny = y + dy * i;
                    if (nx < 0 || nx >= 10 || ny < 0 || ny >= 10) {
                        cout << "Ship goes out of bounds" << endl;
                        return false;
                    }
    
                    shipPositions.push_back({nx, ny});
                }
    
                for (const auto& pos : shipPositions) {
                    int dirX[] = { -1, 1, 0, 0 };
                    int dirY[] = { 0, 0, -1, 1 };
    
                    for (int i = 0; i < 4; ++i) {
                        int adjX = pos.x + dirX[i];
                        int adjY = pos.y + dirY[i];
    
                        if (adjX >= 0 && adjX < 10 && adjY >= 0 && adjY < 10) {
                            if (board.getKey(adjX, adjY) == "\033[36m#\033[0m") {
                                cout << "Too close to another ship (side-by-side not allowed)" << endl;
                                return false;
                            }
                        }
                    }
                }
    
                bool isHorizontal = (dx == 0);
                if (!board.placeShip(x, y, len, isHorizontal)) {
                    cout << "Ship placement failed!" << endl;
                    return false;
                }
    
                ships.emplace_back(x, y, len, isHorizontal);
                if (len == 1) one++;
                else if (len == 2) two++;
                else if (len == 3) three++;
                else if (len == 4) four++;
    
                return true;
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
                return false;
            }
        }
    
        bool isSunk(const Board& board) const {
            for (const auto& pos : _positions) {
                if (board.getKey(pos.x, pos.y) != "\033[32mS\033[0m") {
                    return false;  
                }
            }
            return true;
        }
    };