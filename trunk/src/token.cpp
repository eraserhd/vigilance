#include <string>
#include <token.h>
using namespace std;
using namespace vigilance;

token::token()
{
}

token::token(type_type type,
             wstring const& spelling,
             file_position_t const& position)
    : _type(type),
      _spelling(spelling),
      _position(position)
{
}

token::type_type token::type() const
{
    return _type;
}

wstring const& token::spelling() const
{
    return _spelling;
}

token::file_position_t const& token::position() const
{
    return _position;
}
