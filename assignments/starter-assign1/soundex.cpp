/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
#include <map>
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */

void initMaps(map<char, char> &mapString){

    mapString.clear();

    string zeros = "AEIOUHWY";
    string ones = "BFPV";
    string twos = "CGJKQSXZ";
    string three = "DT";
    string fours = "L";
    string fines = "MN";
    string sixs = "R";
    for (char c : zeros){
        mapString[std::toupper(c)] = '0';
        mapString[std::tolower(c)] = '0';
    }
    for (char c : ones){
        mapString[std::toupper(c)] = '1';
        mapString[std::tolower(c)] = '1';
    }
    for (char c : twos){
        mapString[std::toupper(c)] = '2';
        mapString[std::tolower(c)] = '2';
    }
    for (char c : three){
        mapString[std::toupper(c)] = '3';
        mapString[std::tolower(c)] = '3';
    }
    for (char c : fours){
        mapString[std::toupper(c)] = '4';
        mapString[std::tolower(c)] = '4';
    }
    for (char c : fines){
        mapString[std::toupper(c)] = '5';
        mapString[std::tolower(c)] = '5';
    }
    for (char c : sixs){
        mapString[std::toupper(c)] = '6';
        mapString[std::tolower(c)] = '6';
    }

}

string encodeString(string s){

    string result = "";
    map<char, char> mapString;
    initMaps(mapString);
    for (char c : s)
    {
        result.push_back(mapString[c]);
    }
    return result;
}

/**
 * @brief CoalesceAdjacent
 * @param s 需要处理的字符串
 * 删除掉相邻相同的字符
 * @return
 */
string CoalesceAdjacent(string s)
{
    int N = s.size();
    int cur = 0;
    while(cur < N)
    {
        char curChar = s[cur];
        int i = cur + 1;
        while(i < N)
        {
            if(curChar == s[i])
            {
                s.erase(cur, 1);
                N--;
            }
            else
                break;
        }
        cur++;
    }
    return s;
}

string replaceFirstChar(string encodeNum, string originString)
{
    char firstNum = encodeNum[0];
    map<char, char> mapString;
    initMaps(mapString);
    for (char c : originString)
    {
        if (mapString[c] == firstNum)
        {
            encodeNum[0] = std::toupper(c);
            return encodeNum;
         }
    }
    return "";
}

string removeZero(string s)
{
    string result = "";
    for (char c : s){
         if(c != '0'){
            result.push_back(c);
         }
    }
    return result;
}

string paddingFour(string s)
{
    int N = s.size();
    if (N == 4)
    {
         return s;
    }
    else if (N > 4)
    {
         return s.substr(0, 4);
    }
    else{
         for (int i = N; i < 4;i++)
         {
            s.push_back('0');
         }
         return s;
    }
}

string soundex(string s) {
    /* TODO: Fill in this function. */
    string originString = s;
    string result = removeNonLetters(s);
    result = encodeString(result);
    result = CoalesceAdjacent(result);
    result = replaceFirstChar(result, originString);
    result = removeZero(result);
    result = paddingFour(result);
    return result;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    /* TODO: Fill in the remainder of this function. */
    string s;
    cout << "Enter a surname (RETURN to quit): ";
    cin >> s;
    cout << "Soundex code is " << soundex(s) << endl;

    Vector<string> matchString;
    for (string dataName : databaseNames)
    {
        if(soundex(dataName) == soundex(s))
        {
            matchString.add(dataName);
        }
    }
    matchString.sort();
    cout << "Matches from database: {";
    for (int i = 0; i < matchString.size(); ++i)
    {
        if (i != matchString.size() - 1)
            cout << matchString[i] << ", ";
        else
            cout << matchString[i];
    }
    cout << "}";

}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here
PROVIDED_TEST("remove testing"){
    string s = "";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
    s = "12,3,4";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
}
PROVIDED_TEST("CoalesceAdjacent test") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(CoalesceAdjacent("2222"), "2");
    EXPECT_EQUAL(CoalesceAdjacent("20025555533"), "20253");
}





