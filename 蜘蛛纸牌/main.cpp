#include <iostream>
#include <conio.h>
using namespace std;
#include "Game.h"

void showGame();
bool input();

Game game;

int main()
{
	game.StartNewGame();

	while (true)
	{
		showGame();
		while(!input())
			;
		if (game.GetIsVictory())
		{
			system("cls");
			std::cout << "游戏胜利！按任意键开始新游戏" << std::endl;
			game.StartNewGame();
		}
	}
	getchar();
    return 0;
}

void showGame()
{
	system("cls");
	game.Show();
	std::cout	<< "\n\n\n"
				<< "[w] [s] [a] [d] 移动光标   [e] 发牌  [r] 开始新游戏  [q] 退出游戏" << std::endl
				<< "[1] 简单难度  [2] 普通难度  [3] 困难难度" << std::endl;
}

bool input()
{
	switch (_getch())
	{
	case 'w':
	case 'W':
		game.CurMove(Dir::up);
		break;
	case 's':
	case 'S':
		game.CurMove(Dir::down);
		break;
	case 'a':
	case 'A':
		game.CurMove(Dir::left);
		break;
	case 'd':
	case 'D':
		game.CurMove(Dir::right);
		break;
	case ' ':
		game.Select();
		break;
	case 'e':
	case 'E':
		game.Deal();
		break;
	case 'r':
	case 'R':
		game.StartNewGame();
		break;
	case '1':
		game.SetDifficulty(Difficulty::easy);
		game.StartNewGame();
		break;
	case '2':
		game.SetDifficulty(Difficulty::normal);
		game.StartNewGame();
		break;
	case '3':
		game.SetDifficulty(Difficulty::hard);
		game.StartNewGame();
		break;
	case 'q':
		exit(0);
	default:
		return false;
		break;
	}
	return true;
}