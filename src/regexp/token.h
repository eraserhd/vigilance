#ifndef __regexp__token_h__
#define __regexp__token_h__

namespace vigilance {
namespace regexp {

template <typename CharacterType>
class token {
public:
    typedef CharacterType character_type;
    enum type_type {
        LITERAL,

        KLEENE_STAR,    // *
        OPEN_GROUP,     // (
        CLOSE_GROUP,    // )
        OPEN_SET,       // [
        CLOSE_SET,      // ]
        ALTERNATE,      // |
        LOOKAHEAD       // /
    };

private:
    character_type _character;
    type_type _type;

public:
    token()
    {}

    token(type_type type, character_type character)
        : _type(type), _character(character)
    {}

    type_type type() const { return _type; }
    character_type character() const { return _character; }
};

}
}

#endif // ndef __regexp__token_h__
