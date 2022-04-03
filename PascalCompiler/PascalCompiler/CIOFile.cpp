#include "CIOFile.h"

CIOFile::CIOFile(const std::string& path) {
	this->m_path = path;
	m_numLine = 0;
	m_in_file.open(path);
	if (!m_in_file.is_open())
	{
		// TODO
	}
	m_out_file.open("LISTING.txt");
	if (!m_out_file.is_open()) {
		// TODO
	}
}

CIOFile::~CIOFile() {
	m_in_file.close();
	m_out_file.close();
}

bool CIOFile::GetNextLine(std::string& line) {
	if (!m_in_file.eof()) {
		std::getline(m_in_file, line);
		m_numLine++;
		WriteLine(line);
		return true;
	}
	line = "";
	return false;
}

int CIOFile::GetNumLine() const
{
	return m_numLine;
}

CIOFile& CIOFile::WriteLine(const std::string& line) 
{
	m_out_file << line << std::endl;
	return *this;
}
