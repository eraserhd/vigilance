#ifndef __regex_parser_h__
#define __regex_parser_h__

#include <string>
#include <boost/shared_ptr.hpp>
#include <regexp/token_iterator.h>

namespace vigilance {
namespace regexp {

template <typename CharacterType>
class node {
public:
    typedef CharacterType character_type;
    typedef node<character_type> node_;

public:
    virtual ~node() {}
    virtual std::string repr() const = 0;
};

template <typename CharacterType>
class binary_node : public node<CharacterType> {
public:
    typedef CharacterType character_type;
    typedef boost::shared_ptr<node<character_type> > shared_ptr_type;
    typedef binary_node binary_node_;

private:
    shared_ptr_type _left;
    shared_ptr_type _right;

public:
    binary_node(shared_ptr_type const& left, shared_ptr_type const& right)
        : _left(left), _right(right)
    {}

    shared_ptr_type const& left() const
    {
        return _left;
    }

    shared_ptr_type const& right() const
    {
        return _right;
    }
};

template <typename CharacterType>
class cat_node : public binary_node<CharacterType> {
public:
    cat_node(boost::shared_ptr<node<CharacterType> > const& left,
             boost::shared_ptr<node<CharacterType> > const& right)
        : cat_node::binary_node_(left, right)
    {}

    virtual std::string repr() const
    {
        return "(cat " + this->left()->repr() + " " + this->right()->repr() + ")";
    }
};

template <typename CharacterType>
class or_node : public binary_node<CharacterType> {
public:
    or_node(boost::shared_ptr<node<CharacterType> > const& left,
            boost::shared_ptr<node<CharacterType> > const& right)
        : or_node::binary_node_(left, right)
    {}

    virtual std::string repr() const
    {
        return "(or " + this->left()->repr() + " " + this->right()->repr() + ")";
    }
};

template <typename CharacterType>
class positional_node : public node<CharacterType> {
public:
    typedef CharacterType character_type;

private:
    character_type _value;

public:
    positional_node(character_type const& value)
        : _value(value)
    {}

    character_type value() const
    {
        return _value;
    }

    virtual std::string repr() const
    {
        string value_repr;
        value_repr += (char)value();
        return "#" + value_repr;
    }
};

template <typename CharacterType>
class terminal_node : public node<CharacterType> {
public:
    virtual std::string repr() const
    {
        return "$";
    }
};

template <typename CharacterType>
class empty_node : public node<CharacterType> {
public:
    virtual std::string repr() const
    {
        return "(empty)";
    }
};

template <typename CharacterType>
class kleene_node : public node<CharacterType> {
public:
    typedef CharacterType character_type;
    typedef boost::shared_ptr<node<character_type> > shared_ptr_type;
private:
    shared_ptr_type inner;

public:
    kleene_node(shared_ptr_type const& inner)
        : inner(inner)
    {}

    virtual std::string repr() const
    {
        return "(kleene " + inner->repr() + ")";
    }
};

template <typename IteratorType>
class parser {
public:
    typedef IteratorType iterator_type;
    typedef typename std::iterator_traits<IteratorType>::value_type character_type;
    typedef node<character_type> node_type;
    typedef boost::shared_ptr<node_type> pointer_type;

private:
    iterator_type begin, end;

public:
    parser(iterator_type const& begin, iterator_type const& end)
        : begin(begin), end(end)
    {}

    bool parse_positional(pointer_type& result, iterator_type& begin)
    {
        if (begin == end)
            return false;

        iterator_type i(begin);
        switch (*i) {
        case '(': case ')': case '[': case ']': case '|': case '/': case '*':
            return false;

        case '\\':
            ++i;
            if (i == end) {
                result = pointer_type(new positional_node<character_type>('\\'));
                begin = i;
                return true;
            }
            result = pointer_type(new positional_node<character_type>(*i));
            ++i;
            begin = i;
            return true;
        
        default:
            result = pointer_type(new positional_node<character_type>(*i));
            ++i;
            begin = i;
            return true;
        }
    }

    bool parse_positional_with_optional_kleene(pointer_type& result, iterator_type& begin)
    {
        if (begin == end)
            return false;

        if (!parse_positional(result, begin))
            return false;

        if (begin == end)
            return true;

        if ('*' == *begin) {
            result = pointer_type(new kleene_node<character_type>(result));
            ++begin;
        }

        return true;
    }

    bool parse_cat_expression(pointer_type& result, IteratorType& begin)
    {
        if (!parse_positional_with_optional_kleene(result, begin))
            return false;

        pointer_type next;
        while (parse_positional_with_optional_kleene(next, begin))
            result = pointer_type(new cat_node<character_type>(result, next));

        return true;
    }

    bool parse_or_expression(pointer_type& result, IteratorType& begin)
    {
        if (!parse_cat_expression(result, begin)) {
            if (begin == end || *begin != '|')
                return false;
            result = pointer_type(new empty_node<character_type>());
        }

        pointer_type next;
        while (begin != end && *begin == '|') {
            ++begin;
            if (!parse_cat_expression(next, begin))
                next = pointer_type(new empty_node<character_type>());

            result = pointer_type(new or_node<character_type>(result, next));
        }

        return true;
    }

    pointer_type parse()
    {
        pointer_type result;
        bool empty = !parse_or_expression(result, begin);

        if (begin != end) {
            throw 1; // FIXME:
        }

        if (empty)
            result = pointer_type(new terminal_node<character_type>());
        else {
            result = pointer_type(new cat_node<character_type>(
                        result,
                        pointer_type(new terminal_node<character_type>())
                        ));
        }
        return result;
    }

};


template <typename IteratorType>
boost::shared_ptr<node<typename std::iterator_traits<IteratorType>::value_type> > parse(
        IteratorType begin,
        IteratorType end
        )
{
    parser<IteratorType> p(begin, end);
    return p.parse();
}

}
}

#endif // ndef __regex_parser_h__
