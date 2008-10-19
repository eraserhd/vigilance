
#include <trigraph_decoding_iterator.h>
#include <boost/foreach.hpp>
#include <boost/assign/list_inserter.hpp>
#include <boost/spirit/iterator.hpp>
using namespace std;
using namespace vigilance;
using namespace boost::assign;
using namespace boost::spirit;

class test_trigraph_decoding_iterator : public CxxTest::TestSuite
{
    typedef trigraph_decoding_iterator<wstring::const_iterator> iterator_type;
    
    vector<pair<wstring, wchar_t> > all_trigraphs()
    {
        vector<pair<wchar_t, wchar_t> > data;
        push_back(data)
            (L'=', L'#')
            (L'/', L'\\')
            (L'\'', L'^')
            (L'(', L'[')
            (L')', L']')
            (L'!', L'|')
            (L'<', L'{')
            (L'>', L'}')
            (L'-', L'~');

        vector<pair<wstring, wchar_t> > result;

        pair<wchar_t, wchar_t> e;
        BOOST_FOREACH(e, data) {
            result.push_back(make_pair(wstring(L"??") + e.first, e.second));
        }

        return result;
    }

    wstring decode(const wstring& s)
    {
        iterator_type begin(s.begin(), s.end());
        iterator_type end;
        wstring result;

        copy(begin, end, back_inserter(result));
        return result;
    }

public:
    void test_correctly_decodes_incomplete_trigraph_at_end_of_file()
    {
        TS_ASSERT_EQUALS(L"??", decode(L"??"));
        TS_ASSERT_EQUALS(L"?", decode(L"?"));
    }

    void test_correclty_decodes_question_marks_not_part_of_trigraph()
    {
        TS_ASSERT_EQUALS(L"??m", decode(L"??m"));
        TS_ASSERT_EQUALS(L"?m", decode(L"?m"));
    }

    void test_decodes_all_trigraph_sequences()
    {
        pair<wstring, wchar_t> e;
        BOOST_FOREACH(e, all_trigraphs()) {
            TS_ASSERT_EQUALS(e.second, decode(e.first)[0]);
        }
    }
};
