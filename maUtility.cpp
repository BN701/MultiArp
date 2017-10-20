//
//    MultiArp - Another step in the Great Midi Adventure!
//    Copyright (C) 2017  Barry Neilsen
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

#include "maUtility.h"

#include <cstring>

using namespace std;

// This came from here: https://stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c

vector<string> split(const char *str, char c, bool wantEmptyTokens)
{
    vector<string> result;

    do
    {
        const char *begin = str;

        while (*str != c && *str)
            str++;

        result.push_back(string(begin, str));

        if ( !wantEmptyTokens && result.back().size() == 0 )
            result.pop_back();

    }
    while (0 != *str++);

    return result;
}

string find_token(string s, const char * name)
{
    size_t pos = s.find(name);
    if ( pos == string::npos )
        return "";

    pos += strlen(name);

    char c;
    while ( c = s.at(pos++), c == ' ' );

    switch (c)
    {
    case '\'':
    case '\"':
        break;
    case '[':
        c = ']';
        break;
    case '(':
        c = ')';
        break;
    case '{':
        c = '}';
        break;
    default:
        c = ' ';
        pos--;
        break;
    }

    size_t pos_end = s.find(c, pos);

    if ( pos_end != string::npos )
        return s.substr(pos, pos_end - pos);
    else
        return s.substr(pos);
}



void copy_clipboard(std::string s)
{
    FILE *f = popen("xclip -i -selection clipboard", "w");

    if (f == NULL)
    {
        return;
    }

    fwrite(s.c_str(), sizeof(s[0]), s.size(), f);

    pclose(f);
}

std::string get_clipboard()
{
    FILE *f = popen("xclip -o -selection clipboard 2> /dev/null", "r");

    if (f == NULL)
    {
        return "";
    }

    std::string s;

    char buf[64];
    size_t readn;
    while ((readn = fread(buf, 1, sizeof(buf), f)) > 0)
    {
        s.append(buf, readn);
    }

    if ( s.size() == 0 )
        s = "Clipboard is empty.";

    pclose(f);

    return s;

}


