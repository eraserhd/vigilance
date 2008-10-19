#ifndef __token_h__
#define __token_h__

#include <algorithm>
#include <iterator>
#include <vector>
#include <boost/spirit/iterator/position_iterator.hpp>

namespace cpp {

class token {
private:
    typedef std::pair<wchar_t, boost::spirit::file_position> source_character_type;
    typedef std::vector<source_character_type> source_characters_type;

    source_characters_type _source_characters;

public:
    template<typename IteratorT>
    token(IteratorT begin, IteratorT end)
    {
        std::copy(begin, end, back_inserter(_source_characters));
    }

    std::wstring spelling() const;

};

}

#endif // ndef __token_h__
