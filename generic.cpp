#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <string>

const std::string exec(const std::string cmd) {
  char buffer[128];
  std::string result = "";
  #ifdef _WIN32
  FILE* pipe = _popen(cmd.c_str(), "r");
  #else
  FILE* pipe = popen(cmd.c_str(), "r");
  #endif
  if (!pipe) {
    printf("popen() failed!");
    return std::string("");
  }
  try {
    while (!feof(pipe)) if (fgets(buffer, 128, pipe) != NULL) result += buffer;
  } catch (...) {
    #ifdef _WIN32
    _pclose(pipe);
    #else
    pclose(pipe);
    #endif
    throw;
  }
  #ifdef _WIN32
  _pclose(pipe);
  #else
  pclose(pipe);
  #endif
  return result;
}

int main(const int argc, const char* argv[]) {
  if (argc < 3) {
    printf("gimme at least 2 arguments");
    return 1;
  }
  const char BLNK = ' ';
  const char DELM = ',';
  const char QUOT = '\'';
  const char* R = "R --vanilla --slave -e";
  const char* FRNT = "\"jsonmatch::jsonmatch(";
  const char* BACK = ")\"";
  const char* STDOUTERR = "2>&1";
  std::stringstream ss;
  ss << R << BLNK << FRNT;
  ss << QUOT << argv[1] << QUOT << DELM;
  ss << QUOT << argv[2] << QUOT;
  if (argc >= 4) ss << DELM << argv[3];
  if (argc == 5) ss << DELM << argv[4];
  ss << BACK << BLNK << STDOUTERR;
  std::string rtn = exec(ss.str());
  printf(rtn.c_str());
  return rtn != "" ? 0 : 1;
}