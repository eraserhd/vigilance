#ifndef __trigraph_decoding_iterator_h__
#define __trigraph_decoding_iterator_h__

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/spirit/iterator.hpp>
#include <boost/type_traits.hpp>
#include <parsing_iterator_adaptor.h>

namespace vigilance {

template <typename IteratorType>
class trigraph_decoding_iterator
    : public parsing_iterator_adaptor<
          trigraph_decoding_iterator<IteratorType>
        , IteratorType
    >
{
public:
    trigraph_decoding_iterator()
        : trigraph_decoding_iterator::parsing_iterator_adaptor_()
    {
    }

    template <typename OtherIterator>
    trigraph_decoding_iterator(
            OtherIterator begin,
            OtherIterator end,
            typename boost::enable_if_convertible<OtherIterator, IteratorType>::type* = 0)
        : trigraph_decoding_iterator::parsing_iterator_adaptor_(begin, end)
    {
    }

    typedef typename trigraph_decoding_iterator::parsing_iterator_adaptor_::base_type base_type;

private:
    friend class trigraph_decoding_iterator::parsing_iterator_adaptor_;

    pair<wchar_t, base_type> parse()
    {
        base_type i(this->base_reference());

        pair<wchar_t, base_type> result(*this->base_reference(), i);
        ++result.second;
        ++i;

        if (result.first != L'?')
            return result;

        if (i == this->end_reference() || *i != L'?')
            return result;

        ++i;
        if (i == this->end_reference())
            return result;

        switch (*i) {
        case L'=':
            result.first = L'#';
            break;
        case L'/':
            result.first = L'\\';
            break;
        case L'(':
            result.first = L'[';
            break;
        case L')':
            result.first = L']';
            break;
        case L'<':
            result.first = L'{';
            break;
        case L'>':
            result.first = L'}';
            break;
        case L'!':
            result.first = L'|';
            break;
        case L'-':
            result.first = L'~';
            break;
        case L'\'':
            result.first = L'^';
            break;
        default:
            return result;
        }

        ++i;
        result.second = i;
        return result;
    }
};

}

#endif // ndef __trigraph_decoding_iterator_h__
