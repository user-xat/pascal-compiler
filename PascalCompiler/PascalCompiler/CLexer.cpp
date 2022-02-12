#include "CLexer.h"

CLexer::CLexer(const std::string& path_to_file)
{
	this->file = std::make_unique<CIOFile>(path_to_file);
	this->ch_num = 0;
}

CTokenPtr CLexer::GetNextToken()
{
	CTokenPtr token;
	if (line.empty() || ch_num >= line.size()) {
		file->GetNextLine(line);
		ch_num = 0;
	}
	// Ищем токен
	if (line[ch_num] == ':') {
		++ch_num;
		if (ch_num < line.length() && line[ch_num] == '=') {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::AOP_ASSIGN));
			++ch_num;
		}
		else {
			// TODO: Operation ':'
		}
	}
	else if (line[ch_num] == '<') {
		++ch_num;
		if (ch_num < line.length() && line[ch_num] == '=') {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::COP_LE));
			++ch_num;
		}
		else {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::COP_LT));
		}
	}
	else if (line[ch_num] == '>') {
		++ch_num;
		if (ch_num < line.length() && line[ch_num] == '=') {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::COP_GE));
			++ch_num;
		}
		else {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::COP_GT));
		}
	}
	else if (line[ch_num] == '/') {
		++ch_num;
		if (ch_num < line.length() && line[ch_num] == '/') {
			// TODO: обработать комментарий
		}
		else {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::AOP_DIV));
		}
	}
	else if (line[ch_num] == ';') {
		token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::END_OF_STATEMENT));
		++ch_num;
	}
	else if (line[ch_num] == '{') {
		// TODO: обработать комментарий
	}
	else if (line[ch_num] == '+') {
		token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::AOP_SUM));
		++ch_num;
	}
	else if (line[ch_num] == '-') {
		token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::AOP_SUB));
		++ch_num;
	}
	else if (line[ch_num] == '*') {
		token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::AOP_MULT));
		++ch_num;
	}
	else if (IsDigit(line[ch_num])) {
		std::string num = GetNumber();
		if (num.find('.') != -1) {
			double value;
			if (IsNumber<double>(num, value)) {
				token = std::make_unique<CToken>(new CConstToken<double>(EVariantType::REAL, value));
			}
			else {
				// TODO: Ошибка 
			}
		}
		else {
			int value;
			if (IsNumber<int>(num, value)) {
				token =  std::make_unique<CToken>(new CConstToken<int>(EVariantType::INTEGER, value));
			}
			else {
				// TODO: Ошибка 
			}
		}
	}
	else if (IsLetter(line[ch_num])) {
		std::string word = GetWord();
		if (word == "var") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::VAR));
		}
		else if (word == "type") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::TYPE));
		}
		else if (word == "begin") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::BEGIN));
		}
		else if (word == "end") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::END));
		}
		else if (word == "if") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::IF));
		}
		else if (word == "then") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::THEN));
		}
		else if (word == "else") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::ELSE));
		}
		else if (word == "while") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::WHILE));
		}
		else if (word == "do") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::DO));
		}
		// Логические операции
		else if (word == "and") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::LOP_AND));
		}
		else if (word == "or") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::LOP_OR));
		}
		else if (word == "xor") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::LOP_XOR));
		}
		else if (word == "not") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::LOP_NOT));
		}
		else if (word == "True") {
			token = std::make_unique<CToken>(new CConstToken<bool>(EVariantType::BOOLEAN, true));
		}
		else if (word == "False") {
			token = std::make_unique<CToken>(new CConstToken<bool>(EVariantType::BOOLEAN, false));
		}
		// Типы данных
		else if (word == "integer") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::INTEGER));
		}
		else if (word == "real") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::REAL));
		}
		else if (word == "boolean") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::BOOLEAN));
		}
		else if (word == "string") {
			token = std::make_unique<CToken>(new CKeywordToken(EKeyWords::STRING));
		}
		else {
			// TODO: 
			//		Ошибка
			//		Пользовательский идентификатор
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
	return ch == ' ' || ch == '\t' || '\n';
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
	while (IsDigit(line[ch_num]) || '.') {
		++ch_num;
	}
	return line.substr(ch_beg, ch_num - ch_beg);
}
