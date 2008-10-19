#ifndef __source_character_iterator_h__
#define __source_character_iterator_h__

#include <parsing_iterator_adaptor.h>
#include <utility>

namespace cpp {

template <typename IteratorType>
class source_character_iterator
    : public parsing_iterator_adaptor<
          source_character_iterator<IteratorType>
        , IteratorType
    >
{
public:
    source_character_iterator()
        : source_character_iterator::parsing_iterator_adaptor_()
    {
    }

    template <typename OtherIterator>
    source_character_iterator(
            OtherIterator begin,
            OtherIterator end,
            typename boost::enable_if_convertible<OtherIterator, IteratorType>::type* = 0)
        : source_character_iterator::parsing_iterator_adaptor_(begin, end)
    {
    }

    typedef typename source_character_iterator::iterator_adaptor_::base_type base_type;

private:
    friend class source_character_iterator::parsing_iterator_adaptor_;

    struct parse_method {
        base_type const& _begin;
        base_type const& _end;
        base_type _i;
        int _width;
        wchar_t _c;

        inline parse_method(base_type const& begin, base_type const& end)
            : _begin(begin), _end(end), _i(_begin)
        {
        }

        inline static wchar_t hex_digit_value(wchar_t c)
        {
            if (iswdigit(c))
                return c-L'0';
            else
                return towlower(c)-L'a'+10;
        }

        inline std::pair<wchar_t, base_type> simple_character()
        {
            base_type i(_begin);
            ++i;
            return std::pair<wchar_t, base_type>(*_begin, i);
        }

        inline bool parse_hex_digits()
        {
            _c = 0;
            for (int j = 0; j < _width; ++j, ++_i) {
                if (_i == _end || !iswxdigit(*_i))
                    return false;

                _c = (_c<<4)|hex_digit_value(*_i);
            }
            return true;
        }


        inline std::pair<wchar_t, base_type> parse()
        {
            if (*_i != L'\\')
                return simple_character();
            ++_i;

            if (_i == _end || (*_i != L'u' && *_i != L'U'))
                return simple_character();

            _width = (*_i == L'u') ? 4 : 8;
            ++_i;

            if (!parse_hex_digits())
                return simple_character();
            return std::pair<wchar_t, base_type>(_c, _i);
        }
    };

    std::pair<wchar_t, base_type> parse() const
    {
        return parse_method(this->base_reference(), this->end_reference()).parse();
    }
};

}

#endif // ndef __source_character_iterator_h__
