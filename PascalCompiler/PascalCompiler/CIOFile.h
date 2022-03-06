#pragma once
#ifndef CIO_FILE_H
#define CIO_FILE_H

#include <string>
#include <algorithm>
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
