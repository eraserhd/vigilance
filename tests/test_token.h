
#include <token.h>
using namespace std;
using namespace vigilance;

class test_token : public CxxTest::TestSuite
{
public:
    void test_spelling_is_preserved()
    {
        token t(token::WHITESPACE, L" \t\n\v", token::file_position_t(L"foo.cpp",1,1));
        TS_ASSERT_EQUALS(L" \t\n\v", t.spelling());
    }

    void test_token_type_is_preserved()
    {
        token t(token::WHITESPACE, L" \t\n\v", token::file_position_t(L"foo.cpp",1,1));
        TS_ASSERT_EQUALS(token::WHITESPACE, t.type());
    }

    void test_position_is_preserved()
    {
        token t(token::WHITESPACE, L" \t\n\v", token::file_position_t(L"foo.cpp",1,1));
        TS_ASSERT(token::file_position_t(L"foo.cpp",1,1) == t.position());
    }

    void test_default_constructable()
    {
        token t;
    }
};
