#include "SimilerMethodFactory.h"
#include "levenshtein.h"
#include "mevenshtein.h"

SimilerMethodFactory* SimilerMethodFactory::m_instance = nullptr;

unique_ptr<IsimilerMethod> SimilerMethodFactory::getMethodeObjct(string str)
{
	if (str == "L")
	{
		return make_unique<levenshtein>();	}
	else
	{
		// error msg , temp  
		return make_unique<mevenshtein>();
	}}

SimilerMethodFactory* SimilerMethodFactory::getInstance(void)
{
	if (m_instance == nullptr) {
		m_instance = new SimilerMethodFactory();
	}
	return m_instance;
}
