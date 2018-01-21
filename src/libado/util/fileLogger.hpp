/*
 * fileLogger.hpp
 *
 *  Created on: 2 Jan 2017
 *      Author: linbob
 */

#ifndef LIBADO_UTIL_FILELOGGER_HPP_
#define LIBADO_UTIL_FILELOGGER_HPP_

#include <iostream>
#include <fstream>
#include <algorithm>    // std::find
#include <vector>

namespace Logging{

static void printToConsole(std::string s){
	std::cout << s << std::endl;
}

class FileLogger{
	std::string defaultFilePath;
	std::ofstream ofs;

	std::vector<std::string> pathList;

	FileLogger(std::string path = "");

public:
	void logString(std::string s, std::string filePath = "");
	static FileLogger* getInstance();
};
}

#endif /* LIBADO_UTIL_FILELOGGER_HPP_ */
