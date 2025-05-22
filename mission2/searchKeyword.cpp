#include "searchKeyword.h"
#include "SimilerMethodFactory.h"
#include <algorithm>

searchKeyword::searchKeyword()
{
	inputFileName = "file error \n";
	initializeVariables();
}

searchKeyword::searchKeyword(string filename)
{
	if (filename.empty())
	{
		inputFileName = "file error \n";
	}
	else  	inputFileName = filename;
	initializeVariables();
}

void searchKeyword::initializeVariables()
{
	// 사용할 변수 init 
	uint32_t index;
	for (index = 0; index < MAX_WEEKDAY_CNT; index++)
	{
		weekBest[index].clear();
	}
	for (index = 0; index < MAX_KINDOFDAY_CNT; index++)
	{
		twoBest[index].clear();
	}

	UZ = DEFAULT_UZ;

	dayConvertor = { "monday" ,"tuesday", "wednesday", "thursday" , "friday", "saturday", "sunday" };

}

string searchKeyword::inputSearchKeyword(string keyword, string day)
{
	uint32_t dayIndex = 0;
	uint32_t weekendIndex = 0;

	UZ += DEFAULT_POINT;

	if (keyword.empty())
	{
		return string("Keyword is vacant\n");
	}

	dayIndex = findDayIndex(day);

	//평일 / 주말	
	if (dayIndex >= 0 && dayIndex <= 4) weekendIndex = 0;
	else weekendIndex = 1;

	uint64_t point = UZ;

	//관리 목록에 존재하는지 확인
	//관리되는 키워드이면 점수가 증가
	if (true == checkKeywordMatch(keyword, dayIndex, weekendIndex))
	{
		return keyword;
	}

	//찰떡 HIT
	for (KeywordNode& node : weekBest[dayIndex])
	{
		if (similer(node.name, keyword)) {
			return node.name;
		}
	}

	//완벽 HIT / 찰떡 HIT 둘다 아닌경우
	if (weekBest[dayIndex].size() < MAX_NODE_CNT)
	{
		weekBest[dayIndex].push_back({ keyword, point });
	}
	else
	{
		if (weekBest[dayIndex].back().point < point)
		{
			weekBest[dayIndex].pop_back();
			weekBest[dayIndex].push_back({ keyword, point });
		}
	}
	std::sort(weekBest[dayIndex].begin(), weekBest[dayIndex].end(), greater<KeywordNode>());

	if (twoBest[weekendIndex].size() < MAX_NODE_CNT)
	{
		twoBest[weekendIndex].push_back({ keyword, point });
	}
	else
	{
		if (twoBest[weekendIndex].back().point < point)
		{
			twoBest[weekendIndex].pop_back();
			twoBest[weekendIndex].push_back({ keyword, point });
		}
	}
	std::sort(twoBest[weekendIndex].begin(), twoBest[weekendIndex].end(), greater<KeywordNode>());

	return keyword;
}

bool searchKeyword::similer(const std::string& a, const std::string& b)
{
	uint32_t dist = SimilerMethodFactory::getInstance()->getMethodeObjct("L")->run(a, b);
	uint32_t max_len = std::max(a.length(), b.length());

	// 유사도 비율 (1.0: 완전히 같음, 0.0: 전혀 다름)
	uint64_t similarity = max_len - dist;

	uint64_t score = max_len + (similarity * 99);

	if (score >= (80 * max_len)) return true;

	return false;
}

uint32_t searchKeyword::findDayIndex(string day)
{
	uint32_t index = 0;
	for (string str : dayConvertor)
	{
		if (str == day) break;
		index++;
	}

	return index;
}

void searchKeyword::reorderPoint(uint64_t max_weekday_point, uint64_t max_day_point)
{
	if (UZ >= MAX_NUMBER_OF_LIMIT || max_weekday_point >= MAX_NUMBER_OF_LIMIT || max_day_point >= MAX_NUMBER_OF_LIMIT)
	{
		UZ = DEFAULT_UZ;

		for (uint32_t i = 0; i < MAX_WEEKDAY_CNT; i++)
		{
			uint32_t num = DEFAULT_POINT;
			for (KeywordNode& node : weekBest[i])
			{
				node.point = num;
				num += DEFAULT_POINT;
			}
		}

		for (uint32_t i = 0; i < MAX_KINDOFDAY_CNT; i++)
		{
			uint32_t num = DEFAULT_POINT;
			for (KeywordNode& node : twoBest[i])
			{
				node.point = num;
				num += DEFAULT_POINT;
			}
		}
	}
}

bool searchKeyword::checkKeywordMatch(string keyword, uint32_t dayIndex, uint32_t weekendIndex)
{
	bool keywordMatchFlag = false;
	uint64_t max_weekday_point = 0;
	uint64_t max_day_point = 0;

	for (KeywordNode& wnode : weekBest[dayIndex])
	{
		if (wnode.name == keyword)
		{
			wnode.point += (wnode.point / DEFAULT_POINT);
			max_weekday_point = wnode.point;
			keywordMatchFlag = true;

			for (KeywordNode& tnode : twoBest[weekendIndex])
			{
				if (tnode.name == keyword)
				{
					tnode.point += (tnode.point / DEFAULT_POINT);
					max_day_point = tnode.point;
					break;
				}
			}
			break;
		}
	}
	reorderPoint(max_weekday_point, max_day_point);

	return keywordMatchFlag;
}

void searchKeyword::run() 
{
	ifstream fin{ inputFileName }; //500개 데이터 입력
	if (false == fin.is_open())
	{
		cout << "File open fail\n";
		return;
	}

	for (uint32_t i = 0; i < 500; i++)
	{
		string keyword, day;
		fin >> keyword >> day;
		string returnKeyWord = inputSearchKeyword(keyword, day);
		cout << returnKeyWord << "\n";
	}
}
