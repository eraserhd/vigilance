
#include <string>
#include <source_character_iterator.h>
using namespace std;
using namespace vigilance;

class test_source_character_iterator : public CxxTest::TestSuite
{
    wstring decode(const wstring& s)
    {
        typedef source_character_iterator<wstring::const_iterator> iterator_t;

        iterator_t begin(s.begin(), s.end());
        iterator_t end;

        wstring result;
        copy(begin, end, back_inserter(result));
        return result;
    }

public:
    void test_parses_u4_characters()
    {
        TS_ASSERT_EQUALS(L"\u1542", decode(L"\\" L"u1542"));
    }

    void test_parses_u8_characters()
    {
        TS_ASSERT_EQUALS(L"\U00011542", decode(L"\\" L"U00011542"));
    }

    void test_handles_eof_in_characters_correctly()
    {
        const wstring SHORT = L"\\" L"u154";
        for (int i = 1; i < SHORT.size(); ++i) {
            wstring s = SHORT.substr(0,i);
            TS_ASSERT_EQUALS(s, decode(s));
        }

        const wstring LONG = L"\\" L"U0001154";
        for (int i = 1; i < LONG.size(); ++i) {
            wstring s = LONG.substr(0,i);
            TS_ASSERT_EQUALS(s, decode(s));
        }
    }

    void test_handles_incomplete_characters_correctly()
    {
        const wstring SHORT = L"\\" L"u154";
        for (int i = 1; i < SHORT.size(); ++i) {
            wstring s = SHORT.substr(0,i) + L"q";
            TS_ASSERT_EQUALS(s, decode(s));
        }

        const wstring LONG = L"\\" L"U0001154";
        for (int i = 1; i < LONG.size(); ++i) {
            wstring s = LONG.substr(0,i) + L"q";
            TS_ASSERT_EQUALS(s, decode(s));
        }
    }
    
};
