#include <cstdlib>
#include <stdio.h>
#include <string>

const std::string exec(const std::string cmd) {
  const unsigned int buflen = 128;
  char buffer[buflen];
  std::string result = "";
  #ifdef _WIN32
  FILE* pipe = _popen(cmd.c_str(), "r");
  #else
  FILE* pipe = popen(cmd.c_str(), "r");
  #endif
  if (!pipe) {
    printf("popen() failed");
    return std::string("");
  }
  try {
    while (!feof(pipe)) {
      if (fgets(buffer, buflen, pipe) != NULL) result.append(buffer, buflen);
    }
  } catch (...) {
    #ifdef _WIN32
    _pclose(pipe);
    #else
    pclose(pipe);
    #endif
    printf("pipe failed");
    return std::string("");
  }
  #ifdef _WIN32
  _pclose(pipe);
  #else
  pclose(pipe);
  #endif
  return result;
}

int main(const int argc, const char* argv[]) {
  const unsigned int ZERO = 0, ONE = 1, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5;
  if (argc < THREE) {
    printf("gimme at least 2 arguments\n");
    return ONE;
  }
  std::string s;
  s.append("R --vanilla --slave -e \"jsonmatch::jsonmatch('")
   .append(argv[ONE]).append("','").append(argv[TWO]).append("'");
  if (argc >= FOUR) s.append(",").append(argv[THREE]);
  if (argc == FIVE) s.append(",").append(argv[FOUR]);
  s.append(")\" 2>&1");
  const std::string rtn = exec(s);
  printf(rtn.c_str());
  return rtn.compare("") ? ZERO : ONE;
}
