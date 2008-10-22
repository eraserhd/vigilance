#ifndef __preprocessing_token_iterator_h__
#define __preprocessing_token_iterator_h__

#include <parsing_iterator_adaptor.h>
#include <parsing_algorithms.h>

namespace vigilance {

template <typename IteratorType>
class preprocessing_token_iterator
    : public parsing_iterator_adaptor<
          preprocessing_token_iterator<IteratorType>
        , IteratorType
        , token
    >
{
public:
    preprocessing_token_iterator()
        : preprocessing_token_iterator::parsing_iterator_adaptor_()
    {
    }

    template <typename OtherIterator>
    preprocessing_token_iterator(
            OtherIterator begin,
            OtherIterator end,
            typename boost::enable_if_convertible<OtherIterator, IteratorType>::type* = 0)
        : preprocessing_token_iterator::parsing_iterator_adaptor_(begin, end)
    {
    }

    typedef typename preprocessing_token_iterator::parsing_iterator_adaptor_::base_type base_type;

private:
    friend class preprocessing_token_iterator::parsing_iterator_adaptor_;

    typedef pair<token, base_type> result_type;

    struct single_token_parser {
        base_type i;
        base_type const& end;
        token::file_position_t p;
        wstring sp;

        bool accept_prefix(const wchar_t *p)
        {
            wstring prefix_sp;

            base_type j(i);
            while (*p) {
                if (j == end)
                    return false;
                if (*p != *j)
                    return false;

                prefix_sp += *j;
                ++p;
                ++j;
            }

            i=j;
            sp += prefix_sp;
            return true;
        }

        void accept_character()
        {
            sp += *i;
            ++i;
        }

        single_token_parser(base_type const& begin, base_type const& end)
            : i(begin), end(end), p(begin.get_position())
        {
        }
        
        token::type_type internal_parse()
        {
            if (iswspace(*i)) {
                sp = scan_while(i, end, iswspace);
                return token::PP_WHITESPACE;
            }
            if (is_identifier_first_character(*i)) {
                sp = scan_while(i, end, is_identifier_character);
                return token::PP_IDENTIFIER;
            }
            if (accept_prefix(L"/*")) {
                while (i != end && !accept_prefix(L"*/"))
                    accept_character();
                return token::PP_COMMENT;
            }

            accept_character();
            return token::PP_OTHER_CHAR;
        }

        result_type parse()
        {
            token::type_type tt = internal_parse();
            return result_type(token(tt, sp, p), i);
        }
    };

    result_type parse()
    {
        return single_token_parser(this->base_reference(), this->end_reference()).parse();
    }
};

}

#endif // ndef __preprocessing_token_iterator_h__
