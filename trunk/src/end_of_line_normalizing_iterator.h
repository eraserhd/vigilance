#ifndef __end_of_line_normalizing_iterator_h__
#define __end_of_line_normalizing_iterator_h__

#include <boost/iterator/iterator_adaptor.hpp>

namespace cpp {

//FIXME: concept-check: IteratorType must be a incrementable
template<typename IteratorType>
class end_of_line_normalizing_iterator
    : public boost::iterator_adaptor<
          end_of_line_normalizing_iterator<IteratorType>
        , IteratorType
        , boost::use_default
        , boost::incrementable_traversal_tag
    >
{
public:
    end_of_line_normalizing_iterator()
        : end_of_line_normalizing_iterator::iterator_adaptor_(),
          _is_end(true)
    {
    }

    template <typename OtherIterator>
    end_of_line_normalizing_iterator(
            OtherIterator begin,
            OtherIterator end,
            typename boost::enable_if_convertible<OtherIterator, IteratorType>::type* = 0)
        : end_of_line_normalizing_iterator::iterator_adaptor_(begin),
          _end(end), _is_end(false)
    {
    }

private:
    friend class boost::iterator_core_access;

    IteratorType _end;
    bool _is_end;

    bool is_end() const
    {
        return _is_end || this->base_reference() == _end;
    }

    bool equal(const end_of_line_normalizing_iterator<IteratorType>& rhs) const
    {
        return (is_end() && rhs.is_end()) || this->base_reference() == _end;
    }

    void increment()
    {
        typename std::iterator_traits<IteratorType>::value_type v = *this->base_reference();
        ++ this->base_reference();
        if (!is_end() && v == L'\r' && *this->base_reference() == L'\n')
            ++ this->base_reference();
    }

    typename std::iterator_traits<IteratorType>::value_type dereference() const
    {
        if (*this->base_reference() == L'\r')
            return L'\n';
        return *this->base_reference();
    }
};

}

#endif // ndef __end_of_line_normalizing_iterator_h__
