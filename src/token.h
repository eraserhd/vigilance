#ifndef __token_h__
#define __token_h__

#include <boost/spirit/iterator.hpp>

namespace vigilance {

class token {
public:
    enum type_type {
        WHITESPACE = 1,
    };

    typedef boost::spirit::file_position_base<std::wstring> file_position_t;
private:
    type_type _type;
    std::wstring _spelling;
    file_position_t _position;

public:
    token();
    token(type_type type,
          std::wstring const& spelling,
          file_position_t const& position);

    type_type type() const;
    std::wstring const& spelling() const;
    file_position_t const& position() const;
};

}

#endif // ndef __token_h__
