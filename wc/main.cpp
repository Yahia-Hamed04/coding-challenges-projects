#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>
#include <clocale>
#include <cstdlib>
using namespace std;

size_t count_bytes(ifstream &file) {
 file.seekg(0, ios_base::end);
 
 size_t count = file.tellg();
 file.seekg(0);

 return count;
}

size_t count_lines(ifstream &file) {
 size_t count = 0;
 string line;

 while (getline(file, line)) count++;
 
 file.clear();
 file.seekg(0);

 return count;
}

size_t count_words(ifstream &file) {
 size_t count = 0;
 string word;

 while (file >> word) count++;
 
 file.clear();
 file.seekg(0);

 return count;
}

size_t count_chars(ifstream &file) {
 setlocale(LC_ALL, "");

 string file_str;

 while (!file.eof()) {
  file_str += file.get();
 } 
 file.clear();
 file.seekg(0);

 mblen(NULL, 0);
 auto ptr = file_str.data();
 size_t count = 0;

 for (auto end = ptr + file_str.size(); ptr < end; count++) {
  auto next = mblen(ptr, end - ptr);
  if (next < 1) break;
  ptr += next;
 }

 return count;
}

int main(int argc, char* argv[]) {
 ifstream *file_ptr;
 string filename = argv[1 + (argc > 2)];
 if (filename[0] != '-') {
  file_ptr = new ifstream(filename);
 } else {
  file_ptr = new ifstream(stdin);
 }

 if (argc > 1 && argv[1][0] == '-') {
  switch (argv[1][1]) {
   case 'c': cout << count_bytes(*file_ptr); break;
   case 'l': cout << count_lines(*file_ptr); break;
   case 'w': cout << count_words(*file_ptr); break;
   case 'm': cout << count_chars(*file_ptr); break;
  }
 } else {
  cout << count_lines(*file_ptr)
  <<  ' '   << count_words(*file_ptr)
  <<  ' '   << count_bytes(*file_ptr);
 }

 if (filename[0] != '-') cout << ' ' << filename;

 file_ptr->close();
}