#include "CardsSlots.h"


CardsSlots::CardsSlots()
{
}


CardsSlots::~CardsSlots()
{
}

void CardsSlots::Push(Card card)
{
	_Cards.push_back(card);
}

void CardsSlots::MoveCardsTo(CardsSlots & targetSlots, int first)
{
	std::vector<Card> & targetCards = targetSlots.GetCards();
	targetCards.insert(targetCards.end(), _Cards.begin() + first, _Cards.end());
	_Cards.erase(_Cards.begin() + first, _Cards.end());
	UpdateHideLevel();
}

void CardsSlots::Pop(int num)
{
	_Cards.erase(_Cards.end() - num, _Cards.end());
	UpdateHideLevel();
}

void CardsSlots::UpdateHideLevel()
{
	if (HideLevel > 0 && HideLevel == _Cards.size())
		HideLevel--;
}
