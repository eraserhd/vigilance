#include <parsing_algorithms.h>
#include <cwctype>
using namespace std;

#include "identifier_characters.h"

namespace vigilance {

bool is_identifier_character(wchar_t c)
{
    return IDENTIFIER_CHARACTERS[c];
}

bool is_identifier_first_character(wchar_t c)
{
    return !iswdigit(c) && IDENTIFIER_CHARACTERS[c];
}

}
