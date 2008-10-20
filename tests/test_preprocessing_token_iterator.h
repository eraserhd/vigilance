
#include <token.h>
#include <preprocessing_token_iterator.h>
#include <boost/spirit/iterator.hpp>
#include <boost/foreach.hpp>
using namespace std;
using namespace vigilance;

struct fluent_proxy {
    token const& _t;
    fluent_proxy(token const& t) : _t(t) {}

    fluent_proxy is_spelled(wstring const& spelling) {
        TS_ASSERT_EQUALS(spelling, _t.spelling());
        return *this;
    }

    fluent_proxy has_type(token::type_type tt) {
        TS_ASSERT_EQUALS(tt, _t.type());
        return *this;
    }
    
    fluent_proxy at_column(int n) {
        TS_ASSERT_EQUALS(n, _t.position().column);
        return *this;
    }
};

class test_preprocessing_token_iterator : public CxxTest::TestSuite
{
    typedef boost::spirit::position_iterator<wstring::const_iterator> pos_iterator_t;
    typedef preprocessing_token_iterator<pos_iterator_t> iterator_t;

    wstring _source;
    vector<token> _tokens;

    void assert_token_spellings_match_source()
    {
        wstring total_spelling;
        BOOST_FOREACH(token t, _tokens) {
            total_spelling += t.spelling();
        }

        TS_ASSERT_EQUALS(_source, total_spelling);
    }

    void with_input(const wstring& s)
    {
        _source = s;

        pos_iterator_t p_begin(
                s.begin(),
                s.end(),
                token::file_position_t(L"foo.cpp",1,1)
                );
        pos_iterator_t p_end;

        iterator_t begin(p_begin, p_end);
        iterator_t end;

        _tokens.clear();
        copy(begin, end, back_inserter(_tokens));

        assert_token_spellings_match_source();
    }

    fluent_proxy token_(int n = 1) const {
        TS_ASSERT(_tokens.size()>=n);
        return fluent_proxy(_tokens[n-1]);
    }

public:

    void test_correctly_tokenizes_whitespace()
    {
        with_input(L" \t\n\v\fHello");

        token_(1)
            .has_type(token::PP_WHITESPACE)
            .is_spelled(L" \t\n\v\f")
            .at_column(1);

        token_(2)
            .at_column(3);
    }

    void test_correctly_tokenizes_identifers()
    {
        with_input(L"Hello world");
        
        token_(1)
            .has_type(token::PP_IDENTIFIER)
            .is_spelled(L"Hello")
            .at_column(1);

        token_(2)
            .at_column(6);
    }

};
