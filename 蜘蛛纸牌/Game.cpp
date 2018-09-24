#include "Game.h"
#include <random>

Game::Game(Difficulty difficulty) :_pView(new View(*this))
{
	SetDifficulty(difficulty);
}

Game::~Game()
{
}


void Game::Show()
{
	_pView->ShowGame();
}

void Game::SetDifficulty(Difficulty newDifficulty)
{
	_Difficulty = newDifficulty;
	switch (newDifficulty)
	{
	case easy:
		for (int i = 0; i < GroupsNum; ++i)
		{
			for (int j = 0; j < CardValueNum; ++j)
			{
				_Cards[i * CardValueNum + j] = Card((CardValue)j, CardType::Spade);
			}
		}
		break;
	case normal:
		for (int i = 0; i < GroupsNum; ++i)
		{
			for (int j = 0; j < CardValueNum; ++j)
			{
				_Cards[i * CardValueNum + j] = Card((CardValue)j, j % 2 ? CardType::Heart : CardType::Spade);
			}
		}
		break;
	case hard:
		for (int i = 0; i < GroupsNum; ++i)
		{
			for (int j = 0, k = 0; j < CardValueNum; ++j, k = (k + 1) % 4)
			{
				_Cards[i * CardValueNum + j] = Card((CardValue)j, (CardType)k);
			}
		}
		break;
	default:
		break;
	}
}


void Game::Shuffle()
{
 	static std::random_device rd;
	std::default_random_engine e1(rd());
	std::uniform_int_distribution<int> uniform_dist(0, _Cards.size() - 1);
	for (int i = 0; i < _Cards.size(); ++i)
	{
		std::swap(_Cards[i], _Cards[uniform_dist(e1)]);
	}
}

void Game::InitScenes()
{
	ReservedCount = ReservedQuantity;
	SuccessCount = 0;
	IsVictory = false;
	for (int i = 0; i < Scenes.size(); ++i)
		Scenes[i].Clear();

	for (int i = ReservedQuantity * Scenes.size(), j = 0; i < _Cards.size(); ++i, j = (j+1) % Scenes.size())
		Scenes[j].Push(_Cards[i]);
	
	for (int i = 0; i < Scenes.size(); ++i)
		Scenes[i].SetHideLevel(Scenes[i].Count() - 1);
	
	PSelected.SetValue(-1, -1);
	PCurrent.SetValue(0, Scenes[0].Count() - 1);
}

void Game::StartNewGame()
{
	Shuffle();
	InitScenes();
}

void Game::Deal()
{
	if (ReservedCount < 1)
		return;

	// 将之前选中的取消掉
	PSelected.SetValue(-1, -1);

	// 为每一个槽发一张牌
	for (int i = 0; i < Scenes.size(); ++i)
	{
		Scenes[i].Push(_Cards[(ReservedCount-1) * Scenes.size() + i]);
	}
	// 次数减一
	ReservedCount--;
}

void Game::CurMove(Dir dir)
{
	int width = Scenes.size();
	int height = Scenes[PCurrent.x].Count();
	switch (dir)
	{
	case left:
		if (PCurrent.x == 0)
			PCurrent.x = width -1;
		else
			PCurrent.x--;
		break;
	case up:
		if (height == 0)
			PCurrent.y = 0;
		else if (PCurrent.y == 0)
			PCurrent.y = height - 1;
		else
			PCurrent.y--;
		break;
	case right:
		if (PCurrent.x == width - 1)
			PCurrent.x = 0;
		else
			PCurrent.x++;
		break;
	case down:
		if (height == 0)
			PCurrent.y = 0;
		else if (PCurrent.y == height - 1)
			PCurrent.y = 0;
		else
			PCurrent.y++;
		break;
	default:
		break;
	}

	// 如果现在这一列没任何牌  指向底
	if (Scenes[PCurrent.x].Count() == 0)
		PCurrent.y = 0;
	// 否则如果光标高度高于当前高度
	//     或者之前的高度是0
	//     或者之前在上一列的顶部时
	//  将光标指向当前列的顶部
	else if (PCurrent.y >= Scenes[PCurrent.x].Count()
		|| height == 0
		|| PCurrent.y == height - 1)
		PCurrent.y = Scenes[PCurrent.x].Count() - 1;
}

