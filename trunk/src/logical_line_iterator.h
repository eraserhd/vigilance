#ifndef __logical_line_iterator_h__
#define __logical_line_iterator_h__

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/spirit/iterator.hpp>

namespace cpp {

template <typename IteratorType>
class logical_line_iterator
    : public boost::iterator_adaptor<
        logical_line_iterator<IteratorType>
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
    logical_line_iterator()
        : logical_line_iterator::iterator_adaptor_(), _is_end(true)
    {
    }

    template <typename OtherType>
    logical_line_iterator(
            OtherType begin,
            OtherType end,
            typename boost::enable_if_convertible<OtherType, IteratorType>::type* = 0)
        : logical_line_iterator::iterator_adaptor_(begin),
          _end(end), _is_end(false)
    {
        check_for_escaped_newline();
    }

    typedef typename logical_line_iterator::iterator_adaptor_::base_type base_type;
    typedef typename std::iterator_traits<base_type>::value_type value_type;

private:
    friend class boost::iterator_core_access;

    base_type _end;
    bool _is_end;

    bool at_end() const
    {
        return _is_end || this->base_reference() == _end;
    }

    bool equal(const logical_line_iterator<IteratorType>& rhs) const
    {
        return (this->at_end() && rhs.at_end()) || this->base_reference() == rhs.base_reference();
    }

    void check_for_escaped_newline()
    {
        base_type i(this->base_reference());

        if (i == _end)
            return;
        if (*i != L'\\')
            return;
        ++i;
        if (i == _end)
            return;
        if (*i != L'\n')
            return;

        ++ this->base_reference();
        ++ this->base_reference();
    }

    void increment()
    {
        ++ this->base_reference();
        check_for_escaped_newline();
    }
};

}

#endif // ndef __logical_line_iterator_h__
