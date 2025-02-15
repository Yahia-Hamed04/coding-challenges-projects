#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using std::ifstream;

void print_file(ifstream &file) {
 std::string line;

 while (getline(file, line))
  std::cout << line << std::endl;
}

int main(int argc, char* argv[]) {
 ifstream *file_ptr;
 if (argc < 2 || (strlen(argv[1]) == 1 && argv[1][0] == '-')) {
  file_ptr = new ifstream(stdin);

  print_file(*file_ptr);

  delete file_ptr;
  return 0;
 }

 for(int i = 1; i < argc; i++) {
  file_ptr = new ifstream(argv[i]);

  print_file(*file_ptr);
  delete file_ptr;
 }

 return 0;
}