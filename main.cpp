#include "Players.h"
#include "bots.h"
int main() {
    int secim = 0;
    while (true) {
        system("clear");
        cout << "\033[34m" << "    ███████╗██╗  ██╗██╗██████╗     ██████╗ ██╗      █████╗  ██████╗███████╗ " << "\033[0m" << endl;
        cout << "\033[34m" << "    ██╔════╝██║  ██║██║██╔══██╗    ██╔══██╗██║     ██╔══██╗██╔════╝██╔════╝" << "\033[0m" << endl;
        cout << "\033[34m" << "    ███████╗███████║██║██████╔╝    ██████╔╝██║     ███████║██║     █████╗  " << "\033[0m" << endl;
        cout << "\033[34m" << "    ╚════██║██╔══██║██║██╔═══╝     ██╔═══╝ ██║     ██╔══██║██║     ██╔══╝  " << "\033[0m" << endl;
        cout << "\033[34m" << "    ███████║██║  ██║██║██║         ██║     ███████╗██║  ██║╚██████╗███████╗" << "\033[0m" << endl;
        cout << "\033[34m" << "    ╚══════╝╚═╝  ╚═╝╚═╝╚═╝         ╚═╝     ╚══════╝╚═╝  ╚═╝ ╚═════╝╚══════╝" << "\033[0m" << endl;

        string options[] = {
            "1. Bot vs Player",
            "2. Player vs Player",
            "3. Bot vs Bot",
            "4. Exit"
        };

        for (int i = 0; i < 4; ++i) {
            if (i == secim)
                cout << "\033[32m > " << options[i] << "\033[0m" << endl;
            else
                cout << "   " << options[i] << endl;
        }

        int c = getch();
        if (c == 27) {
            int c1 = getch();
            if (c1 == 91) {
                int c2 = getch();
                switch (c2) {
                    case KEY_UP:
                        secim = (secim > 0) ? secim - 1 : 3;
                        break;
                    case KEY_DOWN:
                        secim = (secim < 3) ? secim + 1 : 0;
                        break;
                }
            }
        } else if (c == KEY_ENTER) {
            try {
                switch (secim) {
                    case 0: {
                        Player player1("Player 1");
                        Bot bot("Bot");

                        player1.placeShips();
                        cout << "\nPlayer 1 done. Press any key to start the game";
                        getKeyPress();
                        system("clear");

                        bot.placeShips();
                        cout << "\nBot done. Press any key to start the game";
                        getKeyPress();

                        int currentPlayer = 1;
                        int x = 0, y = 0;
                        int moveCount = 0;
                        while (true) {
                            system("clear");
                            Player& attacker = (currentPlayer == 1) ? player1 : bot;
                            Player& defender = (currentPlayer == 1) ? bot : player1;

                            cout << attacker.getName() << " Turn. Press Enter to fire"<<endl;
                            cout << "Player's Board:               Bot's Board:"<<endl;

                            for (int i = 0; i < 10; ++i) {
                                for (int j = 0; j < 10; ++j)
                                    cout << player1.getBoard().getKey(i, j) << " ";
                                cout << "         ";
                                for (int j = 0; j < 10; ++j) {
                                    if (i == x && j == y) cout << "\033[7m";
                                    string cell = bot.getBoard().getKey(i, j);
                                    cout << (cell == "\033[36m#\033[0m" ? "*" : cell);
                                    if (i == x && j == y) cout << "\033[0m";
                                    cout << " ";
                                }
                                cout << endl;
                            }

                            if (currentPlayer == 1) {
                                cout << "\nUse arrow keys to move, Enter to attack. Cursor at (" << x << ", " << y << ")"<<endl;
                                int key = getKeyPress();
                                if (key == KEY_UP && x > 0) x--;
                                else if (key == KEY_DOWN && x < 9) x++;
                                else if (key == KEY_LEFT && y > 0) y--;
                                else if (key == KEY_RIGHT && y < 9) y++;
                                else if (key == KEY_ENTER) {
                                    if (bot.getBoard().getKey(x, y) != "\033[32mS\033[0m" &&
                                        bot.getBoard().getKey(x, y) != "\033[31mM\033[0m") {
                                        bool hit = bot.getBoard().attack(x, y);
                                        if (hit) {
                                            cout << "\n\033[32mHit\033[0m"<<endl;
                                            bot.checkSunkShips(x, y);
                                            moveCount++;
                                            getKeyPress();
                                            continue;
                                        } else {
                                            cout << "\n\033[31mMiss\033[0m"<<endl;
                                            currentPlayer = 2;
                                            moveCount++;
                                        }
                                        if (bot.getBoard().isGameOver()) {
                                            cout << "You won"<<endl;
                                            getKeyPress();
                                            return 0;
                                        }
                                    } else {
                                        cout << "Already attacked this position\n";
                                        sleep(1);
                                    }
                                }
                            } else {
                                cout << "\nBot is attacking"<<endl;
                                sleep(1);
                                bool hit = bot.performAttack(player1.getBoard());
                                if (hit) {
                                    moveCount++;
                                    sleep(1);
                                    continue;
                                }
                                if (player1.getBoard().isGameOver()) {
                                    cout << "Bot won"<<endl;
                                    getKeyPress();
                                    return 0;
                                }
                                currentPlayer = 1;
                            }
                            cout << "\nTotal moves: " << moveCount << endl;
                            getKeyPress();
                        }
                    }

                    case 1: {
                        string name1, name2;
                        cout << "Enter name for Player 1: ";
                        getline(cin, name1);
                        cout << "Enter name for Player 2: ";
                        getline(cin, name2);

                        Player player1(name1);
                        Player player2(name2);

                        player1.placeShips();
                        cout << "\n" << name1 << " done. Press any key to switch to " << name2;
                        getKeyPress();
                        system("clear");

                        player2.placeShips();
                        cout << "\n" << name2 << " done. Press any key to start the game";
                        getKeyPress();

                        int currentPlayer = 1, x = 0, y = 0, moveCount = 0;

                        while (true) {
                            system("clear");
                            Player& attacker = (currentPlayer == 1) ? player1 : player2;
                            Player& defender = (currentPlayer == 1) ? player2 : player1;

                            cout << attacker.getName() << " Turn. Press Enter to fire"<<endl;
                            defender.getBoard().displayBoard(x, y, 1, "right", "opponent");

                            int key = getKeyPress();
                            if (key == KEY_UP && x > 0) x--;
                            else if (key == KEY_DOWN && x < 9) x++;
                            else if (key == KEY_LEFT && y > 0) y--;
                            else if (key == KEY_RIGHT && y < 9) y++;
                            else if (key == KEY_ENTER) {
                                bool hit = defender.getBoard().attack(x, y);
                                if (hit) {
                                    cout << "\n\033[32mHit\033[0m"<<endl;
                                    defender.checkSunkShips(x, y);
                                } else {
                                    cout << "\n\033[31mMiss\033[0m"<<endl;
                                    currentPlayer = 3 - currentPlayer;
                                }
                                moveCount++;
                                if (defender.getBoard().isGameOver()) {
                                    cout << "\nAll ships sunk \033[36m" << attacker.getName() << "\033[0m wins"<<endl;
                                    cout << "Total moves: " << moveCount << endl;
                                    getKeyPress();
                                    return 0;
                                }
                                getKeyPress();
                            }
                        }
                    }
                    case 2: {
                    Bot bot1("Bot 1");
                    Bot bot2("Bot 2");
                
                    bot1.placeShips();
                    cout << "\nBot 1 ships placed. Press any key to continue...";
                    getKeyPress();
                    system("clear");
                
                    bot2.placeShips();
                    cout << "\nBot 2 ships placed. Press any key to start the game...";
                    getKeyPress();
                
                    int currentPlayer = 1;
                
                    int moveCount = 0;  

                    while (true) {
                        system("clear");
                        Bot& attacker = (currentPlayer == 1) ? bot1 : bot2;
                        Bot& defender = (currentPlayer == 1) ? bot2 : bot1;

                        cout << attacker.getName() << "'s Turn.\n";

                        Position target = attacker.chooseTarget();
                        cout << attacker.getName() << " attacks at (" << target.x << ", " << target.y << ")\n";

                        bool hit = defender.getBoard().attack(target.x, target.y);
                        if (hit) {
                            cout << "Hit!\n";
                            defender.checkSunkShips(target.x, target.y);
                            moveCount++; 

                            if (defender.getBoard().isGameOver()) {
                                cout << "\n\033[36m" << attacker.getName() << "\033[0m wins!\n";
                                cout << "Total moves: " << moveCount << endl;  
                                break;
                            }

                            continue;
                        } else {
                            cout << "Miss\n";
                        }

                        cout << "\n" << bot1.getName() << "'s Board:               " << bot2.getName() << "'s Board:\n";
                        for (int i = 0; i < 10; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                cout << bot1.getBoard().getKey(i, j) << " ";
                            }
                            cout << "         ";
                            for (int j = 0; j < 10; ++j) {
                                cout << bot2.getBoard().getKey(i, j) << " ";
                            }
                            cout << endl;
                        }
                        if (defender.getBoard().isGameOver()) {
                            cout << "\n\033[36m" << attacker.getName() << "\033[0m wins!\n";
                            cout << "Total moves: " << moveCount << endl; 
                            break;
                        }

                        currentPlayer = 3 - currentPlayer;

                        cout << "\nPress any key to continue...\n";
                        getKeyPress();
                    }
                }
                case 3:
                    cout << "Exiting the game...\n";
                    return 0;
            }
        } catch (const exception& e) {
            cout << "\n\033[31mAn error occurred: " << e.what() << "\033[0m\n";
            cout << "Press any key to return to main menu...\n";
            getKeyPress();
        }
    }
}
}
