#pragma once
#ifndef _CIO_FILE_H_
#define _CIO_FILE_H_

#include <string>
using namespace std;

class CIOFile
{
private:
	string path;
	int numLine;
public:
	CIOFile(string path);
	string GetNextLine();
	int GetNumLine();
};

#endif // !_CIO_FILE_H_
