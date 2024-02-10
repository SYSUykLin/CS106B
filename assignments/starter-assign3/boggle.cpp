/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int points(string str) {
    /* TODO: Implement this function. */
    if (str.length() >= 4)
    {
        return str.length() - 3;
    }
    return 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */

bool isAlpha(char a)
{
    return (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z');
}

int scoreBoard_backtracking(Grid<char>& board, Lexicon& lex,
                            int x_cur, int y_cur,
                            string curStr, Grid<bool>& mark, Set<string>& words)
{
    if (x_cur < 0 || x_cur >= board.numRows() || y_cur < 0 || y_cur >= board.numCols())
    {
        return 0;
    }

    if (!isAlpha(board[x_cur][y_cur]))
    {
        return 0;
    }

    if (!mark[x_cur][y_cur])
    {
        return 0;
    }

    int scores = 0;
    string newStr = curStr + board[x_cur][y_cur];
    mark[x_cur][y_cur] = false;
    if (lex.contains(newStr) && !words.contains(newStr))
    {
        scores += points(newStr);
        words.add(newStr);
    }

    if (lex.containsPrefix(newStr))
    {
        scores += scoreBoard_backtracking(board, lex,
                                          x_cur + 1, y_cur,
                                          newStr, mark, words);
        scores += scoreBoard_backtracking(board, lex,
                                          x_cur - 1, y_cur,
                                          newStr, mark, words);
        scores += scoreBoard_backtracking(board, lex,
                                          x_cur, y_cur + 1,
                                          newStr, mark, words);
        scores += scoreBoard_backtracking(board, lex,
                                          x_cur, y_cur - 1,
                                          newStr, mark, words);
        scores += scoreBoard_backtracking(board, lex,
                                          x_cur + 1, y_cur + 1,
                                          newStr, mark, words);
        scores += scoreBoard_backtracking(board, lex,
                                          x_cur + 1, y_cur - 1,
                                          newStr, mark, words);
        scores += scoreBoard_backtracking(board, lex,
                                          x_cur - 1, y_cur - 1,
                                          newStr, mark, words);
        scores += scoreBoard_backtracking(board, lex,
                                          x_cur - 1, y_cur + 1,
                                          newStr, mark, words);
    }
    mark[x_cur][y_cur] = true;
    return scores;
}

int scoreBoard(Grid<char>& board, Lexicon& lex) {
    /* TODO: Implement this function. */
    Grid<bool> mark = {{true,true,true,true},
                        {true,true,true,true},
                        {true,true,true,true},
                        {true,true,true,true}};
    int scores = 0;
    Set<string> words;
    for (int x = 0; x < board.numRows(); ++x)
        for (int y = 0; y < board.numCols(); ++y)
        {
            string curStr;
            scores += scoreBoard_backtracking(board, lex,
                                              x, y,
                                              curStr, mark, words);
        }


    return scores;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}
