#include <iostream>
#include <memory>
#include "CIOFile.h"
#include "CLexer.h"
#include "EKeyWords.h"

int main()
{
    using namespace std;
    string path = "medium.txt";
    CIOFile io_module(path);
    string line;
    while (io_module.GetNextLine(line)) {
        std::cout << line << std::endl;
    }
    
    /*CLexer lexer(path);
    CTokenPtr token;
    int i = 0;
    do
    {
        token.reset();
        ++i;
        token = lexer.GetNextToken();
        if (token != nullptr)
            cout << i << ": " << token->ToString() << "\n";
    } while (token != nullptr);*/

    return 0;
}
