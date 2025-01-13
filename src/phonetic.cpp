#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

#include "phonetic.h"
#include "convenience.h"

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

Phonetic::Phonetic() {
    import_dictionary();
}

bool Phonetic::import_dictionary() {

    const std::string file_path{"../data/CMUdict/cmudict-0.7b"};

    #ifdef __EMSCRIPTEN__   
    file_path = "/data/cmudict-0.7b";
    #endif
    
    
    std::ifstream cmudict{file_path};
    if (!cmudict.is_open()) {
        std::cerr << "Failed to open the dictionary." << '\n';
        return false;
    }
    std::string line;
    while (std::getline(cmudict, line)) {
        // ignore leading ;
        if(line.empty() || line[0] == ';') {
            continue;
        }

        std::istringstream iss(line);

        // extract word up to white space   
        std::string word;
        iss >> word;

        // strip variation "(n)", used in CMU DICT for multiple entries of same word
        // instead each pronunciation is added to the vector m_dictionary[word]
        if (word.back() == ')'){
            word.pop_back();
            word.pop_back();
            word.pop_back();
        }
       
        // pronunciation is ARPABET symbols, separated by spaces
        // vowels end with a number indicating stress, 0 no stress, 1 primary stress, 2 secondary stress
        std::string pronunciation;
        std::getline(iss, pronunciation);

        // trim white space
        ltrim(pronunciation);
        rtrim(pronunciation);

        m_dictionary[word].push_back(pronunciation);
    }

    cmudict.close();
    return true;
}

std::vector<std::string> Phonetic::word_to_phones(std::string word) {
    // capitalize all queries
    std::transform(word.begin(), word.end(), word.begin(), ::toupper);
    auto it = m_dictionary.find(word);
    if (it != m_dictionary.end()) {
        return it->second;
    }
    else {
        throw std::runtime_error(word + " not found in dictionary.");
    }
}

std::vector<std::pair<std::vector<std::string>, bool>> Phonetic::text_to_phones(const std::string & text) {

    std::vector<std::pair<std::vector<std::string>, bool>> results{};
    std::vector<std::string> words {strip_punctuation(text)};

    for (const auto & w : words) {
        try {
            std::vector<std::string> phones{word_to_phones(w)};
            results.emplace_back(phones, true);
        } catch (const std::exception &) {
            std::vector<std::string> word_searched_for{};
            word_searched_for.emplace_back(w);
            results.emplace_back(word_searched_for, false);
        }
    }

    return results;
}


std::string Phonetic::phone_to_stress(const std::string& phones) {
    std::string stresses{};
    for (const auto & c : phones){
        if (c == '0' || c == '1' || c == '2') {
            stresses.push_back(c);
        }
    }
    return stresses;
}

std::vector<std::string> Phonetic::word_to_stresses(const std::string& word) {
    std::vector<std::string> stresses{};

    std::vector<std::string> phones{word_to_phones(word)};
    for (const auto & p : phones) {
        stresses.emplace_back(phone_to_stress(p));
    }
    return stresses;
}

int Phonetic::phone_to_syllable_count(const std::string& phones) {
    return static_cast<int>(phone_to_stress(phones).length());
}

std::vector<int> Phonetic::word_to_syllable_counts(const std::string& word) {
    std::vector<int> syllables;
    std::vector<std::string> phones{word_to_phones(word)};
    for(const auto & p : phones) {
        syllables.emplace_back(phone_to_syllable_count(p));
    }
    return syllables;
}

std::string Phonetic::get_rhyming_part(const std::string& phones) {
    std::string result{};
    // if we were using C++23 we could use std::ranges::find_last_if, but we're not

    // get a reverse iterator pointing at the number in the last stressed vowel
    auto r_it {std::find_if(phones.rbegin(), phones.rend(), [](char c) {
        return std::isdigit(c) && c != '0';
    })};
    // if we found it
    if (r_it != phones.rend()) {
        // move it up to the first char of our vowel
        r_it += 2;
        // turn it around
        // base moves us one to the right, so we got to step it back
        auto f_it = r_it.base() - 1;
        std::copy(f_it, phones.end(), std::back_inserter(result));
        return result;
    }
    // but if the word has no stresses, we'll return from just the last vowel onward
    auto no_stress_r_it {std::find_if(phones.rbegin(), phones.rend(), [](char c) {
        return std::isdigit(c);
    })};

    if (no_stress_r_it != phones.rend()) {
        // move it up to the first char of our vowel
        no_stress_r_it += 2;
        // turn it around
        // base moves us one to the right, so we got to step it back
        auto f_it = no_stress_r_it.base() - 1;
        std::copy(f_it, phones.end(), std::back_inserter(result));
        return result;
    }

    // else, there are no vowels at all, so we return an empty string
    return result;
}

