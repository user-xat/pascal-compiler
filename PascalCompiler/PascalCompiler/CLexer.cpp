#include "CLexer.h"

CLexer::CLexer(const std::string& path_to_file)
{
	this->file = std::make_unique<CIOFile>(path_to_file);
	this->ch_num = 0;
}

CToken CLexer::GetNextToken()
{
	if (line.empty() || ch_num >= line.size()) {
		file->GetNextLine(line);
		ch_num = 0;
	}
	// »щем токен
}
