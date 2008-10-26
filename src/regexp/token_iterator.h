#ifndef __regexp__token_iterator_h__
#define __regexp__token_iterator_h__

#include <regexp/token.h>

namespace vigilance {
namespace regexp {

template <typename IteratorType>
class token_iterator
    : public vigilance::parsing_iterator_adaptor<
          token_iterator<IteratorType>
        , IteratorType
        , token<typename std::iterator_traits<IteratorType>::value_type>
      >
{
public:
    token_iterator()
        : token_iterator::parsing_iterator_adaptor_()
    {}

    template <typename OtherIterator>
    token_iterator(
            OtherIterator begin,
            OtherIterator end,
            typename boost::enable_if_convertible<OtherIterator, IteratorType>::type* = 0)
        : token_iterator::parsing_iterator_adaptor_(begin, end)
    {}

    typedef typename token_iterator::parsing_iterator_adaptor_::base_type base_type;
    typedef typename token_iterator::parsing_iterator_adaptor_::value_type value_type;

private:
    friend class token_iterator::parsing_iterator_adaptor_;

    pair<value_type, base_type> make_result(typename value_type::type_type token_type)
    {
        base_type i(this->base_reference());
        value_type t(token_type, *i);
        ++i;
        return pair<value_type, base_type>(t, i);
    }

    pair<value_type, base_type> parse()
    {
        base_type i(this->base_reference());
        switch (*i) {
        case L'\\':
            {
                ++i;
                if (i == this->end_reference())
                    return make_result(value_type::LITERAL);

                value_type t(value_type::LITERAL, *i);
                ++i;
                return pair<value_type, base_type>(t, i);
            }
    
        case L'(':
            return make_result(value_type::OPEN_GROUP);
        case L')':
            return make_result(value_type::CLOSE_GROUP);
        case L'[':
            return make_result(value_type::OPEN_SET);
        case L']':
            return make_result(value_type::CLOSE_SET);
        case L'|':
            return make_result(value_type::ALTERNATE);
        case L'*':
            return make_result(value_type::KLEENE_STAR);
        case L'/':
            return make_result(value_type::LOOKAHEAD);
        default:
            return make_result(value_type::LITERAL);
        }
    }
};

}
}

#endif // ndef __regexp__token_iterator_h__
