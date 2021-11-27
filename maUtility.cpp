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

#include "maUtility.h"

#include <cmath>
#include <cstdlib>
#include <cstring>

#if defined(MA_BLUE) && !defined(MA_BLUE_PC)
//#include "maBaseUI.h"
#endif

using namespace std;

// PangoLayout* layout = pango_cairo_create_layout(cairo_t);

#if 0

/*
    Pango needs glib, which makes the build dependencies much more complicated.

    So far I've added these to compiler search:

    /usr/include/pango-1.0
    /usr/include/glib-2.0
    /usr/lib/x86_64-linux-gnu/glib-2.0/include

    This to linker search:

    /usr/lib/x86_64-linux-gnu

    These to linker options:

    -lpango-1.0
    -lpangocairo-1.0

     ... and I'm still stuck here:

    /usr/bin/ld: obj/Debug/maUtility.o||undefined reference to symbol 'g_type_check_instance_is_a'

    I tried adding -lglib-2.0 and a load of other vaguely related things but couldn't get past this.
*/
//#include "cairot.h"   // Don't know what this is ...
#include <cairo/cairo.h>
#include <pango/pangocairo.h>

void dump_stuff(PangoLayout* layout) {
   PangoContext* context = pango_layout_get_context(layout);
   PangoFontFamily** families; int n;
   pango_context_list_families(context, &families, &n);
   printf("%d families:\n", n);
   for (int i=0; i < n; i++) {
     PangoFontFamily* family = families[i];
     if (!PANGO_IS_FONT_FAMILY(family)) {
       printf("a %s\n", G_OBJECT_TYPE_NAME(family));
       continue;
     }
     printf("%s\n", pango_font_family_get_name(family));
     PangoFontFace** faces; int m;
     pango_font_family_list_faces(family, &faces, &m);
     for (int j = 0; j < m; j++) {
       PangoFontFace* face = faces[j];
       printf("  %s", pango_font_face_get_face_name(face));
       int* sizes; int nsizes;
       pango_font_face_list_sizes(face, &sizes, &nsizes);
       if (sizes) {
	for (int k = 0; k < nsizes; ++k) printf(" %d", sizes[k]);
	g_free(sizes);
       }
       if (pango_font_face_is_synthesized(face)) printf(" *");
       printf("\n");
     }
     g_free(faces);
   }
   g_free(families);
}

#endif

// This came from here: https://stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c

vector<string> split(const char *str, char c, bool wantEmptyTokens)
{
    vector<string> result;

    do
    {
        const char *begin = str;

        while (*str != c && *str)
            str++;

        if ( !result.empty() && result.back().back() == '\\')
        {
            result.back().resize(result.back().size() - 1);
            result.back() += string(begin, str);
        }
        else
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


#if !defined(MA_BLUE) || defined(MA_BLUE_PC)

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
#endif

uint64_t AlsaTimeStampToMicros(snd_seq_timestamp & t)
{
    return static_cast<uint64_t>(t.time.tv_sec) * 1000000 + t.time.tv_nsec/1000;
}

uint64_t EventTimeToMicros(snd_seq_event_t & ev)
{
    return AlsaTimeStampToMicros(ev.time);
}



int64_t gcd(int64_t a, int64_t b)
{
    for (;;)
    {
        if (a == 0) return b;
        b %= a;
        if (b == 0) return a;
        a %= b;
    }
}

int64_t lcm(int64_t a, int64_t b)
{
    int temp = gcd(a, b);

    return temp ? (a / temp * b) : 0;
}


double tidy(double val, int dp)
{
    double t = 1;
    for ( int i = 0; i < dp; i++ )
        t *= 10;
    return static_cast<double>(lround(t * val))/t;
}

double tidy_3(double val)
{
    return static_cast<double>(lround(1000.0 * val))/1000.0;
}

bool equals(double val1, double val2, int dp)
{
    double t = 1;
    for ( int i = 0; i < dp; i++ )
        t *= 10;
    return lround(t * val1) == lround(t * val2);
}

bool equals_3(double val1, double val2)
{
    return lround(1000.0 * val1) == lround(1000.0 * val2);
}



#if defined(MA_BLUE) && !defined(MA_BLUE_PC)
extern "C" char* sbrk(int incr);
extern char _estack;
extern uint32_t g_heap_0;  // Defined in main module, set when globals initialise.

void MemStats(uint32_t & heap_top, uint32_t & stack_bottom, uint32_t & free_mem)
{
    char top;
    heap_top = reinterpret_cast<uint32_t>(sbrk(0));// - 0x1FFFE000;
    stack_bottom = reinterpret_cast<uint32_t>(&top);// - 0x1FFFE000;

    free_mem = stack_bottom - heap_top;
    stack_bottom = reinterpret_cast<uint32_t>(&_estack) - stack_bottom;
    heap_top -= g_heap_0; // Observed start of heap, Jun 2018 with "13480 bytes of globals".

//    free_mem = &top - reinterpret_cast<char*>(sbrk(0));
}

uint32_t FreeMem()
{
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
#if 0
// for Teensy 3.0. Does it work anywhere else?
// And what if memory has become fragmented? The
// test allocation below could be anywhere.
    uint32_t stackTop = 0;
    uint32_t heapTop = 0;

    // current position of the stack.
    stackTop = (uint32_t) &stackTop;

    // current position of heap.
    void* hTop = malloc(1);
    heapTop = (uint32_t) hTop;
    free(hTop);
    // The difference is (approximately) the free, available ram.
    return stackTop - heapTop;
#endif
}

#ifdef __arm__
extern "C" char* sbrk(int incr);
int FreeRam() {
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}
#else  // __arm__
extern char *__brkval;
extern char __bss_end;
/** Amount of free RAM
 * \return The number of free bytes.
 */
int FreeRam() {
  char top;
  return __brkval ? &top - __brkval : &top - &__bss_end;
}
#endif  // __arm

#else
uint32_t FreeMem()
{
    // I don't have any code for other platforms.
    return 0;
}

void MemStats(uint32_t & heap_top, uint32_t & stack_bottom, uint32_t & free_mem)
{
    heap_top = 0;
    stack_bottom = 0;
    free_mem = 0;
}

#endif


