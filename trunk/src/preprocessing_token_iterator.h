#ifndef __preprocessing_token_iterator_h__
#define __preprocessing_token_iterator_h__

#include <parsing_iterator_adaptor.h>

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

    result_type parse()
    {
        base_type i(this->base_reference());
        token::file_position_t p(this->base_reference().get_position());

        if (iswspace(*i)) {
            wstring spelling;
            while (i != this->end_reference() && iswspace(*i)) {
                spelling += *i;
                ++i;
            }
            return result_type(token(token::WHITESPACE, spelling, p), i);
        }

        //FIXME:
    }
};

}

#endif // ndef __preprocessing_token_iterator_h__
