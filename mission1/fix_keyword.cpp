#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

const uint32_t  DEFAULT_UZ = 90;
const uint32_t  MAX_WEEKDAY_CNT = 7;
const uint32_t  MAX_KINDOFDAY_CNT = 2;
const uint32_t  DEFAULT_POINT = 10;
const uint32_t  MAX_NODE_CNT = 10;
const uint64_t MAX_NUMBER_OF_LIMIT = 21000000000;
// Struct 
struct KeywordNode
{
	string name;
	uint64_t point;

	bool operator>(const KeywordNode& other) const
	{
		return point > other.point;
	}
};

// 사용되어야 하는  변수 선언 
vector<KeywordNode> weekBest[MAX_WEEKDAY_CNT]; //월 ~ 금
vector<KeywordNode> twoBest[MAX_KINDOFDAY_CNT]; //평일, 주말
int UZ; // dafault 9 ;
string inputFileName;
vector<string>dayConvertor;


// 레벤슈타인 거리 계산 알고리즘 (문자열 유사도 검사)
uint32_t levenshtein(const std::string& a, const std::string& b)
{
	const size_t len_a = a.size();
	const size_t len_b = b.size();

	std::vector<std::vector<uint32_t>> d(len_a + 1, std::vector<uint32_t>(len_b + 1));

	for (size_t i = 0; i <= len_a; ++i) d[i][0] = i;
	for (size_t j = 0; j <= len_b; ++j) d[0][j] = j;

	for (size_t i = 1; i <= len_a; ++i) {
		for (size_t j = 1; j <= len_b; ++j) {
			if (a[i - 1] == b[j - 1])
				d[i][j] = d[i - 1][j - 1];
			else
				d[i][j] = 1 + std::min({ d[i - 1][j], d[i][j - 1], d[i - 1][j - 1] });
		}
	}
	return d[len_a][len_b];
}

// 점수 환산
bool similer(const std::string& a, const std::string& b)
{
	uint32_t dist = levenshtein(a, b);
	uint32_t max_len = std::max(a.length(), b.length());

	// 유사도 비율 (1.0: 완전히 같음, 0.0: 전혀 다름)
	uint64_t similarity = max_len - dist;

	uint64_t score = max_len + (similarity * 99);

	if (score >= (80 * max_len)) return true;

	return false;
}

uint32_t findDayIndex(string day)
{
	uint32_t index = 0;
	for (string str : dayConvertor)
	{
		if (str == day) break;
		index++;
	}

	return index;
}

//재정렬 작업
void reorderPoint(uint64_t max_weekday_point, uint64_t max_day_point)
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

bool checkKeywordMatch(string keyword, uint32_t dayIndex, uint32_t weekendIndex)
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

string inputSearchKeyword(string keyword, string day)
{
	uint32_t dayIndex = 0;
	uint32_t weekendIndex = 0;

	UZ += DEFAULT_POINT;
	if (keyword.empty())
	{
		return "Keyword is vacant\n";
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


void searchKeyword()
{
	ifstream fin{ inputFileName }; //500개 데이터 입력
	if (false == fin.is_open())
	{
		cout << "File open fail\n";
		return;
	}

	for (int i = 0; i < 500; i++)
	{
		string keyword, day;
		fin >> keyword >> day;
		string returnKeyWord = inputSearchKeyword(keyword, day);
		cout << returnKeyWord << "\n";
	}
}

void initializeVariables()
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

	inputFileName = "keyword_weekday_500.txt";

	dayConvertor = { "monday" ,"tuesday", "wednesday", "thursday" , "friday", "saturday", "sunday" };

}

int main()
{
	initializeVariables();

	// 기능 실행
	searchKeyword();
}
