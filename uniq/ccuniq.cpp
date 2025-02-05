#include <iostream>
#include <fstream>
#include <vector>

const char* ws = " \t\n\r\f\v";

// trim from end of string (right)
inline std::string& rtrim(std::string& s, const char* t = ws) {
 s.erase(s.find_last_not_of(t) + 1);
 return s;
}

// trim from beginning of string (left)
inline std::string& ltrim(std::string& s, const char* t = ws) {
 s.erase(0, s.find_first_not_of(t));
 return s;
}

// trim from both ends of string (right then left)
inline std::string& trim(std::string& s, const char* t = ws) {
 return ltrim(rtrim(s, t), t);
}

void readData(
 std::istream &is,
 std::ostream &os = std::cout,
 bool show_count = false,
 bool only_dups  = false,
 bool only_uniq  = false
) {
 if (only_dups && only_uniq) return;
 
 int count = 0;
 std::string prev_line = "", line;
 while (getline(is, line)) {
  line = trim(line);
  if (line == "") continue;
  
  if (line != prev_line) {
   if (prev_line != "") {
    if (show_count) {
          if (!(only_dups || only_uniq)) os << count << ' ';
     else if (count > 1 && only_dups)    os << count << ' ';
     else if (count == 1 && only_uniq)   os << count << ' ';
    }

         if (!(only_dups || only_uniq)) os << prev_line << '\n';
    else if (count > 1 && only_dups)    os << prev_line << '\n';
    else if (count == 1 && only_uniq)   os << prev_line << '\n';
   }

   prev_line = line;
   count = 0;
  }

  count++;
 }

 if (prev_line != "") {
  if (show_count) {
        if (!(only_dups || only_uniq)) os << count << ' ';
   else if (count > 1 && only_dups)    os << count << ' ';
   else if (count == 1 && only_uniq)   os << count << ' ';
  }

       if (!(only_dups || only_uniq)) os << prev_line << '\n';
  else if (count > 1 && only_dups)    os << prev_line << '\n';
  else if (count == 1 && only_uniq)   os << prev_line << '\n';
 }
}

int main(int argc, char* argv[]) {
 if (argc == 1) {
  std::cout << "Usage: uniq [{in}|-] [out]" << std::endl;

  return 1;
 }

 std::vector<std::string> args;
 for (int i = 1; i < argc; i++) {
  args.push_back(std::string(argv[i]));
 }

 std::string in_name, out_name;
 bool show_count = false;
 bool only_dups  = false;
 bool only_uniq  = false;
 for (std::string arg : args) {
  if (arg[0] == '-') {
   show_count = show_count || arg == "-c" || arg == "--count";
   only_dups  = only_dups  || arg == "-d" || arg == "--repeated";
   only_uniq  = only_uniq  || arg == "-u" || arg == "--unique";

   if (arg == "-") in_name = "-";
  } else if (in_name != "") {
   out_name = arg;
  } else in_name = arg;
 }

 std::istream *is = &std::cin;
 std::ostream *os = &std::cout;

 if (in_name == "") {
  std::cerr << "No input file provided" << std::endl;
  
  return 1;
 } else if (in_name != "-") {
  std::ifstream* in = new std::ifstream(in_name);
  if (!in->is_open()) {
   std::cerr << "Could not open file: " << in_name << std::endl;
 
   return 1;
  }

  is = in;
 }

 if (out_name != "") {
  std::ofstream* out = new std::ofstream(out_name);
  if (!out->is_open()) {
   std::cerr << "Could not open file: " << out_name << std::endl;
 
   return 1;
  }

  os = out;
 }
 
 readData(*is, *os, show_count, only_dups, only_uniq);
 
 return 0;
}