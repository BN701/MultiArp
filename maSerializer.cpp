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

#include "maSerializer.h"
#include "maUtility.h"

//#if defined(MA_BLUE) && !defined(MA_BLUE_PC)
//
//#include <Arduino.h>
//
//#include <SD.h>
//#include <SPI.h>
//
//#else
//
//#include <cstdlib>
//#include <fstream>
//#include <iostream>
//
//#endif

using namespace std;

#if defined(MA_BLUE_PC)
FILE * Serializer::m_pFile;
#elif defined(MA_BLUE)
File Serializer::m_File;
#else
std::fstream Serializer::m_fstream;
#endif

Serializer::Serializer()
{
    //ctor
}

Serializer::~Serializer()
{
    //dtor
}

#if defined(MA_BLUE_PC)
bool Serializer::OpenFile(const char * file_name, bool for_write)
{
    m_pFile = fopen (file_name, for_write ? "w" : "r");
//            std::stringstream ss;
//            ss << "Serializer: open file for " << (for_write ? "write " : "read ") << (m_pFile != nullptr ? "OK" : "Failed");
    return m_pFile != nullptr;
}

void Serializer::WriteField(const char * s) const
{
    if ( m_pFile != nullptr )
    {
        fputs(s, m_pFile);
    }
}

void Serializer::CloseFile()
{
    if ( m_pFile != nullptr )
    {
        fclose (m_pFile);
        m_pFile = nullptr;
    }
}

#elif defined(MA_BLUE)

bool Serializer::OpenFile(const char * file_name, bool for_write)
{
    if ( for_write )
    {
        SD.remove(file_name);   // FILE_WRITE opens for append. (I haven't found the rewrite option.)
        m_File = SD.open(file_name, FILE_WRITE);
    }
    else
      m_File = SD.open(file_name, FILE_READ);
    std::stringstream ss;
    ss << "Serializer: open file \"" << file_name << "\", " << (for_write ? "write " : "read ") << (m_File ? "OK" : "Failed");
    Serial.println(ss.str().c_str());
    return m_File;
}

void Serializer::WriteField(const char * s) const
{
    m_File.print(s);
}

void Serializer::CloseFile()
{
    m_File.close();
}

#else

bool Serializer::OpenFile(const char * file_name, bool for_write)
{
    m_fstream.open(file_name, for_write ? ios_base::out : ios_base :: in);
    return m_fstream.good();
}

void Serializer::CloseFile()
{
    m_fstream.close();
}
#endif

int Serializer::Load(const char * file_name)
{
    OpenFile(file_name);
//    istream_iterator<string> ii {is};
//
//    ParseFile(ii);
    ParseFile();
    CloseFile();

    return 0;
}

int Serializer::WriteFile(const char * file_name)
{
    OpenFile(file_name, true);

    Save();

    CloseFile();

//    ofstream os {file_name};
//
//    os << *this;    // Could call Save() directly and not bother with <<.

    return 0;
}

string Serializer::ParseString()
{
    string s;
#if defined(MA_BLUE_PC)
    if ( m_pFile != nullptr )
    {
        int c;
        while ( (c = fgetc(m_pFile)) != EOF )
        {
            if ( c > ' ' )
                s += c;
            else if ( s.length() == 0 )
                continue;
            else
                break;
        }
    }
//    cout << s << '\n';
#elif defined(MA_BLUE)
    while ( m_File.available() )
    {
        int c = m_File.read();
        if ( c > ' ' )
            s += c;
        else if ( s.length() == 0 )
            continue;
        else
            break;
    }
//    Serial.println(s.c_str());
#else
    s = *(++m_fstream);
#endif

    return s;
}


bool Serializer::ParseField(string & t, unsigned int & idField)
{
    t = ParseString();

    if ( t.length() == 0 )
        return false;

    char * endptr;
    idField = strtol(t.c_str(), &endptr, 0);

    if ( *endptr != ':' )
        idField = 0;

    if ( idField == 0 )
        return false;

    t.erase(0, endptr + 1 - & t.front() );

    if ( t.front() == '"')
    {
        t.erase(t.begin());
        string u;
        while ( (u = ParseString()).length() != 0 )
        {
            t += ' ';
            t += u;
            if ( t.back() == '"' )
            {
                t.erase(t.end() - 1);
                break;
            }
        }
    }

    return t.length() > 0;
}

int Serializer::ParseFile()
{
#if false
#if defined(MA_BLUE) && !defined(MA_BLUE_PC)
    Serial.println("\n\nParseFile() new level ...");
#else
    cout << "\n\nParseFile() new level ...\n";
#endif
#endif


    static string indent = ">";
    stringstream ss;
    string s;

    indent.insert(0, "--");

    bool bQuit = false;
    unsigned int idField, idObj;
    while ( !bQuit && ParseField(s, idField) )
    {
        if ( idField == 0 )
            continue;

#if false
#if defined(MA_BLUE) && !defined(MA_BLUE_PC)
        ss.str("");
        ss << indent << idField << ", " << s;
        Serial.println(ss.str().c_str());
#else
        cout << indent << idField << ", " << s << '\n';
#endif
#endif
        switch ( idField )
        {
            case 1:     // Our own object header, which we ignore, or a child object.
                if ( (idObj = strtol(s.c_str(), NULL, 0)) != m_SerType )
                {
                    Serializer * newObject = CreateObject(idObj);
                    if ( newObject != nullptr )
                    {
                        newObject->ParseFile();
                        StoreObject(newObject);
                        delete newObject;
                    }
                    else
                    {
                        /*
                            Found an unexpected object. We'll just keep processing fields
                            until our own 'end of object' is found.
                        */
                    }
                }
                break;
            case 2:     // End of object. Make sure it's the current object's type.
                bQuit = strtol(s.c_str(), NULL, 0) == m_SerType;
                break;
            default:    // Process field
                StoreField(s.c_str(), idField % m_SerTypeMultiplier);
                break;
        }


    }

#if false
#if defined(MA_BLUE) && !defined(MA_BLUE_PC)
    Serial.println("ParseFile() returning ...");
    uint32_t heap_top, stack_bottom, free_mem;
    MemStats(heap_top, stack_bottom, free_mem);
    ss.str("");
    ss << "Stack: " << stack_bottom << " Heap: " << heap_top << " Free: " << free_mem << '\n';
    Serial.println(ss.str().c_str());
#else
    cout << "ParseFile() returning ...\n\n";
#endif
#endif
    indent.erase(0, 2);
    return 0;
}

