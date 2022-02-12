#include "CIOFile.h"

CIOFile::CIOFile(const std::string& path) {
	this->path = path;
	numLine = 0;
	file.open(path);
	if (file.is_open())
	{
		// TODO
	}
}

CIOFile::~CIOFile() {
	file.close();
}

bool CIOFile::GetNextLine(std::string& line) {
	if (!file.eof()) {
		std::getline(file, line);
		numLine++;
		return true;
	}
	return false;
}

int CIOFile::GetNumLine()
{
	return numLine;
}

//void CIOFile::ToLower(std::string& line) {
//	std::for_each(line.begin(), line.end(), [](char& c) {
//		c = tolower(c);
//	});
//}