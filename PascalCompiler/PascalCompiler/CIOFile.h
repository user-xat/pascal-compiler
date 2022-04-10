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
	std::ifstream m_in_file;
	std::ofstream m_out_file;
	std::string m_path;
	int m_num_line;

public:
	CIOFile(const std::string& path);
	~CIOFile();
	bool GetNextLine(std::string& line);
	int GetNumLine() const;
	CIOFile& WriteLine(const std::string& line);
};

typedef std::unique_ptr<CIOFile> CIOFilePtr;

#endif // !_CIO_FILE_H_
