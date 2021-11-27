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

#ifndef SERIALIZER_H
#define SERIALIZER_H

#if defined(MA_BLUE) && !defined(MA_BLUE_PC)

#include <Arduino.h>

#include <SD.h>
#include <SPI.h>

#else

#include <cstdlib>
#include <fstream>
#include <iostream>

#endif

//#include <fstream>
//#include <iomanip>
//#include <iostream>
#include <iterator>
#include <sstream>
//#include <vector>

/*
    TODO:

    For AVR/ARM builds, avoid using << operators and call Save() directly.

    The output stream can be stored in the base class, still opened and closed
    by WriteFile().

    It can be anything we want, I guess, but take it out of the function parameters.

    Load() uses an istream_iterator<string>, which avoids searching for white space,
    but I guess we're going to have to do that ourselves for AVR/ARM. At least there
    are only two functions that are affected, I think: ParseField() and ParseFile().
*/

class Serializer
{
    public:

        friend std::ostream& operator << (std::ostream& os, const Serializer& ser);

        enum ser_type_t
        {
            ser_reserved,
            ser_second_before_first,
            ser_george,
            ser_first_thing,
            ser_smaller_thing,
            ser_another,
            ser_something_else
        };

        Serializer();
        Serializer(ser_type_t t):
            m_SerType(t)
        {}

        virtual ~Serializer();

        ser_type_t Type() { return m_SerType; }

        int Load(const char * file_name);
        int WriteFile(const char * file_name);

        virtual Serializer * CreateObject(int idObj) = 0;
        virtual void StoreObject(Serializer * object) = 0;
        virtual void StoreField(const char * s, int idField) = 0;
        virtual void Save() const = 0;


        // Operator << is a friend function, not a virtual member function,
        // and won't be overridden in the derived class. Hence need to call
        // something that is a virtual member.

//        friend std::ostream & operator << (std::ostream & of, const Serializer & ser)
//        {
//            return (&ser)->Save();
//        }

    protected:
        // To make sure object/field IDs are unique - this may improve data safety when loading
        // from text files - store all IDs object ID x N + field ID.

        // On the MCU, assume object/field ID has to fit into 32 bit unsigned int. A reasonable
        // value is 128, which gives 128 stored data elements per object and 512 stored object types.

        unsigned int m_SerTypeMultiplier = 128;

        ser_type_t m_SerType;
        std::istream_iterator<std::string> m_Eos;

        std::string ParseString();

        void WriteLineFeed() const
        {
            WriteField("\n");
        }

        unsigned int FieldPrefix(unsigned int idField) const
        {
            return m_SerType * m_SerTypeMultiplier + idField;
        }

        std::ostream & WriteObjectHeader(std::ostream & os) const
        {
            return os << "\n01:" << m_SerType << '\n';
        }

        void WriteObjectHeader() const
        {
            std::stringstream s;
            s << "\n01:" << m_SerType << '\n';
            WriteField(s.str().c_str());
        }

        std::ostream & WriteObjectTail(std::ostream & os) const
        {
            return os << "02:" << m_SerType << '\n';
        }

        void WriteObjectTail() const
        {
            std::stringstream s;
            s << "02:" << m_SerType << '\n';
            WriteField(s.str().c_str());
        }

        std::ostream & WriteField(std::ostream & os, unsigned int id, int i) const
        {
            return os << FieldPrefix(id) << ':' << i << ' ';
        }

        void WriteField(unsigned int id, int i) const
        {
            std::stringstream s;
            s << FieldPrefix(id) << ':' << i << ' ';
            WriteField(s.str().c_str());
        }

        std::ostream & WriteField(std::ostream & os, unsigned int id, double n) const
        {
            return os << FieldPrefix(id) << ':' << n << ' ';
        }

        void WriteField(unsigned int id, double n) const
        {
            std::stringstream s;
            s << FieldPrefix(id) << ':' << n << ' ';
            WriteField(s.str().c_str());
        }

        std::ostream & WriteField(std::ostream & os, unsigned int id, const char * s) const
        {
            return os << FieldPrefix(id) << ":\"" << s << "\" ";
        }

        void WriteField(unsigned int id, const char * field) const
        {
            std::stringstream s;
            s << FieldPrefix(id) << ":\"" << field << "\" ";
            WriteField(s.str().c_str());
        }

    private:
        int ParseFile();
        bool ParseField(std::string & s, unsigned int & id);

        bool OpenFile(const char * file_name, bool for_write = false);
        void WriteField(const char * s) const;
        void CloseFile();

#if defined(MA_BLUE_PC)
        static FILE * m_pFile;
#elif defined(MA_BLUE)
        static File m_File;
#else
        static std::fstream m_fstream;
#endif

};

#endif // SERIALIZER_H
