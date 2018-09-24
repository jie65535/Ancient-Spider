#pragma once
#include <vector>
#include <array>
#include "Card.h"
#include "View.h"
#include "CardsSlots.h"

// 难度
enum Difficulty
{
	// 简单
	easy,
	// 普通
	normal,
	// 困难
	hard,
};

// 牌槽数量
const int SlotsNum  = 10;

// 卡组数量
const int GroupsNum = 8;

// 预留数  预留的牌组数量 = 预留数 * 槽数量 = 5 * 10 = 50 可以发五次
const int ReservedQuantity = 5;

struct Point {
	int x, y;

	Point() 
		:x(0), y(0) 
	{}

	Point(int a, int b)
		:x(a), y(b)
	{}

	void SetValue(int a, int b)
	{
		x = a, y = b;
	}

	bool operator == (const Point p) const 
	{
		return x == p.x && y == p.y;
	}
};

enum Dir {
	left,
	up,
	right,
	down
};

class View;
class Game
{
private:
	// 本场游戏的所有牌
	std::array<Card, GroupsNum * CardValueNum> _Cards;

	// 游戏难度
	Difficulty _Difficulty;

	View *_pView;

	std::array<CardsSlots, SlotsNum> Scenes;

	// 预留发牌的次数
	int ReservedCount;

	// 成功完成组合的计数器
	int SuccessCount;

	Point PSelected;
	Point PCurrent;

	bool IsVictory;
public:
	friend class View;

	Game(Difficulty difficulty = Difficulty::easy);
	~Game();

	void Show();

	void SetDifficulty(Difficulty newDifficulty);
	Difficulty GetDifficulty() const { return _Difficulty; }

	bool GetIsVictory() const { return IsVictory; }

	// 洗牌
	void Shuffle();

	// 初始化场景
	void InitScenes();

	// 开始一场新游戏
	void StartNewGame();

	// 发牌（预留发牌次数足够的情况下才会发成功）
	void Deal();

public:
	// 光标移动
	void CurMove(Dir dir);

	// 选择当前光标指向的牌  返回是否选择成功
	bool Select();

private:
	void MoveCards(Point point, int SlotsIndex);

	bool CheckIsOptional(Point point);

	bool CheckIsOrderly(Card cur, Card next, Difficulty difficulty);

	bool CheckIsSuccess(int SlotsIndex);
};

