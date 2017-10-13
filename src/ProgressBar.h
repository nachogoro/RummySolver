#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <string>

namespace ProgressBar
{
	/**
	 * Prints the progress bar and leaves the cursor right at the beginning of
	 * it, ready for a new call to the method.
	 */
	void printBar(unsigned int percentage);

	/**
	 * Prints a message while the progress bar is on the screen on top on the
	 * last printed message. Since the cursor is right at the beginning of the
	 * progress bar, it moves the cursor up by one line before printing, and
	 * then returns it to its original position.
	 */
	void printMessage(const std::string& message);
}

#endif /* PROGRESS_BAR_H */
