#include <vector>
#include <string>
#include<iostream>
#pragma once
using namespace std;
class Board {
    private:
        vector<vector<string>> table;
        const int size = 10;
        string emptyPick = "*";
        string shipPick = "\033[36m#\033[0m";
        string hitPick = "\033[32mS\033[0m";
        string missPick = "\033[31mM\033[0m";
    
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
            if (x < 0 || x >= size || y < 0 || y >= size) {
                throw out_of_range("Invalid coordinates in getKey");
            }
            return table[x][y];
        }
    
        bool placeShip(int x, int y, int length, bool isHorizontal) {
            if (x < 0 || x >= size || y < 0 || y >= size) {
                throw out_of_range("Invalid coordinates in placeShip");
            }
    
            if (isHorizontal) {
                if (y + length > size) {
                    throw invalid_argument("Ship does not fit horizontally");
                }
    
                for (int i = y; i < y + length; ++i) {
                    if (table[x][i] == shipPick) {
                        throw invalid_argument("Ship overlap detected");
                    }
                }
    
                for (int i = y; i < y + length; ++i) {
                    table[x][i] = shipPick;
                }
            } else {
                if (x + length > size) {
                    throw invalid_argument("Ship does not fit vertically");
                }
    
                for (int i = x; i < x + length; ++i) {
                    if (table[i][y] == shipPick) {
                        throw invalid_argument("Ship overlap detected");
                    }
                }
    
                for (int i = x; i < x + length; ++i) {
                    table[i][y] = shipPick;
                }
            }
            return true;
        }
    
        bool attack(int x, int y) {
            if (x < 0 || x >= size || y < 0 || y >= size) {
                throw out_of_range("Invalid coordinates in attack");
            }
    
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
                    if (cell == shipPick) {
                        return false;
                    }
                }
            }
            return true;
        }
    };
