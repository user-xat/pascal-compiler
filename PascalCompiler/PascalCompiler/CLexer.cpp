#include "CLexer.h"

CLexer::CLexer(const std::string &path_to_file)
{
	this->m_file = std::make_unique<CIOFile>(path_to_file);
	this->m_ch_num = 0;
}

CTokenPtr CLexer::GetNextToken()
{
	CTokenPtr token = nullptr;
	// Получение новой строки
	while (m_line.empty() || m_ch_num >= m_line.size()) {
		if (!GetNewLine()) {
			return nullptr;
		}
	}
	// Пропуск проблелов
	if (!SkipSpaces()) {
		return nullptr;
	}
	// Обработка комментариев
	if (!SkipComments()) {
		return nullptr;
	}
	// Ищем токен
	if (m_line[m_ch_num] == '.') {
		token = std::make_unique<CKeywordToken>(key_words.find(".")->second);
		++m_ch_num;
	}
	else if (m_line[m_ch_num] == ',') {
		token = std::make_unique<CKeywordToken>(key_words.find(",")->second);
		++m_ch_num;
	}
	else if (m_line[m_ch_num] == '=') {
		token = std::make_unique<CKeywordToken>(key_words.find("=")->second);
		++m_ch_num;
	}
	else if (m_line[m_ch_num] == ':') {
		++m_ch_num;
		if (m_ch_num < m_line.length() && m_line[m_ch_num] == '=') {
			token = std::make_unique<CKeywordToken>(key_words.find(":=")->second);
			++m_ch_num;
		}
		else {
			token = std::make_unique<CKeywordToken>(key_words.find(":")->second);
		}
	}
	else if (m_line[m_ch_num] == '<') {
		++m_ch_num;
		if (m_ch_num < m_line.length() && m_line[m_ch_num] == '=') {
			token = std::make_unique<CKeywordToken>(key_words.find("<=")->second);
			++m_ch_num;
		}
		else if (m_ch_num < m_line.length() && m_line[m_ch_num] == '>') {
			token = std::make_unique<CKeywordToken>(key_words.find("<>")->second);
			++m_ch_num;
		}
		else {
			token = std::make_unique<CKeywordToken>(key_words.find("<")->second);
		}
	}
	else if (m_line[m_ch_num] == '>') {
		++m_ch_num;
		if (m_ch_num < m_line.length() && m_line[m_ch_num] == '=') {
			token = std::make_unique<CKeywordToken>(key_words.find(">=")->second);
			++m_ch_num;
		}
		else {
			token = std::make_unique<CKeywordToken>(key_words.find(">")->second);
		}
	}
	else if (m_line[m_ch_num] == '/') {
		++m_ch_num;
		token = std::make_unique<CKeywordToken>(key_words.find("/")->second);
	}
	else if (m_line[m_ch_num] == ';') {
		token = std::make_unique<CKeywordToken>(key_words.find(";")->second);
		++m_ch_num;
	}
	else if (m_line[m_ch_num] == '+') {
		token = std::make_unique<CKeywordToken>(key_words.find("+")->second);
		++m_ch_num;
	}
	else if (m_line[m_ch_num] == '-') {
		token = std::make_unique<CKeywordToken>(key_words.find("-")->second);
		++m_ch_num;
	}
	else if (m_line[m_ch_num] == '*') {
		token = std::make_unique<CKeywordToken>(key_words.find("*")->second);
		++m_ch_num;
	}
	else if (m_line[m_ch_num] == '(') {
		++m_ch_num;
		token = std::make_unique<CKeywordToken>(key_words.find("(")->second);
	}
	else if (m_line[m_ch_num] == ')') {
		++m_ch_num;
		token = std::make_unique<CKeywordToken>(key_words.find(")")->second);
	}
	else if (m_line[m_ch_num] == '\'') {
		int start = m_ch_num + 1;
		do {
			++m_ch_num;
		} while (m_ch_num < m_line.size() && m_line[m_ch_num] != '\'');
		if (m_ch_num >= m_line.size()) {
			// TODO: Error
			std::string err_mes = GenerateErrorMessage(m_file->GetNumLine());
			m_file->WriteLine(err_mes);
			return nullptr;
		}
		std::string str = m_line.substr(start, m_ch_num - start);
		token = std::make_unique<CConstToken>(str);
		++m_ch_num;
	}
	else if (IsDigit(m_line[m_ch_num])) {
		std::string num = GetNumber();
		if (num.find('.') != -1) {
			double value;
			if (IsNumber<double>(num, value)) {
				token = std::make_unique<CConstToken>(value);
			}
			else {
				// TODO: Ошибка 
				std::string err_mes = GenerateErrorMessage(m_file->GetNumLine());
				m_file->WriteLine(err_mes);
				return nullptr;
			}
		}
		else {
			int value;
			if (IsNumber<int>(num, value)) {
				token =  std::make_unique<CConstToken>(value);
			}
			else {
				// TODO: Ошибка 
				std::string err_mes = GenerateErrorMessage(m_file->GetNumLine());
				m_file->WriteLine(err_mes);
				return nullptr;
			}
		}
	}
	else if (IsLetter(m_line[m_ch_num])) {
		std::string word = GetWord();
		if (word == "True") {
			token = std::make_unique<CConstToken>(true);
		}
		else if (word == "False") {
			token = std::make_unique<CConstToken>(false);
		}
		else {
			token = ProcessingKeyWord(word);
			if (token == nullptr) {
				token = std::make_unique<CIdentToken>(word);
			}
		}
	}

	return token;
}

