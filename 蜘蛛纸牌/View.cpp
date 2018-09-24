#include "View.h"
#include <iostream>

const char *CardTypeView[] = {
	"黑桃",
	"红心",
	"梅花",
	"方块",
};

const char *CardValueView[] = {
	" A"," 2"," 3"," 4"," 5"," 6"," 7"," 8"," 9","10"," J"," Q"," K"
};


View::~View()
{
}

void View::ShowCard(bool isNull)
{
	std::cout << (isNull ? "      " : "XXXXXX");
}
void View::ShowCard(Card card)
{
	std::cout << CardTypeView[card.GetType()] << CardValueView[card.GetValue()];
}

void View::ShowGame()
{
	bool flag = true;
	char lc = '[', rc = ']';
	for (int level = 0; flag; ++level)
	{
		flag = false;
		for (int i = 0; i < _Game.Scenes.size(); ++i)
		{
			if (_Game.PSelected.y == level && _Game.PSelected.x == i)
				lc = rc = '*';
			else if (_Game.PCurrent.y == level && _Game.PCurrent.x == i)
				lc = '>', rc = '<';
			else
				lc = '[', rc = ']';

			// 如果是还没翻开的槽
			if (level < _Game.Scenes[i].GetHideLevel())
			{
				std::cout << lc;
				ShowCard(false);
				std::cout << rc;
				flag = true;
			}
			// 否则如果是有牌的
			else if (level < _Game.Scenes[i].Count())
			{
				std::cout << lc;
				ShowCard(_Game.Scenes[i].GetCards()[level]);
				std::cout << rc;
				flag = true;
			}
			// 否则如果是空的槽
			else if (level == 0 && _Game.Scenes[i].Count() == 0)
			{
				// 如果光标正在这个槽上，出现光标
				if (_Game.PCurrent.y == level && _Game.PCurrent.x == i)
					lc = '>', rc = '<';
				else
					lc = ' ', rc = ' ';
				std::cout << lc;
				ShowCard(true);
				std::cout << rc;
			}
			// 其它的全部输出空
			else
			{
				std::cout << ' ';
				ShowCard(true);
				std::cout << ' ';
			}
		}
		std::cout << std::endl;
	}

	std::cout << "\n\n\n当前已经完成：" << _Game.SuccessCount << "组\t剩余发牌次数：" << _Game.ReservedCount << "次" << std::endl;
}
