#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
    ifstream file(fileName);
    bool door_exit= false;
    //check if file does not exist
    if(!file.is_open()){
        return nullptr;
    }

    if(!file.good()){
        return nullptr;
    }

    file >> maxRow;
    //check if maxrow fail
    if(file.fail()){
        return nullptr;
    }
    file >> maxCol;
    //check if maxCol fail
    if(file.fail()){
        return nullptr;
    }
    //check if max row and max col is greater than 0
    if(maxRow <= 0 || maxRow <= 0 || maxCol >= INT32_MAX/maxRow){
        return nullptr;
    }
    file >> player.row;
    //check player.row fails
    if(file.fail()){
        return nullptr;
    }
    file >> player.col;
    //check if player.col fails
    if(file.fail()){
        return nullptr;
    }
    //check if player row and column is inside max row/col
    if(player.row >= maxRow || player.col >= maxCol || player.row < 0 || player.col < 0){
        return nullptr;
    }

    char** map = createMap(maxRow,maxCol);
    for(int i = 0; i < maxRow; i++){
        for(int j = 0; j < maxCol; j++){
            file >> map[i][j];
            //check if any of them fail
            if(file.fail()){
                deleteMap(map,maxRow);
                return nullptr;
            }
            char temp = map[i][j];
            if(!(temp == TILE_MONSTER || temp == TILE_OPEN || temp == TILE_PILLAR || temp == TILE_TREASURE || temp == TILE_AMULET || temp == TILE_DOOR || temp == TILE_EXIT)){
                deleteMap(map,maxRow);
                return nullptr;
            }
            if(map[i][j] == TILE_DOOR || map[i][j] == TILE_EXIT){
                door_exit = true;
            }
        }
    }
    if(door_exit == false){
        deleteMap(map,maxRow);
        return nullptr;
    }
    map[player.row][player.col] = TILE_PLAYER;
    return map;
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    //move up
    if(input == MOVE_UP){
        nextRow = nextRow - 1;
    }
    if(input == MOVE_DOWN){
        nextRow = nextRow + 1;
    }

    if(input == MOVE_RIGHT){
        nextCol += 1;
    }

    if(input == MOVE_LEFT){
        nextCol -=1;
    }
    
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {
    if(maxRow <= 0|| maxCol <= 0){
        return nullptr;
    }
    else{
        char** map = new char*[maxRow];
        for(int row = 0; row < maxRow; row++){
            map[row] = new char[maxCol];
        }
        for(int i = 0; i < maxRow ; i++){
            for(int j = 0; j < maxCol; j++){
                map[i][j] = TILE_OPEN;
            }
        }
        return map;
    }
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    if(map != nullptr){
        for(int i = 0; i < maxRow; i++){
            delete[] map[i];
        }
    delete[] map;
    }
    maxRow = 0;
    map = nullptr;
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    int initial_row = maxRow;
    int initial_col = maxCol;

    if(maxRow <= 0 || maxCol <= 0 || map == nullptr){
        return nullptr;
    }

    maxRow *=2;
    maxCol *=2;
   
    char** new_map = createMap(maxRow,maxCol);

    //populate initial square
    for(int i =0; i < initial_row; i++){
        for(int j = 0; j < initial_col; j++){
            new_map[i][j] = map[i][j];
        }
    }
    //square to the right
    for(int i = 0 ; i < initial_row; i++){
        for(int j = initial_col; j < maxCol; j++){
            new_map[i][j] = map[i][j - initial_col];

            if(new_map[i][j] == 'o'){
                new_map[i][j] = '-';
            }
        }
    }

    //under
    for(int i = initial_row; i < maxRow; i++){
        for(int j = 0; j < initial_col; j++){
            new_map[i][j] = map[i - initial_row][j];

            if(new_map[i][j] == 'o'){
                new_map[i][j] = '-';
            }
        }
    }
    //diagonal from
    for(int i =initial_row; i < maxRow; i++){
        for(int j = initial_col; j < maxCol; j++){
            new_map[i][j] = map[i - initial_row][j - initial_col];

            if(new_map[i][j] == 'o'){
                new_map[i][j] = '-';
            }
        }
    }

    deleteMap(map,initial_row);
    return new_map;
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    if(nextRow == player.row && nextCol == player.col){
        return STATUS_STAY;
    }
    else if(nextCol >= maxCol || nextRow >= maxRow){
        return STATUS_STAY;
    }
    else if(nextCol < 0 || nextRow < 0){
        return STATUS_STAY;
    }
    else if(map[nextRow][nextCol] == TILE_PILLAR || map[nextRow][nextCol] == TILE_MONSTER){
        return STATUS_STAY;
    }
    else if(map[nextRow][nextCol] == TILE_TREASURE){
        map[player.row][player.col] = TILE_OPEN;
        map[nextRow][nextCol] = TILE_PLAYER;
        player.row = nextRow;
        player.col = nextCol;
        player.treasure++;
        return STATUS_TREASURE;
    }
    else if(map[nextRow][nextCol] == TILE_AMULET){
        map[player.row][player.col] = TILE_OPEN;
        map[nextRow][nextCol] = TILE_PLAYER;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_AMULET;
    }
    else if(map[nextRow][nextCol] == TILE_DOOR){
        map[player.row][player.col] = TILE_OPEN;
        map[nextRow][nextCol] = TILE_PLAYER;
        player.row = nextRow;
        player.col = nextCol;
        map[player.row][player.col] = TILE_PLAYER;
        return STATUS_LEAVE;
    }
    else if(map[nextRow][nextCol] == TILE_EXIT){
        if(player.treasure <= 0){
            return STATUS_STAY;
        }
        else{
            map[player.row][player.col] = TILE_OPEN;
            map[nextRow][nextCol] = TILE_PLAYER;
            player.row = nextRow;
            player.col = nextCol;
            return STATUS_ESCAPE;
        }
    
    }
    else{
        map[player.row][player.col] = TILE_OPEN;
        map[nextRow][nextCol] = TILE_PLAYER;
        player.row = nextRow;
        player.col = nextCol;
        return STATUS_MOVE;
    }
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {

    //check blocks above player
    for(int i = player.row - 1; i >=0; i--){
        if(map[i][player.col] == TILE_PILLAR){
            break;
        }
        if(map[i][player.col] == TILE_MONSTER){
            map[i][player.col] = TILE_OPEN;
            map[i + 1][player.col] = TILE_MONSTER;
        } 
    }
    //check underneath player
    for(int i = player.row + 1; i < maxRow; i++){
        if(map[i][player.col] == TILE_PILLAR){
            break;
        }
        if(map[i][player.col] == TILE_MONSTER){
            map[i][player.col] = TILE_OPEN;
            map[i - 1][player.col] = TILE_MONSTER;
        } 
    }
    //check left of the player
    for(int i = player.col - 1; i >= 0; i--){
        if(map[player.row][i] == TILE_PILLAR){
            break;
        }
        if(map[player.row][i] == TILE_MONSTER){
            map[player.row][i] = TILE_OPEN;
            map[player.row][i+1] = TILE_MONSTER;
        } 
    }
    //check right of the player
    for(int i = player.col + 1; i < maxCol; i++){
        if(map[player.row][i] == TILE_PILLAR){
            break;
        }
        if(map[player.row][i] == TILE_MONSTER){
            map[player.row][i] = TILE_OPEN;
            map[player.row][i-1] = TILE_MONSTER;
        } 
    }
    if(map[player.row][player.col] == TILE_MONSTER){
        return true;
    }
    else{
        return false;
    }
}
