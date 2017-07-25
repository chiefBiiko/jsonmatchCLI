#include <stdio.h>
#include <string>

std::string exec(const std::string cmd) {
  const unsigned int BUFLEN = 128;
  char buffer[BUFLEN];
  std::string out;
  #ifdef _WIN32
  FILE* pipe = _popen(cmd.c_str(), "r");
  #else
  FILE* pipe = popen(cmd.c_str(), "r");
  #endif
  if (!pipe) {
    printf("popen() failed\n");
    exit(1);
  }
  try {
    while (!feof(pipe)) {
      if (fgets(buffer, BUFLEN, pipe) != NULL) out += buffer;
    }
  } catch (...) {
    #ifdef _WIN32
    _pclose(pipe);
    #else
    pclose(pipe);
    #endif
    printf("pipe error\n");
    exit(1);
  }
  #ifdef _WIN32
  _pclose(pipe);
  #else
  pclose(pipe);
  #endif
  return out;
}

void trimBothSides(std::string s) {  // modifies in place
  const char* WS = " \t\n\r\f\v";
  s.erase(0, s.find_first_not_of(WS));
  s.erase(s.find_last_not_of(WS) + 1);
}

std::string readInFile(const char* filename) {
  const int TERMINATORS = 2;  // y 2? thought '\0' is one character??
  int c;
  std::string input;
  FILE* fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("fopen() failed\n");
    exit(1);
  }
  try {
    do {
      c = fgetc(fp);
      if (c == '"') input += "\\\\\\";
      input += c;
    } while (c != EOF);
  } catch (...) {
    fclose(fp);
    printf("file error\n");
    exit(1);
  }
  fclose(fp);
  trimBothSides(input);
  input.resize(input.size() - TERMINATORS);
  return input;
}

bool fileExists(const char* filename) {
  FILE* fp;
  if ((fp = fopen(filename, "r")) != NULL) {
    fclose(fp);
    return 1; 
  } else {
    return 0;
  }
}

std::string concatCommand(std::string json, 
                          const int argc, const char* argv[]) {
  std::string cmd;
  cmd.append("R --vanilla --slave -e jsonmatch::jsonmatch('")
     .append(json).append("','").append(argv[2]).append("'");
  if (argc >= 4) cmd.append(",").append(argv[3]);
  if (argc == 5) cmd.append(",").append(argv[4]);
  cmd.append(") 2>&1\n");
  return cmd;
}

int main(const int argc, const char* argv[]) {
  if (argc < 3) {
    printf("gimme at least 2 arguments\n");
    exit(1);
  }
  std::string json = fileExists(argv[1]) ? readInFile(argv[1]) : argv[1];
  std::string cmd = concatCommand(json, argc, argv);
  printf(exec(cmd).c_str());
  return 0;
}
