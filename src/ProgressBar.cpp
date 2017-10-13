#include "ProgressBar.h"
#include <iostream>
#include <iomanip>
#include <sstream>

#define LENGTH_OF_PROGRESS_BAR 55
#define PERCENTAGE_BIN_SIZE (100.0/LENGTH_OF_PROGRESS_BAR)

namespace
{
	std::string generateProgressBar(unsigned int percentage)
	{
		const int progress = static_cast<int>(percentage/PERCENTAGE_BIN_SIZE);
		std::ostringstream ss;
		ss << " " << std::setw(3) << std::right << percentage << "% ";
		std::string bar("[" + std::string(LENGTH_OF_PROGRESS_BAR-2, ' ') + "]");

		unsigned int numberOfSymbols = std::min(
				std::max(0, progress - 1),
				LENGTH_OF_PROGRESS_BAR - 2);

		bar.replace(1, numberOfSymbols, std::string(numberOfSymbols, '|'));

		ss << bar;
		return ss.str();
	}
}

void ProgressBar::startProgressBar(const std::string& msg)
{
	std::cout << msg << "\n";
	printBar(0);
}

void ProgressBar::printBar(unsigned int percentage)
{
	std::cout << generateProgressBar(percentage) << "\r" << std::flush;
}

void ProgressBar::printMessage(const std::string& message)
{
	std::cout << "\r\033[F" << message << "\n";
}

void ProgressBar::endProgressBar()
{
	std::cout << std::string(2, '\n');
}
