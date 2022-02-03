#pragma once
#ifndef _CIO_FILE_H_
#define _CIO_FILE_H_

#include <string>
#include <fstream>
#include <memory>

class CIOFile
{
private:
	std::ifstream file;
	std::string path;
	int numLine;
public:
	CIOFile(const std::string& path);
	~CIOFile();
	bool GetNextLine(std::string& line);
	int GetNumLine();
};

typedef std::unique_ptr<CIOFile> CIOFilePtr;

#endif // !_CIO_FILE_H_
