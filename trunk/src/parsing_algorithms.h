#ifndef __parsing_algorithms_h__
#define __parsing_algorithms_h__

#include <string>

namespace vigilance {

bool is_identifier_character(wchar_t c);
bool is_identifier_first_character(wchar_t c);

template <typename IteratorType, typename FunctorType>
std::basic_string<typename std::iterator_traits<IteratorType>::value_type> scan_while(IteratorType& begin, IteratorType const& end, FunctorType pred)
{
    std::basic_string<typename std::iterator_traits<IteratorType>::value_type> result;
    while (begin != end && pred(*begin)) {
        result += *begin;
        ++begin;
    }
    return result;
}

}

#endif // ndef __parsing_algorithms_h__
