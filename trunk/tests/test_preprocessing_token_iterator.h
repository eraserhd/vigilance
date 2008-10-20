
#include <token.h>
#include <preprocessing_token_iterator.h>
#include <boost/spirit/iterator.hpp>
using namespace std;
using namespace vigilance;

class test_preprocessing_token_iterator : public CxxTest::TestSuite
{
    token first_token(const wstring& s)
    {
        typedef boost::spirit::position_iterator<wstring::const_iterator> pos_iterator_t;
        typedef preprocessing_token_iterator<pos_iterator_t> iterator_t;

        pos_iterator_t p_begin(
                s.begin(),
                s.end(),
                token::file_position_t(L"foo.cpp",1,1)
                );
        pos_iterator_t p_end;

        iterator_t begin(p_begin, p_end);
        iterator_t end;

        return *begin;
    }

public:
    void test_correctly_tokenizes_whitespace()
    {
        token t(first_token(L" \t\rHello"));
        TS_ASSERT_EQUALS(token::WHITESPACE, t.type());
        TS_ASSERT_EQUALS(L" \t\r", t.spelling());
        TS_ASSERT(token::file_position_t(L"foo.cpp",1,1) == t.position());
    }
};
