#include "Players.h"
#include <iostream>
#include <cstdlib>
#pragma once
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
        
            vector<string> directions = {"right", "down", "left", "up"};
        
            for (int i = 0; i < 4; ++i) {
                int len = shipLengths[i];
                int count = shipCounts[i];
        
                while (count > 0) {
                    bool placed = false;
        
                    while (!placed) {
                        int x = rand() % 10;
                        int y = rand() % 10;
                        string direction = directions[rand() % 4];
        
                        if (direction == "right" && y + len <= 10) {
                            bool collision = false;
                            for (int j = 0; j < len; ++j)
                                if (board[x][y + j] == 'X') collision = true;
        
                            if (!collision) {
                                placed = placeShip(x, y, len, "right");
                                if (placed) {
                                    for (int j = 0; j < len; ++j)
                                        board[x][y + j] = 'X';
                                    count--;
                                }
                            }
                        }
        
                        else if (direction == "down" && x + len <= 10) {
                            bool collision = false;
                            for (int j = 0; j < len; ++j)
                                if (board[x + j][y] == 'X') collision = true;
        
                            if (!collision) {
                                placed = placeShip(x, y, len, "down");
                                if (placed) {
                                    for (int j = 0; j < len; ++j)
                                        board[x + j][y] = 'X';
                                    count--;
                                }
                            }
                        }
        
                        else if (direction == "left" && y - len + 1 >= 0) {
                            bool collision = false;
                            for (int j = 0; j < len; ++j)
                                if (board[x][y - j] == 'X') collision = true;
        
                            if (!collision) {
                                placed = placeShip(x, y, len, "left");
                                if (placed) {
                                    for (int j = 0; j < len; ++j)
                                        board[x][y - j] = 'X';
                                    count--;
                                }
                            }
                        }
        
                        else if (direction == "up" && x - len + 1 >= 0) {
                            bool collision = false;
                            for (int j = 0; j < len; ++j)
                                if (board[x - j][y] == 'X') collision = true;
        
                            if (!collision) {
                                placed = placeShip(x, y, len, "up");
                                if (placed) {
                                    for (int j = 0; j < len; ++j)
                                        board[x - j][y] = 'X';
                                    count--;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        Position chooseTarget() {
            for (auto it = hitTargets.rbegin(); it != hitTargets.rend(); ++it) {
                Position lastHit = *it;
                vector<Position> directions = {
                    {0, 1}, {1, 0}, {0, -1}, {-1, 0}
                };
        
                for (const auto& dir : directions) {
                    int nx = lastHit.x + dir.x;
                    int ny = lastHit.y + dir.y;
                    if (nx >= 0 && nx < 10 && ny >= 0 && ny < 10 && !wasAttacked(nx, ny)) {
                        return {nx, ny};
                    }
                }
            }
        
            int x, y;
            int tries = 0;
            do {
                x = rand() % 10;
                y = rand() % 10;
                tries++;
                if (tries > 100) break; 
            } while (wasAttacked(x, y));
        
            return {x, y};
        }
    
        bool performAttack(Board& opponentBoard) {
            Position target = chooseTarget();  
            attackHistory.push_back(target);
            cout << getName() << " attacks at (" << target.x << ", " << target.y << ")"<<endl;
            
            bool hit = opponentBoard.attack(target.x, target.y);  
            if (hit) {
                cout << "Hit"<<endl;
                hitTargets.push_back(target);  
                return true;  
            } else {
                cout << "Miss"<<endl;
            }
            sleep(1);
            return false;
        }
    };