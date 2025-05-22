#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class searchKeyword
{
public:
	searchKeyword();
	searchKeyword(string filename);
	void run();	

protected:
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

	void initializeVariables();
	string inputSearchKeyword(string keyword, string day);
	bool similer(const std::string& a, const std::string& b);
	uint32_t findDayIndex(string day);
	void reorderPoint(uint64_t max_weekday_point, uint64_t max_day_point);
	bool checkKeywordMatch(string keyword, uint32_t dayIndex, uint32_t weekendIndex);

	const uint32_t  DEFAULT_UZ = 90;
	static const uint32_t  MAX_WEEKDAY_CNT = 7;
	static const uint32_t  MAX_KINDOFDAY_CNT = 2;
	const uint32_t  DEFAULT_POINT = 10;
	const uint32_t  MAX_NODE_CNT = 10;
	const uint64_t MAX_NUMBER_OF_LIMIT = 21000000000;

	vector<KeywordNode> weekBest[MAX_WEEKDAY_CNT]; //월 ~ 금
	vector<KeywordNode> twoBest[MAX_KINDOFDAY_CNT]; //평일, 주말
	int UZ; // dafault 9 ;
	string inputFileName;
	vector<string>dayConvertor;

};
