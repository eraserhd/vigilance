
#include <boost/assign/list_inserter.hpp>
#include <boost/spirit/iterator/position_iterator.hpp>
#include <token.h>
using namespace std;
using namespace cpp;
using namespace boost::assign;
using namespace boost::spirit;

class test_token : public CxxTest::TestSuite
{
public:
    void test_gets_spelling()
    {
        vector<pair<wchar_t, file_position> > data;
        push_back(data)
            (L'A', file_position("foo.cpp",1,1))
            (L'b', file_position("foo.cpp",1,2))
            (L'c', file_position("foo.cpp",1,3));

        token t(data.begin(), data.end());
        TS_ASSERT_EQUALS(L"Abc", t.spelling());
    }

};
