// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include <string>
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
string cleanToken(string s)
{
    int start = 0;
    int end = s.length()-1;
    bool return_flag = true;
    while(start < end && return_flag)
    {
        return_flag = false;
        if (ispunct(static_cast<unsigned char>(s[start])))
        {
            ++ start;
            return_flag = true;
        }
        if (ispunct(static_cast<unsigned char>(s[end])))
        {
            -- end;
            return_flag = true;
        }
    }
    s = s.substr(start, end - start + 1);

    bool contain_alpha = false;
    for(int i = 0; i < s.length(); i++)
    {
        char c = s[i];
        if(isalpha(static_cast<unsigned char>(c)))
        {
            contain_alpha = true;
            s[i] = toLowerCase(s[i]);
        }
    }
    if (!contain_alpha)
    {
        return string("");
    }

    return s;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> split_set = stringSplit(text, " ");
    for (string s : split_set)
    {
        string c_s = cleanToken(s);
        if (c_s != "")
        {
            tokens.add(c_s);
        }
    }
    return tokens;
}

map<string, string> readWebFile(string filename)
{
    ifstream in;
    if (!openFile(in, filename))
    {
        error("Cannot open file named " + filename);
    }
    map<string, string> webText;
    Vector<string> lines;
    readEntireFile(in, lines);

    for (int i = 0; i < lines.size(); i+=2)
    {
        webText[lines[i]] = lines[i+1];
    }
    return webText;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    int count = 0;
    map<string, string> webText = readWebFile(dbfile);
    for (auto it = webText.begin(); it != webText.end(); ++it)
    {
        ++count;
        string body_text = it->second;
        Set<string> body_text_set = gatherTokens(body_text);
        for (string keyword : body_text_set)
        {
            if (index.containsKey(keyword))
            {
                index[keyword].add(it->first);
            }
            else
            {
                Set<string> url_set;
                url_set.add(it->first);
                index[keyword] = url_set;
            }
        }
    }
    return count;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    Vector<string> query_set = stringSplit(query, " ");
    for (string c : query_set)
    {
        string s = cleanToken(c);
        if (index.containsKey(s))
        {
            if (isalpha(static_cast<unsigned char>(c[0])))
            {
                result.unionWith(index[s]);
            }
            else if (c[0] == '+')
            {
                result.intersect(index[s]);
            }
            else if (c[0] == '-')
            {
                result.difference(index[s]);
            }
        }
    }
    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    std::cout << "Stand by while building index..." << std::endl;
    Map<string, Set<string>> index;
    int pages_count = buildIndex(dbfile, index);
    string input;
    while(true)
    {
        std::cout << "Indexed " << pages_count << " pages containing " << index.size() << " unique terms" << std::endl;
        std::cout << "Enter query sentence (RETURN/ENTER to quit): ";
        std::cin.ignore();
        getline(std::cin, input);
        if (toLowerCase(input) == "return" || input == "/n")
        {
            std::cout << "All done!" << std::endl;
            return;
        }
        Set<string> search_results = findQueryMatches(index, input);
        std::cout << "Found " << search_results.size() << " matching pages " << std::endl;
        std::cout << search_results << std::endl;
    }


}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("Time text") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
    TIME_OPERATION(nPages, buildIndex("res/tiny.txt", index));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}

// TODO: add your test cases here
STUDENT_TEST("search engine")
{
    searchEngine("res/website.txt");
}
