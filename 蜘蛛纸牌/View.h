#pragma once

#include "Card.h"
#include "CardType.h"
#include "Game.h"

extern const char *CardTypeView[];
extern const char *CardValueView[];

class Game;
class View
{
private:
	Game &_Game;

public:
	View(Game &game) :_Game(game) {}
	~View();

	void ShowCard(bool isNull);
	void ShowCard(Card card);

	void ShowGame();
};

