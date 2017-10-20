#ifndef MAUTILITY_H_INCLUDED
#define MAUTILITY_H_INCLUDED

#include <vector>
#include <string>


std::vector<std::string> split(const char *str, char c = ' ', bool wantEmptyTokens = false);
std::string find_token(std::string s, const char * name);

std::string get_clipboard();
void copy_clipboard(std::string s);


#endif // MAUTILITY_H_INCLUDED
