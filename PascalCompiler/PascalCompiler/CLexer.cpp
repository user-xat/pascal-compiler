#include "CLexer.h"

CLexer::CLexer(const std::string &path_to_file)
{
	this->file = std::make_unique<CIOFile>(path_to_file);
	this->ch_num = 0;
}

CTokenPtr CLexer::GetNextToken()
{
	CTokenPtr token = nullptr;
	// Получение новой строки
	while (line.empty() || ch_num >= line.size()) {
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
	if (line[ch_num] == ',') {
		token = std::make_unique<CKeywordToken>(key_words.find(",")->second);
		++ch_num;
	}
	else if (line[ch_num] == '=') {
		token = std::make_unique<CKeywordToken>(key_words.find("=")->second);
		++ch_num;
	}
	else if (line[ch_num] == ':') {
		++ch_num;
		if (ch_num < line.length() && line[ch_num] == '=') {
			token = std::make_unique<CKeywordToken>(key_words.find(":=")->second);
			++ch_num;
		}
		else {
			token = std::make_unique<CKeywordToken>(key_words.find(":")->second);
		}
	}
	else if (line[ch_num] == '<') {
		++ch_num;
		if (ch_num < line.length() && line[ch_num] == '=') {
			token = std::make_unique<CKeywordToken>(key_words.find("<=")->second);
			++ch_num;
		}
		else if (ch_num < line.length() && line[ch_num] == '>') {
			token = std::make_unique<CKeywordToken>(key_words.find("<>")->second);
			++ch_num;
		}
		else {
			token = std::make_unique<CKeywordToken>(key_words.find("<")->second);
		}
	}
	else if (line[ch_num] == '>') {
		++ch_num;
		if (ch_num < line.length() && line[ch_num] == '=') {
			token = std::make_unique<CKeywordToken>(key_words.find(">=")->second);
			++ch_num;
		}
		else {
			token = std::make_unique<CKeywordToken>(key_words.find(">")->second);
		}
	}
	else if (line[ch_num] == '/') {
		++ch_num;
		token = std::make_unique<CKeywordToken>(key_words.find("/")->second);
	}
	else if (line[ch_num] == ';') {
		token = std::make_unique<CKeywordToken>(key_words.find(";")->second);
		++ch_num;
	}
	else if (line[ch_num] == '+') {
		token = std::make_unique<CKeywordToken>(key_words.find("+")->second);
		++ch_num;
	}
	else if (line[ch_num] == '-') {
		token = std::make_unique<CKeywordToken>(key_words.find("-")->second);
		++ch_num;
	}
	else if (line[ch_num] == '*') {
		token = std::make_unique<CKeywordToken>(key_words.find("*")->second);
		++ch_num;
	}
	else if (line[ch_num] == '(') {
		++ch_num;
		token = std::make_unique<CKeywordToken>(key_words.find("(")->second);
	}
	else if (line[ch_num] == ')') {
		++ch_num;
		
		token = std::make_unique<CKeywordToken>(key_words.find(")")->second);
	}
	else if (line[ch_num] == '\'') {
		int start = ch_num + 1;
		do
		{
			++ch_num;
		} while (ch_num < line.size() && line[ch_num] != '\'');
		if (ch_num >= line.size()) {
			// TODO: Error
		}
		std::string str = line.substr(start, ch_num - start);
		token = std::make_unique<CConstToken>(str);
		++ch_num;
	}
	else if (IsDigit(line[ch_num])) {
		std::string num = GetNumber();
		if (num.find('.') != -1) {
			double value;
			if (IsNumber<double>(num, value)) {
				token = std::make_unique<CConstToken>(value);
			}
			else {
				// TODO: Ошибка 
			}
		}
		else {
			int value;
			if (IsNumber<int>(num, value)) {
				token =  std::make_unique<CConstToken>(value);
			}
			else {
				// TODO: Ошибка 
			}
		}
	}
	else if (IsLetter(line[ch_num])) {
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
	int ch_beg{ ch_num };
	while (IsLetter(line[ch_num]) || IsDigit(line[ch_num])) {
		++ch_num;
	}
	return line.substr(ch_beg, ch_num - ch_beg);
}

std::string CLexer::GetNumber() {
	int ch_beg{ ch_num };
	while (IsDigit(line[ch_num]) || line[ch_num] == '.') {
		++ch_num;
	}
	return line.substr(ch_beg, ch_num - ch_beg);
}

bool CLexer::GetNewLine()
{
	bool result = this->file->GetNextLine(this->line);
	this->ch_num = 0;
	return result;
}

bool CLexer::SkipSpaces() {
	bool success{ true };
	if (ch_num >= line.size()) {
		success = GetNewLine();
	}
	while (success && IsWhiteSpace(line[ch_num]))
	{
		++ch_num;
		if (ch_num >= line.size()) {
			success = GetNewLine();
		}
	}
	return success;
}

bool CLexer::SkipComments() {
	bool success{ true };
	// // comment
	if (ch_num < line.size() - 1 && line[ch_num] == '/' && line[ch_num + 1] == '/') {
		do
		{
			success = GetNewLine();
			success = SkipSpaces();
		} while (success && ch_num < line.size() - 1 && line[ch_num] == '/' && line[ch_num + 1] == '/');
	}
	// { comment }
	else if (ch_num < line.size() && line[ch_num] == '{') {
		while (line[ch_num] != '}')
		{
			++ch_num;
			if (ch_num >= line.size()) {
				success = GetNewLine();
			}
			if (!success) {
				// TODO: Error
			}
		}
		++ch_num;
	}
	// (* comment *)
	else if (ch_num < line.size() - 1 && line[ch_num] == '(' && line[ch_num + 1] == '*') {
		do
		{
			++ch_num;
			if (ch_num >= line.size() - 1) {
				success = GetNewLine();
			}
			if (!success) {
				// TODO: Error
			}
		} while (ch_num < line.size() - 1 && !(line[ch_num] == '*' && line[ch_num + 1] == ')'));
		ch_num += 2;
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
