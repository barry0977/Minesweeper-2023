#ifndef SERVER_H
#define SERVER_H

#include<cstdlib>
#include<iostream>

/*
 * You may need to define some global variables for the information of the game map here.
 * Although we don't encourage to uss global variables in real cpp projects, you may have to use them because the use of
 * class is not taught yet. However, if you are member of A-class or have learnt the use of cpp class, member functions,
 * etc., you're free to modify this structure.
 */
int rows; // The count of rows of the game map
int columns;  // The count of columns of the game map
int game_state; // The state of the game, 0 for continuing, 1 for winning, -1 for losing
char map[35][35], show[35][35];
int step_taken = 0;//记录所用步数
/**
 * @brief The definition of function InitMap()
 *
 * @details This function is designed to read the initial map from stdin. For example, if there is a 3 * 3 map in which
 * mines are located at (0, 1) and (1, 2) (0-based), the stdin would be
 *     3 3
 *     .X.
 *     ...
 *     ..X
 * where X stands for a mine block and . stands for a normal block. After executing this function, your game map would
 * be initialized, with all the blocks unvisited.
 */
void InitMap()
{
    std::cin >> rows >> columns;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            std::cin >> map[i][j];
            show[i][j] = '?';
        }
    }
}

/**
 * @brief The definition of function VisitBlock(int, int)
 *
 * @details This function is designed to visit a block in the game map. We take the 3 * 3 game map above as an example.
 * At the beginning, if you call VisitBlock(0, 0), the return value would be 0 (game continues), and the game map would
 * be
 *     1??
 *     ???
 *     ???
 * If you call VisitBlock(0, 1) after that, the return value would be -1 (game ends and the players loses) , and the
 * game map would be
 *     1X?
 *     ???
 *     ???
 * If you call VisitBlock(0, 2), VisitBlock(2, 0), VisitBlock(1, 2) instead, the return value of the last operation
 * would be 1 (game ends and the player wins), and the game map would be
 *     1@1
 *     122
 *     01@
 *
 * @param row The row coordinate (0-based) of the block to be visited.
 * @param column The column coordinate (0-based) of the block to be visited.
 *
 * @note You should edit the value of game_state in this function. Precisely, edit it to
 *    0  if the game continues after visit that block, or that block has already been visited before.
 *    1  if the game ends and the player wins.
 *    -1 if the game ends and the player loses.
 */
void VisitBlock(int row, int column)
{
    int number = 0;//用于记录周围格子的地雷数
    bool isWin = 1;//用于判断是否获胜
    bool isVisit = 1;//用于判断格子是否被访问过
    step_taken += 1;
    if (show[row][column] >= '0' && show[row][column] <= '8')
    {
        isVisit = 0;
    }
    if (isVisit == 1)
    {
        if (map[row][column] == 'X')
        {
            game_state = -1;
            show[row][column] = 'X';
        }
        else
        {
            int i, j, i1, i2, j1, j2;//判断该坐标是否位于边界处
            i1 = ((row - 1) >= 0) ? (row - 1) : 0;
            i2 = ((row + 1) <= (rows - 1)) ? (row + 1) : (rows - 1);
            j1 = ((column - 1) >= 0) ? (column - 1) : 0;
            j2 = ((column + 1) <= (columns - 1)) ? (column + 1) : (columns - 1);
            for (i = i1; i <= i2; i++)
            {
                for (j = j1; j <= j2; j++)
                {
                    if (map[i][j] == 'X')
                    {
                        number++;
                    }
                }
            }
            show[row][column] = number + '0';
            if (number == 0)//如果是0，则继续访问周围一圈格子
            {
                for (int i = i1; i <= i2; i++)
                {
                    for (int j = j1; j <= j2; j++)
                    {
                        if (show[i][j] != '?')
                        {
                            continue;
                        }
                        VisitBlock(i, j);
                        step_taken -= 1;
                    }
                }
            }
            game_state = 0;
            for (int i = 0; i < rows; i++)//判断是否所有不是地雷的格子都已访问完
            {
                for (int j = 0; j < columns; j++)
                {
                    if (show[i][j] < '0' || show[i][j] > '8')
                    {
                        if (map[i][j] != 'X')
                        {
                            isWin = 0;
                        }
                    }
                }
            }
            if (isWin == 1)//所有格子都访问完，输出全部地图并改变游戏状态
            {
                game_state = 1;
                for (int i = 0; i < rows; i++)
                    for (int j = 0; j < columns; j++)
                    {
                        if (map[i][j] == 'X')
                        {
                            show[i][j] = '@';
                        }
                    }
            }
        }
    }
}

/**
 * @brief The definition of function PrintMap()
 *
 * @details This function is designed to print the game map to stdout. We take the 3 * 3 game map above as an example.
 * At the beginning, if you call PrintMap(), the stdout would be
 *    ???
 *    ???
 *    ???
 * If you call VisitBlock(2, 0) and PrintMap() after that, the stdout would be
 *    ???
 *    12?
 *    01?
 * If you call VisitBlock(0, 1) and PrintMap() after that, the stdout would be
 *    ?X?
 *    12?
 *    01?
 * If the player visits all blocks without mine and call PrintMap() after that, the stdout would be
 *    1@1
 *    122
 *    01@
 * (You may find the global variable game_state useful when implementing this function.)
 *
 * @note Use std::cout to print the game map, especially when you want to try the advanced task!!!
 */
void PrintMap()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            std::cout << show[i][j];
        }
        std::cout << "\n";
    }
}

/**
 * @brief The definition of function ExitGame()
 *
 * @details This function is designed to exit the game.
 * It outputs a line according to the result, and a line of two integers, visit_count and step_count,
 * representing the number of blocks visited and the number of steps taken respectively.
 */
void ExitGame()
{
    int block_visited = 0;//用于记录访问的格子数
    if (game_state == 1)
    {
        std::cout << "YOU WIN!\n";
    }
    else
    {
        std::cout << "GAME OVER!\n";
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (show[i][j] >= '0' && show[i][j] <= '8')
            {
                block_visited++;
            }
        }
    }
    std::cout << block_visited << " " << step_taken;
    exit(0); // Exit the game immediately
}

#endif
