#pragma once
#include <iostream>
#include "levenshtein.h"
using namespace std;

class SimilerMethodFactory
{
public:
	unique_ptr<IsimilerMethod> getMethodeObjct(string str);
	static SimilerMethodFactory* getInstance(void);
private:
	SimilerMethodFactory() = default;
	static SimilerMethodFactory* m_instance;
};
