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
    const char REDIRECT = '>';
    const char* RSCRIPT = "Rscript";
    const char* RFILE = "jsonmatchCLI.R";
    const char* STREAMLOG = ".stdout_stderr.log";
    const char* STDOUTERR = "2>&1";
    std::stringstream ss;
    ss << RSCRIPT << BLNK << RFILE << BLNK; 
    ss << argv[1] << BLNK << argv[2] << BLNK;
    if (argc >= 4) ss << argv[3] << BLNK;
    if (argc == 5) ss << argv[4] << BLNK;
    ss << REDIRECT << BLNK << STREAMLOG << BLNK << STDOUTERR;

    std::system(ss.str().c_str());
    std::cout << std::ifstream(STREAMLOG).rdbuf();
    std::remove(STREAMLOG);

    return 0;
}