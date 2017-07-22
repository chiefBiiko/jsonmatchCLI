#include <cstdlib>
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
    while (!feof(pipe)) {
      if (fgets(buffer, 128, pipe) != NULL) result.append(buffer, 128);
    }
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
    printf("gimme at least 2 arguments\n");
    return 1;
  }
  std::string s;
  s.append("R --vanilla --slave -e \"jsonmatch::jsonmatch('")
   .append(argv[1]).append("','").append(argv[2]).append("'");
  if (argc >= 4) s.append(",").append(argv[3]);
  if (argc == 5) s.append(",").append(argv[4]);
  s.append(")\" 2>&1");
  const std::string rtn = exec(s);
  printf(rtn.c_str());
  return s.compare("") ? 0 : 1;
}
