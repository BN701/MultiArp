#ifndef MACOMMAND_H_INCLUDED
#define MACOMMAND_H_INCLUDED

#include <string>

void navigate_pattern_store(int key);

void do_command_line(int argc, char *argv[]);
bool do_command(std::string commandList);

#define RESET_NONE 0
#define RESET_ALL 1
#define RESET_CURRENT 2


#endif // MACOMMAND_H_INCLUDED
