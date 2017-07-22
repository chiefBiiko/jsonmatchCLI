#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>

int main(const int argc, const char* argv[]) {

    if (argc < 3) {
        std::cerr << "gimme at least 2 arguments" << std::endl;
        return 1;
    }
    
    const char BLNK = ' ';
    const char DELM = ',';
    const char QUOT = '\'';
    const char REDIRECT = '>';
    const char* R = "R --vanilla --slave -e";
    const char* FRNT = "\"jsonmatch::jsonmatch(";
    const char* BACK = ")\"";
    const char* STREAMLOG = ".stdout_stderr.log";
    const char* STDOUTERR = "2>&1";
    std::stringstream ss;
    ss << R << BLNK << FRNT;
    ss << QUOT << argv[1] << QUOT << DELM;
    ss << QUOT << argv[2] << QUOT;
    if (argc >= 4) ss << DELM << argv[3];
    if (argc == 5) ss << DELM << argv[4];
    ss << BACK << BLNK << REDIRECT << BLNK << STREAMLOG << BLNK << STDOUTERR;
    
    std::system(ss.str().c_str());
    std::cout << std::ifstream(STREAMLOG).rdbuf();
    std::remove(STREAMLOG);

    return 0;
}