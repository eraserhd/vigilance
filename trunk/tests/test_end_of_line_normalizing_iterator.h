#include <end_of_line_normalizing_iterator.h>
#include <iterator>
#include <iostream>
#include <algorithm>
using namespace std;
using namespace vigilance;

class test_end_of_line_normalizing_iterator : public CxxTest::TestSuite
{
    typedef end_of_line_normalizing_iterator<wstring::const_iterator> iterator_t;

    wstring normalize(const wstring& input)
    {
        wstring result;

        iterator_t begin(input.begin(), input.end());
        iterator_t end;

        copy(begin,end,back_inserter(result));
        return result;
    }

public:
    void test_non_end_iterator_comparisons_work()
    {
        const wstring S = L"A";
        iterator_t begin(S.begin(), S.end());
        iterator_t end;
        TS_ASSERT(begin != end);

        ++begin;
        TS_ASSERT(begin == end);
    }

    void test_passes_characters()
    {
        const wstring S = L"A";
        iterator_t begin(S.begin(), S.end());
        TS_ASSERT_EQUALS(L'A', *begin);
    }

    void test_leaves_single_newline_unchanged()
    {
        TS_ASSERT_EQUALS(L"a\nb", normalize(L"a\nb"));
    }

    void test_translates_CRLF_into_newline()
    {
        TS_ASSERT_EQUALS(L"a\nb", normalize(L"a\r\nb"));
    }

    void test_translates_lone_CR_into_newline()
    {
        TS_ASSERT_EQUALS(L"a\nb", normalize(L"a\rb"));
    }

    void test_correctly_handles_CR_at_end_of_file()
    {
        TS_ASSERT_EQUALS(L"a\n", normalize(L"a\r"));
    }
    
    void test_correctly_handles_CRLF_at_end_of_file()
    {
        TS_ASSERT_EQUALS(L"a\n", normalize(L"a\r\n"));
    }

    void test_correctly_handles_doubled_CRs()
    {
        TS_ASSERT_EQUALS(L"a\n\n", normalize(L"a\r\r"));
    }

    void test_correctly_handles_doubled_LFs()
    {
        TS_ASSERT_EQUALS(L"a\n\n", normalize(L"a\n\n"));
    }

    void test_correctly_handles_doubled_CRLFs()
    {
        TS_ASSERT_EQUALS(L"a\n\n", normalize(L"a\r\n\r\n"));
    }
};
