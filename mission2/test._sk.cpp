#include "gmock/gmock.h"
#include "searchKeyword.h"

#include "SimilerMethodFactory.h"

using namespace testing;

class searchKeywordfotT : public searchKeyword
{
public:
	string getFilename()
	{
		return inputFileName;
	}
	searchKeywordfotT()
	{
		inputFileName = "file error \n";
	}
	searchKeywordfotT(string filename)
	{

		if (filename.empty())
		{
			inputFileName = "file error \n";
		}
		else  inputFileName = filename;
		initializeVariables();
	}
	string inputSearchKeyword1(string keyword, string day)
	{
		return inputSearchKeyword(keyword, day);
	}
private:
};

TEST(SK, T1)
{
	string inName = "keyword_weekday_500.txt";
	searchKeywordfotT sk(inName);

	string fileName = sk.getFilename();

	EXPECT_EQ(fileName, inName);

	
	searchKeywordfotT sk1("");
	fileName = sk1.getFilename();
	EXPECT_EQ(fileName, "file error \n");

	searchKeywordfotT sk2;
	fileName = sk2.getFilename();
	EXPECT_EQ(fileName, "file error \n");

}

TEST(SK, T2)
{
	SimilerMethodFactory* a = SimilerMethodFactory::getInstance();
	SimilerMethodFactory* b = SimilerMethodFactory::getInstance();
	
	EXPECT_EQ(a, b);
}

TEST(SK, T3)
{
	uint32_t a = SimilerMethodFactory::getInstance()->getMethodeObjct("L")->run("aa", "ab");

	EXPECT_NE(a, 0);
    a = SimilerMethodFactory::getInstance()->getMethodeObjct("M")->run("aa", "ab");
	
}



TEST(SK, T4)
{
	string inName = "keyword_weekday_500.txt";
	searchKeywordfotT sk(inName);

	string a = sk.inputSearchKeyword1("s", "monday");

	EXPECT_EQ(a, "s");

	a = sk.inputSearchKeyword1("", "monday");

	EXPECT_EQ(a, "Keyword is vacant\n");

}

TEST(SK, T5)
{
	string inName = "keyword_weekday_500.txt";
	searchKeywordfotT sk(inName);
	sk.run();

}