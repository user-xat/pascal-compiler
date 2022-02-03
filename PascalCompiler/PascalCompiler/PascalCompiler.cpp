#include <iostream>
#include <memory>
#include "CIOFile.h"
using namespace std;

int main()
{
    string path = "test.pas";
    //std::unique_ptr<CIOFile> io_module_ptr(new CIOFile(path));
    CIOFile io_module(path);
    string line;
    while (io_module.GetNextLine(line)) {
        std::cout << line << std::endl;
    }

    return 0;
}
