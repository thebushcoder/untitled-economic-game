/*
 * fileLogger.cpp
 *
 *  Created on: 2 Jan 2017
 *      Author: linbob
 */

#include "fileLogger.hpp"

using namespace Logging;

FileLogger::FileLogger(std::string path) : defaultFilePath(path){
	if(defaultFilePath.empty()){
		defaultFilePath = getenv("HOME");
		defaultFilePath += "/linbobGame/debug.log";
	}

	ofs.open(defaultFilePath, std::ofstream::out);
	if(ofs.is_open()){
		ofs << "";
	}
	ofs.close();
}

void FileLogger::logString(std::string s, std::string filePath){
	if(filePath.empty()){
		ofs.open(defaultFilePath, std::ofstream::out | std::ofstream::app);
	}else{
		std::string p = getenv("HOME");
		p += "/linbobGame/" + filePath;

		if(std::find(pathList.begin(), pathList.end(), p) != pathList.end()){
			ofs.open(p, std::ofstream::out | std::ofstream::app);
		}else{
			pathList.push_back(p);
			ofs.open(p, std::ofstream::out);
		}
	}

	if(ofs.is_open()){
		ofs << s << std::endl;
	}
	ofs.close();
}
FileLogger* FileLogger::getInstance(){
	static FileLogger logger;
	return &logger;
}
