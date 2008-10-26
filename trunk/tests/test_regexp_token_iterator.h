#include <string>
#include <regexp/token.h>
#include <regexp/token_iterator.h>
using namespace std;
using namespace vigilance;

class test_regexp_token_iterator : public CxxTest::TestSuite
{
    vector<regexp::token<char> > tokens;

    void with(string const& s)
    {
        tokens.clear();

        typedef regexp::token_iterator<string::const_iterator> iterator_t;

        iterator_t begin(s.begin(), s.end());
        iterator_t end;

        copy(begin, end, back_inserter(tokens));
    }

    void assert_all_tokens_are_of_type(regexp::token<char>::type_type token_type)
    {
        BOOST_FOREACH(regexp::token<char> const& t, tokens) {
            TS_ASSERT_EQUALS(token_type, t.type());
        }
    }
public:
    void test_returns_correct_values_for_literal_characters()
    {
        with("abcd");
assert_all_tokens_are_of_type(regexp::token<char>::LITERAL);

        TS_ASSERT_EQUALS('a', tokens[0].character());
        TS_ASSERT_EQUALS('b', tokens[1].character());
        TS_ASSERT_EQUALS('c', tokens[2].character());
        TS_ASSERT_EQUALS('d', tokens[3].character());
    }

    void test_returns_literals_for_backlash_esacped_special_characters()
    {
        with("\\(\\)\\[\\]\\|\\*\\\\\\/");
        
        assert_all_tokens_are_of_type(regexp::token<char>::LITERAL);

        TS_ASSERT_EQUALS('(', tokens[0].character());
        TS_ASSERT_EQUALS(')', tokens[1].character());
        TS_ASSERT_EQUALS('[', tokens[2].character());
        TS_ASSERT_EQUALS(']', tokens[3].character());
        TS_ASSERT_EQUALS('|', tokens[4].character());
        TS_ASSERT_EQUALS('*', tokens[5].character());
        TS_ASSERT_EQUALS('\\', tokens[6].character());
        TS_ASSERT_EQUALS('/', tokens[7].character());
    }

    void test_parses_grouping_operators()
    {
        with("()");
       
        TS_ASSERT_EQUALS(regexp::token<char>::OPEN_GROUP, tokens[0].type());
        TS_ASSERT_EQUALS(regexp::token<char>::CLOSE_GROUP, tokens[1].type());
    }

    void test_parses_set_operators()
    {
        with("[]");

        TS_ASSERT_EQUALS(regexp::token<char>::OPEN_SET, tokens[0].type());
        TS_ASSERT_EQUALS(regexp::token<char>::CLOSE_SET, tokens[1].type());
    }

    void test_parses_alternative_operator()
    {
        with("|");

        TS_ASSERT_EQUALS(regexp::token<char>::ALTERNATE, tokens[0].type());
    }

    void test_parses_kleene_star()
    {
        with("*");

        TS_ASSERT_EQUALS(regexp::token<char>::KLEENE_STAR, tokens[0].type());
    }

    void test_parses_lookahead_marker()
    {
        with("/");

        TS_ASSERT_EQUALS(regexp::token<char>::LOOKAHEAD, tokens[0].type());
    }

    void test_returns_backslash_literal_for_backslash_at_end_of_line()
    {
        with("\\");

        TS_ASSERT_EQUALS(regexp::token<char>::LITERAL, tokens[0].type());
        TS_ASSERT_EQUALS(L'\\', tokens[0].character());
    }

};
