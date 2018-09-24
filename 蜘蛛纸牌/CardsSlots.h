#pragma once
#include <vector>
#include "Card.h"

class CardsSlots
{
private:
	std::vector<Card> _Cards;
	int HideLevel;
public:
	CardsSlots();
	~CardsSlots();

	int GetHideLevel() const { return HideLevel; }
	void SetHideLevel(int level) { HideLevel = level; }

	size_t Count() const { return _Cards.size(); }

	void Clear() { _Cards.clear(); }

	void Push(Card card);
	
	void MoveCardsTo(CardsSlots &targetSlots, int first);

	void Pop(int num);

 	std::vector<Card> & GetCards() { return _Cards; }
private:
	void UpdateHideLevel();
};

