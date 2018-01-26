//
//    MultiArp - Another step in the Great Midi Adventure!
//    Copyright (C) 2017, 2018  Barry Neilsen
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef MAUTILITY_H_INCLUDED
#define MAUTILITY_H_INCLUDED

#include <vector>
#include <string>


std::vector<std::string> split(const char *str, char c = ' ', bool wantEmptyTokens = false);
std::string find_token(std::string s, const char * name);

std::string get_clipboard();
void copy_clipboard(std::string s);

int64_t lcm(int64_t a, int64_t b);
int64_t gcd(int64_t a, int64_t b);

double tidy(double val, int dp = 3);
double tidy_3(double val);

bool equals(double val1, double val2, int dp = 3);
bool equals_3(double val1, double val2);




#endif // MAUTILITY_H_INCLUDED
