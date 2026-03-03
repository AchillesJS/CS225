/**
 * @file cartalk_puzzle.cpp
 * Holds the function which solves a CarTalk puzzler.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include <fstream>
#include <vector>
#include <string>

#include "cartalk_puzzle.h"

using namespace std;

/**
 * Solves the CarTalk puzzler described here:
 * http://www.cartalk.com/content/wordplay-anyone.
 * @return A vector of (string, string, string) tuples
 * Returns an empty vector if no solutions are found.
 * @param d The PronounceDict to be used to solve the puzzle.
 * @param word_list_fname The filename of the word list to be used.
 */
vector<std::tuple<std::string, std::string, std::string>> cartalk_puzzle(PronounceDict d,
                                    const string& word_list_fname)
{
    vector<std::tuple<std::string, std::string, std::string>> ret;

    /* Your code goes here! */
    ifstream word_file(word_list_fname);
    vector<string> words;
    string word;
    if (word_file.is_open()) {
        while (word_file >> word) {
            words.push_back(word);
        }
    }
    for (const auto& w1 : words) {
        if (w1.size() < 3) {
            continue;
        }
        // remove first letter
        string w2 = w1.substr(1);
        // remove second letter
        string w3 = w1;
        w3.erase(1, 1);
        if (d.homophones(w1, w2) && d.homophones(w1, w3)) {
            ret.push_back(std::make_tuple(w1, w2, w3));
        }
    }

    return ret;
}
