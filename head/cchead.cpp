#include <iostream>
#include <fstream>
#include <cstring>

void return_mode() {
 std::string line;

 for (int i = 0; i < 10; i++) {
  getline(std::cin, line);

  std::cout << line << std::endl;
 }
}

int main(int argc, char* argv[]) {
 int count, mode = 0;
 int file_pos = 1;
 if (argc < 2) return_mode();
 else if (argc > 2 && memcmp(argv[1], "-n", 2) == 0) {
  file_pos++; mode = 1;
  std::string arg1_str = std::string(argv[1]);
  
  if (arg1_str.length() < 3) {
   file_pos++;
   count = atoi(argv[2]);
  } else {
   count = std::stoi(arg1_str.substr(2));
  }
 } else if (argc > 2 && memcmp(argv[1], "-c", 2) == 0) {
  file_pos++; mode = 2;
  std::string arg1_str = std::string(argv[1]);
  
  if (arg1_str.length() < 3) {
   file_pos++;
   count = atoi(argv[2]);
  } else {
   count = std::stoi(arg1_str.substr(2));
  }
 }

 std::ifstream file;
 std::string line;

 bool print_names = file_pos < argc - 1;
 for (int i = file_pos; i < argc; i++) {
  if (print_names)
   std::cout << "==> " << argv[i] << " <==" << std::endl;

  file.open(argv[i]);
  switch (mode) {
   case 1: {
    for (int i = 0; i < count && getline(file, line); i++) {
     std::cout << line << '\n';
    }
   } break;
   case 2: {
    char *bytes = new char[count + 1];
    bytes[count] = 0;
 
    file.read(bytes, count);
    std::cout << bytes << std::endl;
   } break;
  }

  file.close();
 }

 return 0;
}