bool CLexer::IsLetter(const char ch) {
	return 'A' <= ch && ch <= 'Z' || 'a' <= ch && ch <= 'z' || ch == '_';
}

bool CLexer::IsDigit(const char ch) {
	return '0' <= ch && ch <= '9';
}

bool CLexer::IsWhiteSpace(const char ch) {
	return ch == ' ' || ch == '\t' || ch == '\n';
}

std::string CLexer::GetWord() {
	int ch_beg{ m_ch_num };
	while (IsLetter(m_line[m_ch_num]) || IsDigit(m_line[m_ch_num])) {
		++m_ch_num;
	}
	return m_line.substr(ch_beg, m_ch_num - ch_beg);
}

std::string CLexer::GetNumber() {
	int ch_beg{ m_ch_num };
	while (IsDigit(m_line[m_ch_num]) || m_line[m_ch_num] == '.') {
		++m_ch_num;
	}
	return m_line.substr(ch_beg, m_ch_num - ch_beg);
}

bool CLexer::GetNewLine()
{
	bool result = this->m_file->GetNextLine(this->m_line);
	this->m_ch_num = 0;
	return result;
}

bool CLexer::SkipSpaces() {
	bool success{ true };
	if (m_ch_num >= m_line.size()) {
		success = GetNewLine();
	}
	while (success && IsWhiteSpace(m_line[m_ch_num]))
	{
		++m_ch_num;
		if (m_ch_num >= m_line.size()) {
			success = GetNewLine();
		}
	}
	return success;
}

bool CLexer::SkipComments() {
	bool success{ true };
	// // comment
	if (m_ch_num < m_line.size() - 1 && m_line[m_ch_num] == '/' && m_line[m_ch_num + 1] == '/') {
		do
		{
			success = GetNewLine();
			success = SkipSpaces();
		} while (success && m_ch_num < m_line.size() - 1 && m_line[m_ch_num] == '/' && m_line[m_ch_num + 1] == '/');
	}
	// { comment }
	else if (m_ch_num < m_line.size() && m_line[m_ch_num] == '{') {
		while (m_line[m_ch_num] != '}')
		{
			++m_ch_num;
			if (m_ch_num >= m_line.size()) {
				success = GetNewLine();
			}
			if (!success) {
				// TODO: Error
			}
		}
		++m_ch_num;
	}
	// (* comment *)
	else if (m_ch_num < m_line.size() - 1 && m_line[m_ch_num] == '(' && m_line[m_ch_num + 1] == '*') {
		do
		{
			++m_ch_num;
			if (m_ch_num >= m_line.size() - 1) {
				success = GetNewLine();
			}
			if (!success) {
				// TODO: Error
			}
		} while (m_ch_num < m_line.size() - 1 && !(m_line[m_ch_num] == '*' && m_line[m_ch_num + 1] == ')'));
		m_ch_num += 2;
	}
	success = SkipSpaces();
	return success;
}

CTokenPtr CLexer::ProcessingKeyWord(const std::string &word) {
	CTokenPtr token = nullptr;
	std::map<std::string, EKeyWords>::const_iterator type = key_words.find(word);
	if (type != key_words.end()) {
		token = std::make_unique<CKeywordToken>(type->second);
	}
	return token;
}


std::string CLexer::GenerateErrorMessage(int line_num)
{
	std::string mes;
	for (int i = 0; i < m_ch_num; ++i) {
		mes += ' ';
	}
	mes += "| Unexpected token on " + std::to_string(line_num) + " line and " + std::to_string(m_ch_num + 1) + " character";
	return mes;
}
