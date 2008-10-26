
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <regexp/parser.h>
using namespace std;
using namespace vigilance;

class test_regexp_parser : public CxxTest::TestSuite
{
    string parse_to_repr(const string& s)
    {
        return regexp::parse(s.begin(), s.end())->repr();
    }
public:
    void test_can_parse_empty_regexp()
    {
        TS_ASSERT_EQUALS("$", parse_to_repr(""));
    }

    void test_can_parse_series_of_cat_characters()
    {
        TS_ASSERT_EQUALS("(cat (cat (cat #a #b) #c) $)", parse_to_repr("abc"));
    }

    void test_can_parse_kleene()
    {
        TS_ASSERT_EQUALS("(cat (kleene #a) $)", parse_to_repr("a*"));
    }

    void test_can_alternate()
    {
        TS_ASSERT_EQUALS("(cat (or #a #b) $)", parse_to_repr("a|b"));
    }

    void test_can_alternate_with_initial_empty_expression()
    {
        TS_ASSERT_EQUALS("(cat (or (empty) #a) $)", parse_to_repr("|a"));
    }

    void test_can_alternate_with_empty_second_expression()
    {
        TS_ASSERT_EQUALS("(cat (or #a (empty)) $)", parse_to_repr("a|"));
    }

    void test_can_alternate_with_empty_expression_in_middle()
    {
        TS_ASSERT_EQUALS("(cat (or (or #a (empty)) #b) $)", parse_to_repr("a||b"));
    }

    // FIXME: check for all the parse errors

};

