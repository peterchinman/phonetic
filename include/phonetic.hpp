#pragma once

#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


class Phonetic {
private:
    // map of words with their CMU ARPABET_pronunciations, separated by spaces
    std::unordered_map<std::string, std::vector<std::string>> m_dictionary {};

    // All vowels used in CMU
    const std::unordered_set<std::string> CMU_VOWELS = {
        "AA", "AE", "AH", "AO", "EH",
        "ER", "IH", "IY", "UH", "UW", // 10 monopthongs
        "AW", "AY", "EY", "OW", "OY" // 5 dipthongs
    };

// TODO mark functions as const that don't change state

public:

    // Runs import_dictionary();
    Phonetic();
    
    // assumes filepath "../data/CMUdict/cmudict-0.7b"
    bool import_dictionary();

    /**
     * Get array of possible pronunciations from CMUdict.
     * 
     * Throws a std::exception if word not found.
     * 
     * @param (string) word: English word to look up.
     * @return Vector of strings, of the possible pronunciations recorded in the dictionary, each of which are a string of ARAPBET phones separated by spaces.
    */
    std::vector<std::string> word_to_phones(std::string word);

    /**
     * Get an array of possible pronuncitation of each word from a text.
     * 
     * If a word is not found, vector will contain the word that was searched for, and bool is marked false.
     * 
     * @param text (string): text to look up
     * @return An array of pairs, each pair has 1. array of possible pronunciations, 2. a bool flag indicating if word was found.
    */
    std::vector<std::pair<std::vector<std::string>, bool>> text_to_phones(const std::string & text);

     /**
     * Takes a string of space-separated CMUdict phones, returns a string of the stresses.
     * 
     * 0 = no stress, 1 = primary stress, 2 = secondary stress.
     * 
     * @param (string) phones: space-separated CMUdict phones
     * @return (string): String of stresses
    */
    std::string phone_to_stress(const std::string& phones);

    /**
     * Takes an English word and returns a vector of possible stresses.
     * 
     * TODO: should do exception handling here as well. 
     * 
     * @param (string) word: English word
     * @return (vector<string>): vector of strings of stresses
    */
    std::vector<std::string> word_to_stresses(const std::string& word);

    /**
     * Take a string of space-separated CMUdict phones, returns number of syllables.
     * 
     * @param (string) phones: space-separated CMUdict phones
     * @return (int): number of syllables
    */
    int phone_to_syllable_count(const std::string& phones);

    /**
     * Takes an English word, returns a vector of possible stresses.
     * 
     * (Most words have a single syllable count even across multiple pronunciations, but this catches the outliers, e.g. "fire".)
     * 
     * @param (string) word: English word
     * @return (vector<int>): possible numbers of syllables
    */
    std::vector<int> word_to_syllable_counts(const std::string& word);


    /**
     * Get the rhyming part from a string of space-separated phones.
     * 
     * "Rhyming part" consists of: the accented vowel closest, to the end of the word.
     * 
     * @param phones (string): string of space-separated phones
     * @return (string): a string of space-separated phones
    */
    std::string get_rhyming_part(const std::string& phones);

    // TODO implement search
        // search by phones
        // search by stress
};


