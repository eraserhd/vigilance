#ifndef __source_character_iterator_h__
#define __source_character_iterator_h__

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/spirit/iterator.hpp>
#include <utility>

namespace cpp {

template <typename IteratorType>
class source_character_iterator
    : public boost::iterator_adaptor<
          source_character_iterator<IteratorType>
        , typename boost::mpl::if_<
              boost::is_convertible<
                  typename boost::iterator_traversal<IteratorType>::type
                , boost::forward_traversal_tag
              >
            , IteratorType
            , boost::spirit::multi_pass<
                  IteratorType
                , boost::spirit::multi_pass_policies::input_iterator
                , boost::spirit::multi_pass_policies::first_owner
                , boost::spirit::multi_pass_policies::no_check
                , boost::spirit::multi_pass_policies::std_deque
              >
          >::type
        , boost::use_default
        , boost::incrementable_traversal_tag
    >
{
public:
    source_character_iterator()
        : source_character_iterator::iterator_adaptor_(),
          _is_end(true)
    {
    }

    template <typename OtherIterator>
    source_character_iterator(
            OtherIterator begin,
            OtherIterator end,
            typename boost::enable_if_convertible<OtherIterator, IteratorType>::type* = 0)
        : source_character_iterator::iterator_adaptor_(begin),
          _end(end), _is_end(false)
    {
    }

    typedef typename source_character_iterator::iterator_adaptor_::base_type base_type;
    typedef typename std::iterator_traits<base_type>::value_type value_type;

private:
    friend class boost::iterator_core_access;

    base_type _end;
    bool _is_end;

    bool is_end() const
    {
        return _is_end || this->base_reference() == _end;
    }

    bool equal(const source_character_iterator<IteratorType>& rhs) const
    {
        return (is_end() && rhs.is_end()) || this->base_reference() == _end;
    }

    std::pair<int, wchar_t> parse() const
    {
        base_type i(this->base_reference());
        std::pair<int, wchar_t> result(1,*i);

        if (*i != L'\\')
            return result;

        ++i;
        if (i == _end || (*i != L'u' && *i != L'U'))
            return result;

        int width = (*i == L'u') ? 4 : 8;
        ++i;

        wchar_t c = 0;
        for (int j = 0; j < width; ++j, ++i) {
            if (!iswxdigit(*i))
                return result;

            c<<=4;
            if (iswdigit(*i))
                c |= *i-L'0';
            else
                c |= towlower(*i)-L'a'+10;
        }

        result.first = 2+width;
        result.second = c;
        return result;
    }

    void increment()
    {
        pair<int, wchar_t> r(parse());

        for (int i = 0; i < r.first; ++i)
            ++ this->base_reference();
    }

    value_type dereference() const
    {
        return parse().second;
    }

};

}

#endif // ndef __source_character_iterator_h__
