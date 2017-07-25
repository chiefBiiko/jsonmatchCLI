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

std::string readInFile(const char* filename) {
  #ifdef _WIN32
  const int NEWLINE_CHAR_COUNT = 2;
  #else
  const int NEWLINE_CHAR_COUNT = 1;
  #endif
  int c;
  std::string in;
  FILE* fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("fopen() failed\n");
    exit(1);
  }
  try {
    do {
      c = fgetc(fp);
      if (c == '"') in += "\\\\\\";
      in += c;
    } while (c != EOF);
  } catch (...) {
    fclose(fp);
    printf("file error\n");
    exit(1);
  }
  fclose(fp);
  in.resize(in.size() - NEWLINE_CHAR_COUNT);
  return in;
}

int fileExists(const char* filename) {
  FILE* fp;
  if ((fp = fopen(filename, "r")) != NULL) {
    fclose(fp);
    return 1; 
  } else {
    return 0;
  }
}

/*
 * TODO:
 * -escape quotes in read-in file - DONE
 * -test file exists!
 * -makeindi string concat stuff and use std::string 4 dynamic growing only
 * -
 */
int main(const int argc, const char* argv[]) {
  if (argc < 3) {
    printf("gimme at least 2 arguments\n");
    exit(1);
  }
  std::string in = readInFile(argv[1]);
  std::string cmd;
  cmd.append("R --vanilla --slave -e jsonmatch::jsonmatch('")
   .append(in.c_str()).append("','").append(argv[2]).append("'");
  if (argc >= 4) cmd.append(",").append(argv[3]);
  if (argc == 5) cmd.append(",").append(argv[4]);
  cmd.append(") 2>&1\n");
  /*
  printf(s.c_str());
  */
  printf(exec(cmd).c_str());
  return 0;
}
