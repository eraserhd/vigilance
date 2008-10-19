
#include <string>
#include <logical_line_iterator.h>
using namespace std;
using namespace vigilance;

class test_logical_line_iterator : public CxxTest::TestSuite
{
    string decode(const string& s)
    {
        typedef logical_line_iterator<string::const_iterator> iterator_t;

        iterator_t begin(s.begin(), s.end());
        iterator_t end;

        string result;
        copy(begin,end,back_inserter(result));
        return result;
    }
public:
    void test_strips_backslash_newline()
    {
        TS_ASSERT_EQUALS("ab", decode("a\\\nb"));
    }

    void test_does_not_strip_backslash_at_end_of_file()
    {
        TS_ASSERT_EQUALS("ab\\", decode("ab\\"));
    }

    void test_does_not_strip_backslash_without_newline()
    {
        TS_ASSERT_EQUALS("a\\b", decode("a\\b"));
    }

    void test_strips_backslash_newline_at_beginning_of_file()
    {
        TS_ASSERT_EQUALS("ab", decode("\\\nab"));
    }
};
