#pragma once

#include <string>

class IsimilerMethod
{
public:
	virtual uint32_t  run(const std::string& a, const std::string& b) = 0;
};