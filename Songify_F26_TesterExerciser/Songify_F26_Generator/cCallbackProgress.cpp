#include "cCallbackProgress.h"

#include <iostream>

void cCallbackProgress::progressData(std::string updateText)
{
	std::cout << updateText;
	return;
};