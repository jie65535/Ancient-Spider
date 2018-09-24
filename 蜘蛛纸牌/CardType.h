#pragma once



// 卡牌类型
enum CardType {
	// 黑桃
	Spade,

	// 红桃
	Heart,

	// 梅花
	Club,

	// 方块
	Diamond
};

const int CardTypeNum = 4;

// 卡牌的值
enum CardValue {
	_A,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,
	_10,
	_J,
	_Q,
	_K,
};

const int CardValueNum = 13;