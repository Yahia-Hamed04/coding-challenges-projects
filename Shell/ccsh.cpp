#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

#define streql(s1, s2) !strcmp((s1), (s2))

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

std::vector<char *> split(std::string &str) {
 std::vector<char *> list;
 for (int i = 0; i < str.length(); i++) {
  int start = i, length = 0;
  if (str[i] == '"') {
   start = ++i;
   for (; i < str.length() && str[i] != '"'; i++) {
    length += str[i] != '"';
   } i++;
  } else if (str[i] != ' ') {
   for (; i < str.length() && str[i] != ' '; i++) {
    length += str[i] != ' ';
   }
  }

  if (length > 0) {
   char *s = new char[length + 1];
   std::strcpy(s, str.substr(start, length).c_str());
   list.push_back(s);
  }
 }

 list.push_back(NULL);
 return list;
}

std::vector<std::vector<char*>> split_commands(std::string &line) {
 std::vector<std::vector<char*>> commands;

 for (int i = 0; i < line.size(); i++) {
  int start = i, length = 0;

  for (; i < line.size() && line[i] != '|'; i++) {
   length++;
  }

  std::string command = line.substr(start, length);
  std::vector<char*> args_vec = split(trim(command));

  commands.push_back(args_vec);
 }

 return commands;
}

pid_t launch_process(char* args[], int fd[2], int orig_out, bool last) { 
 dup2(fd[0], 0); close(fd[0]);
 
 if (last) fd[1] = dup(orig_out);
 else pipe(fd);

 dup2(fd[1], 1); close(fd[1]);
 
 pid_t pid = fork();

 if (pid == 0) {
  signal(SIGINT, NULL);

  execvp(args[0], args);
 }

 return pid;
}

void launch_processes(std::vector<std::vector<char*>> commands) {
 if (commands.size() == 0) return;

 int tmpin  = dup(STDIN_FILENO);
 int tmpout = dup(STDOUT_FILENO);
 int fd[2];
 fd[0] = dup(tmpin);

 pid_t pid;
 int size = commands.size();
 for (int i = 0; i < size; i++) {
  if (streql(commands[i][0], "cd")) {
   chdir(commands[i][1]);
   continue;
  }
  
  pid = launch_process(commands[i].data(), fd, tmpout, i == size - 1);
 }

 dup2(tmpin,  0); close(tmpin);
 dup2(tmpout, 1); close(tmpout);
 waitpid(pid, NULL, 0);
}

void nothing(int signal) {}

int main() {
 signal(SIGINT, nothing);
 std::vector<std::string> command_history;

 while (true) {
  std::cout << "ccsh> ";
  std::string line; std::getline(std::cin, line);

  trim(line);
  if (line == "exit") return 0;
  command_history.push_back(line);

  std::vector<std::vector<char*>> commands = split_commands(line); 
  launch_processes(commands);
 }
}