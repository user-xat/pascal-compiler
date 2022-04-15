#include <iostream>
#include <memory>
#include "CIOFile.h"
#include "CLexer.h"
#include "CParser.h"

int main()
{
    using namespace std;
    //string path = "minimum_error.txt";
    string path = "minimum.txt";
    //path = "minimum_sem_error.txt";
    //path = "medium.txt";
    //path = "medium_sem_error.txt";
    //path = "hard_sem.txt";
    //path = "hard_sem_error.txt";
    //path = "hard_sem_error.txt";
    //path = "synt_error.txt";
    
    //CIOFile io_module(path);
    //string line;
    //while (io_module.GetNextLine(line)) {
    //    std::cout << line << std::endl;
    //}
    
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

    CParser parser(path);
    parser.Parse();

    return 0;
}
