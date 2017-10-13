#include "ProgressBar.h"
#include <iostream>

#define LENGTH_OF_PROGRESS_BAR 55
#define PERCENTAGE_BIN_SIZE (100.0/LENGTH_OF_PROGRESS_BAR)

namespace
{

	std::string generateProgressBar(unsigned int percentage)
	{
		const int progress = static_cast<int>(percentage/PERCENTAGE_BIN_SIZE);
		std::cout <<      "0%    10    20    30    50    60    70    80    90    100%\n";
		std::string bar = "[-----|-----|-----|-----|-----|-----|-----|-----|-----]";

		unsigned int numberOfSymbols = std::min(
				std::max(0, progress - 1),
				LENGTH_OF_PROGRESS_BAR - 2);

		for (size_t i = 1; i <= numberOfSymbols; ++i)
		{
			if (bar[i] != '|')
			{
				bar[i] = '*';
			}
		}

		return bar;
	}
}

void ProgressBar::printBar(unsigned int percentage)
{
	std::cout << generateProgressBar(percentage) << "\r\033[F" << std::flush;
}

void ProgressBar::printMessage(const std::string& message)
{
	std::cout << "\r\033[F" << message << "\n";
}
