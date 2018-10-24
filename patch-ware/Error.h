

#ifndef PW_ERROR_H
#define PW_ERROR_H

#include <string>
#include <fstream>
#include <iostream>

const std::string LOG_FILE = "error.log";

void error(const std::string &msg, bool toFile = false);


#endif