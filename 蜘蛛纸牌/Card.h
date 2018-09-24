#pragma once

#include "CardType.h"

class Card
{
private:
	CardValue	_Value;
	CardType	_Type;
public:
	Card() {}
	Card(CardValue value, CardType type)
		:_Value(value), _Type(type) {}
	~Card() {}

	CardValue GetValue() const
	{
		return _Value;
	}

	CardType GetType() const
	{
		return _Type;
	}

	bool operator<(const Card & card) const
	{
		return _Value < card._Value;
	}

	bool operator<(const CardValue & value) const
	{
		return _Value < value;
	}

};

