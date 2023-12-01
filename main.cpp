#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <fstream> // ファイル操作のためのヘッダーファイル

using namespace std;

const int MAP_SIZE = 10;
const int EMPTY = 0;
const int PLAYER = 1;
const int BASE = 2;
const int HUT = 3;
const int HOUSE = 4;
const int PARK = 5;
int POPULATION = 0;
int TAX = 10;

struct Player {
    int x;
    int y;
    int gold;
};

// セーブ関数
void saveGame(const int map[MAP_SIZE][MAP_SIZE], const Player& player) {
    ofstream saveFile("save.txt");

    // マップの状態をファイルに保存
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            saveFile << map[i][j] << " ";
        }
        saveFile << endl;
    }

    // プレイヤーの情報をファイルに保存
    saveFile << player.x << " " << player.y << " " << player.gold << endl;

    // その他のゲーム状態をファイルに保存
    saveFile << POPULATION << " " << TAX << endl;

    saveFile.close();
}

// ロード関数
void loadGame(int map[MAP_SIZE][MAP_SIZE], Player& player) {
    ifstream loadFile("save.txt");
    if (!loadFile) {
        // ロードファイルがない場合は新しいゲームを開始
        return;
    }

    // マップの状態をファイルから読み込み
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            loadFile >> map[i][j];
        }
    }

    // プレイヤーの情報をファイルから読み込み
    loadFile >> player.x >> player.y >> player.gold;

    // その他のゲーム状態をファイルから読み込み
    loadFile >> POPULATION >> TAX;

    loadFile.close();
}

void displayStats(const Player& player) {
    cout << "Gold: " << player.gold << endl;
    cout << "POPULATION: " << POPULATION << endl;
    cout << "TAX: " << TAX << endl;
}

void displayMap(const int map[MAP_SIZE][MAP_SIZE], const Player& player) {
    system("cls");
    displayStats(player);
    int pop = 0;
    int tax = 0;
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            if (i == player.y && j == player.x) {
                cout << "P ";
            } else if (map[i][j] == BASE) {
                cout << "B ";
            } else if (map[i][j] == HUT) {
                cout << "H ";
                pop++;
            } else if (map[i][j] == HOUSE) {
                cout << "h ";
                pop++;
            } else if (map[i][j] == PARK) {
                cout << "p ";
                tax++;
            } else {
                cout << map[i][j] << " ";
            }
        }
        cout << endl;
    }
    POPULATION = pop;
    TAX = tax + 10;
}

void movePlayer(Player& player, char direction) {
    switch (direction) {
        case 'w':
            if (player.y > 0) {
                player.y--;
            }
            break;
        case 'a':
            if (player.x > 0) {
                player.x--;
            }
            break;
        case 's':
            if (player.y < MAP_SIZE - 1) {
                player.y++;
            }
            break;
        case 'd':
            if (player.x < MAP_SIZE - 1) {
                player.x++;
            }
            break;
    }
}

void buildBuilding(Player& player, int map[MAP_SIZE][MAP_SIZE]) {
    if (player.gold >= 20 && map[player.y][player.x] == EMPTY) {
        player.gold -= 20;
        map[player.y][player.x] = BASE;
    }
}

void destroyBuilding(Player& player, int map[MAP_SIZE][MAP_SIZE]) {
    if (player.gold >= 10 && map[player.y][player.x] != EMPTY) {
        player.gold -= 10;
        map[player.y][player.x] = EMPTY;
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    int map[MAP_SIZE][MAP_SIZE];
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            map[i][j] = EMPTY;
        }
    }

    Player player;
    player.x = rand() % MAP_SIZE;
    player.y = rand() % MAP_SIZE;
    player.gold = 100;
    
    // ロード関数を呼び出し、保存されたデータがあればそれを読み込む
    loadGame(map, player);
    
    char input;
    do {
        displayMap(map, player);

        cout << "w/a/s/d to move\nq to quit\nb to build base\ne to destroy building\nz to save game-data";
        if (map[player.y][player.x] == BASE)
            cout << "\n1 to build hut(1*1)\n2 to build house(2*1)\n3 to build park(2*2)";
        input = _getch();

        if (input != 'q') {
            if (input == 'b') {
                buildBuilding(player, map);
            } else if (input == 'z') {
                saveGame(map, player);
            } else if (input == 'e') {
                destroyBuilding(player, map);
            } else {
                movePlayer(player, input);

                if (rand() % 5 == 0) {
                    player.gold += TAX * POPULATION;
                }

                displayStats(player);
            }
            if(map[player.y][player.x] == BASE)
            {
                if (player.gold >= 10 &&input == '1'){
                    player.gold-=10;
                    map[player.y][player.x] = HUT;
                }
                if (player.gold >= 20 &&input == '2'&&map[player.y][player.x] == BASE&&map[player.y+1][player.x] == BASE){
                    player.gold-=20;
                    map[player.y][player.x] = HOUSE;
                    map[player.y+1][player.x] = HOUSE;
                }
                if (player.gold >= 50 &&input == '3'&&map[player.y][player.x] == BASE&&map[player.y+1][player.x] == BASE&&map[player.y][player.x+1] == BASE&&map[player.y+1][player.x+1] == BASE){
                    player.gold-=50;
                    map[player.y][player.x] = PARK;
                    map[player.y+1][player.x] = PARK;
                    map[player.y][player.x+1] = PARK;
                    map[player.y+1][player.x+1] = PARK;
                }
            }
        }

    } while (input != 'q');

    return 0;
}
