#pragma once
#include "IsimilerMethod.h"

class mevenshtein : public IsimilerMethod
{
public:
	uint32_t  run(const std::string& a, const std::string& b);
};