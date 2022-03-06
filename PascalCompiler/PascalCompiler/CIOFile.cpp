#include "CIOFile.h"

CIOFile::CIOFile(const std::string& path) {
	this->path = path;
	numLine = 0;
	file.open(path);
	if (!file.is_open())
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
	line = "";
	return false;
}

int CIOFile::GetNumLine()
{
	return numLine;
}
