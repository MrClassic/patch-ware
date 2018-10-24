


#include "Error.h"

void error(const std::string &msg, bool toFile) {

	if (toFile) {
		std::fstream errorFile;
		errorFile.open(LOG_FILE.c_str(), std::ios_base::app | std::ios_base::out);
		errorFile << msg << '\n' << '\n';
		errorFile.close();
	}

	std::cerr << msg << '\n' << '\n';

}