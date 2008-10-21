
#include <parsing_algorithms.h>
using namespace std;
using namespace vigilance;

class test_parsing_algorithms : public CxxTest::TestSuite
{
public:
    void test_is_identifier_character_correctly_identifies_csym_characters()
    {
        const wchar_t DATA[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
        for (const wchar_t *p = DATA; *p; ++p) {
            TS_ASSERT(is_identifier_character(*p));
        }
    }

    void test_is_identifier_first_character_returns_the_same_as_is_identifier_character_except_for_digits()
    {
        for (wchar_t c = 0; c <= (wchar_t)65535; ++c) {
            if (iswdigit(c)) {
                TS_ASSERT(!is_identifier_first_character(c));
                continue;
            }

            TS_ASSERT_EQUALS(is_identifier_character(c), is_identifier_first_character(c));
        }
    }

    void test_returns_correct_values_for_some_spot_checks()
    {
        TS_ASSERT(is_identifier_character((wchar_t)0x0ae0)); // single character range
        TS_ASSERT(!is_identifier_character((wchar_t)0x30a0)); // before range
        TS_ASSERT(is_identifier_character((wchar_t)0x30a1)); // start of range
        TS_ASSERT(is_identifier_character((wchar_t)0x30b0)); // middle of range
        TS_ASSERT(is_identifier_character((wchar_t)0x30fe)); // end of range
        TS_ASSERT(!is_identifier_character((wchar_t)0x30ff)); // after range
    }
};