bool Game::Select()
{
	if (PCurrent.x < 0 || PCurrent.x >= Scenes.size())
		return false;

	if (Scenes[PCurrent.x].Count() == 0 && PCurrent.y != 0)
		return false;
	
	// 如果选中之前选的，则取消选中
	if (PSelected == PCurrent)
	{
		PSelected.SetValue(-1, -1);
		return true;
	}

	// 如果之前未选中任何东西
	if (PSelected.x == -1)
	{
		// 判断当前选的这个可不可以选
		if (CheckIsOptional(PCurrent))
		{
			// 如果可以选就选中这个位置，返回true
			PSelected = PCurrent;
			return true;
		}
		else
		{
			return false;
		}
	}
	// 否则说明之前选中了东西，尝试移动
	else
	{
		// 如果新选的和之前选的是同一列 或者 选的是不是中间的牌
		if (PCurrent.x == PSelected.x || PCurrent.y != Scenes[PCurrent.x].Count() - 1)
		{
			// 如果这一列啥都没有
			if (Scenes[PCurrent.x].Count() == 0)
			{
				// 直接移动过去
				MoveCards(PSelected, PCurrent.x); 
				PSelected.SetValue(-1, -1);
				return true;
			}
			if (CheckIsOptional(PCurrent))
			{
				PSelected = PCurrent;
				return true;
			}
			else
			{
				return false;
			}
		}
		// 否则说明选的是其它列的最后一张
		else
		{
			// 如果可以移动过去
			if (CheckIsOrderly(
				Scenes[PCurrent.x].GetCards().at(PCurrent.y),
				Scenes[PSelected.x].GetCards().at(PSelected.y),
				_Difficulty))
			{
				// 直接移动过去
				MoveCards(PSelected, PCurrent.x);
				PSelected.SetValue(-1, -1);
				return true;
			}
			// 否则选中这一张牌
			else
			{
				PSelected = PCurrent;
				return true;
			}
		}
	}
}

void Game::MoveCards(Point point, int SlotsIndex)
{
	// 将指定位置的牌移动到对应的槽中
	Scenes[point.x].MoveCardsTo(Scenes[SlotsIndex], point.y);

	// 检测是否完成
	if (CheckIsSuccess(SlotsIndex))
	{
		// 如果完成一组，则弹出这一组排
		Scenes[SlotsIndex].Pop(CardValueNum);
		// 然后将完成计数自增
		SuccessCount++;

		if (SuccessCount == GroupsNum)
			IsVictory = true;
	}
}


bool Game::CheckIsOptional(Point point)
{
	// 如果这一列啥都没有
	if (Scenes[point.x].Count() == 0)
		return false;

	if (point.y < Scenes[point.x].GetHideLevel())
		return false;

	if (point.y == Scenes[point.x].Count() - 1)
		return true;

	
	for (int i = point.y; i < Scenes[point.x].Count() - 1; ++i)
	{
		// 检查是否有序，只要有一个是错误的，那直接无法选中
		if (!CheckIsOrderly(Scenes[point.x].GetCards().at(i), Scenes[point.x].GetCards().at(i + 1), _Difficulty))
		{
			return false;
		}
	}

	return true;
}

bool Game::CheckIsOrderly(Card cur, Card next, Difficulty difficulty)
{
	if (cur.GetValue() != next.GetValue() + 1)
		return false;

	// 根据难度检查花色
	switch (difficulty)
	{
	// 简单难度不看花色
	case easy:
		return true;

	// 普通难度要看是否黑红相间
	case normal:
		return cur.GetType() == CardType::Spade && next.GetType() == CardType::Heart
			|| cur.GetType() == CardType::Heart && next.GetType() == CardType::Spade;
	
	// 困难难度要看是否按照黑桃、红桃、梅花、方块的顺序来
	case hard:
		return cur.GetType() == CardType::Spade		&& next.GetType() == CardType::Heart
			|| cur.GetType() == CardType::Heart		&& next.GetType() == CardType::Club
			|| cur.GetType() == CardType::Club		&& next.GetType() == CardType::Diamond
			|| cur.GetType() == CardType::Diamond	&& next.GetType() == CardType::Spade;
	default:
		break;
	}
	return false;
}

// 检查这一个槽是否完成
bool Game::CheckIsSuccess(int SlotsIndex)
{
	// 先判断这个槽的数量足不足够完成一副牌
	if (Scenes[SlotsIndex].Count() < CardValueNum)
		return false;

	// 如果显示出来的数量不足完成一副牌，直接返回
	if (Scenes[SlotsIndex].Count() - Scenes[SlotsIndex].GetHideLevel() < CardValueNum)
		return false;

	std::vector<Card> Cards = Scenes[SlotsIndex].GetCards();
	// 然后判断最下面那张牌是不是A，如果不是直接返回
	if (Cards.back().GetValue() != CardValue::_A)
		return false;

	// 然后检查倒数一副牌数量的位置开始是否有序，直接返回检查结果
	return CheckIsOptional(Point(SlotsIndex, Cards.size() - CardValueNum));
}
