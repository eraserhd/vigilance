#ifndef __parsing_iterator_adaptor_h__
#define __parsing_iterator_adaptor_h__

#include <boost/spirit/iterator.hpp>

namespace vigilance {

using boost::spirit::file_position;

template <
      typename Derived
    , typename Base
    , typename Value = boost::use_default
  >
class parsing_iterator_adaptor
    : public boost::iterator_adaptor<
          Derived
        , typename boost::mpl::if_<
              boost::is_convertible<
                  typename boost::iterator_traversal<Base>::type
                , boost::forward_traversal_tag
              >
            , Base
            , boost::spirit::multi_pass<
                  Base
                , boost::spirit::multi_pass_policies::input_iterator
                , boost::spirit::multi_pass_policies::first_owner
                , boost::spirit::multi_pass_policies::no_check
                , boost::spirit::multi_pass_policies::std_deque
              >
          >::type
       , Value
       , boost::incrementable_traversal_tag
    >
{
public:
    parsing_iterator_adaptor()
        : parsing_iterator_adaptor::iterator_adaptor_(), _is_end(true)
    {
    }

    template <typename OtherType>
    parsing_iterator_adaptor(
            OtherType begin,
            OtherType end,
            typename boost::enable_if_convertible<OtherType, Base>::type* = 0)
        : parsing_iterator_adaptor::iterator_adaptor_(begin),
          _end(end), _is_end(false)
    {
        do_parse();
    }

    typedef typename parsing_iterator_adaptor::iterator_adaptor_::base_type base_type;
    typedef typename std::iterator_traits<base_type>::value_type value_type;

    void set_position(file_position const& p)
    {
        this->base_reference().set_position(p);
    }

    file_position const& get_position() const
    {
        return this->base_reference().get_position();
    }

    void set_tabchars(int n)
    {
        return this->base_reference().set_tabchars(n);
    }

protected:
    typedef parsing_iterator_adaptor parsing_iterator_adaptor_;

    Base const& end_reference() const
    {
        return _end;
    }

    Base& end_reference()
    {
        return _end;
    }

private:
    friend class boost::iterator_core_access;

    base_type _end;
    bool _is_end;
    value_type _value;
    base_type _next;

    Derived& derived()
    {
        return *static_cast<Derived *>(this);
    }

    Derived const& derived() const
    {
        return *static_cast<Derived const*>(this);
    }

    bool is_end() const
    {
        return _is_end || this->base_reference() == _end;
    }

    bool equal(const Derived& rhs) const
    {
        return (this->is_end() && rhs.is_end()) || this->base_reference() == rhs.base_reference();
    }

    void do_parse()
    {
        if (this->base_reference() == _end)
            return;

        pair<value_type,base_type> result(derived().parse());
        _value = result.first;
        _next = result.second;
    }

    void increment()
    {
        while (this->base_reference() != _next)
            ++ this->base_reference();

        do_parse();
    }

    value_type dereference() const
    {
        return _value;
    }
};

}

#endif // ndef __parsing_iterator_adaptor_h__
