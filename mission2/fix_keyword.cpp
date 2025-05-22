#include <iostream>
#include "SimilerMethodFactory.h"
#include "searchKeyword.h"

using namespace std;

int main()
{
	searchKeyword sk("keyword_weekday_500.txt");

	// 기능 실행
	sk.run();
}
