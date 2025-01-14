#include <catch2/catch_test_macros.hpp>
#include "convenience.hpp"
#include "phonetic.hpp"
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

struct Fixture {
    mutable Phonetic dict;

    // bool import_success{};

    // Fixture() {
        // bool import_success = dict.import_dictionary();
    // }
};


TEST_CASE_PERSISTENT_FIXTURE(Fixture, "Dictionary tests") {
    SECTION ("dictionary import success") {
    }
    SECTION("word_to_phones") {
        std::vector<std::string> word_lower = dict.word_to_phones("associate");
        REQUIRE(word_lower.size() == 4);
        REQUIRE(word_lower[0] == "AH0 S OW1 S IY0 AH0 T");
        // upper case
        std::vector<std::string> word_upper = dict.word_to_phones("ASSOCIATE");
        REQUIRE(word_upper.size() == 4);
        REQUIRE(word_upper[0] == "AH0 S OW1 S IY0 AH0 T");
    }

    SECTION("word_to_phones exception") {
        std::vector<std::string> bad_word{};
        REQUIRE_THROWS(bad_word = dict.word_to_phones("sdfasdg"));
    } 

    SECTION("phone_to_stress") {
        std::string stresses = dict.phone_to_stress("M AA1 D ER0 N AY2 Z D");
        REQUIRE(stresses == "102");
    }

    SECTION("word_to_stresses") {
        std::vector<std::string> stresses{dict.word_to_stresses("atoll")};
        REQUIRE(stresses[0] == "12");
        REQUIRE(stresses[2] == "01");
    }

    SECTION("phone_to_syllable_count") {
        REQUIRE(dict.phone_to_syllable_count("F AY1 ER0") == 2);
    }

    SECTION("word_to_syllable_counts") {
        std::vector<int> fire = dict.word_to_syllable_counts("fire");
        REQUIRE(fire.size() == 2);
        REQUIRE(fire[0] == 2);
        REQUIRE(fire[1] == 1);
    }

    SECTION("text_to_phones simple") {
        std::string text{"smelly dog"};
        std::vector<std::pair<std::vector<std::string>, bool>> phones{dict.text_to_phones(text)};
        REQUIRE(phones.size() == 2);
        REQUIRE(phones[0].first[0] == "S M EH1 L IY0");
        REQUIRE(phones[1].first[0] == "D AO1 G");
    }
  
    SECTION("text_to_phones complex punct") {
        std::string text{"Smelly dog? Drip-dry--good dog."};
        std::vector<std::pair<std::vector<std::string>, bool>> phones{dict.text_to_phones(text)};
        REQUIRE(phones.size() == 5);
        REQUIRE(phones[0].first[0] == "S M EH1 L IY0");
        REQUIRE(phones[2].first[0] == "D R IH1 P D R AY1");
    }

    SECTION("text_to_phones exceptions") {
        std::string text{"Smelly dog? asdfaga"};
        std::vector<std::pair<std::vector<std::string>, bool>> phones{dict.text_to_phones(text)};
        REQUIRE(phones.size() == 3);
        REQUIRE(phones[2].second == false);
        REQUIRE(phones[2].first[0] == "asdfaga");
    }

    SECTION("get_rhyming_part") {
        std::string phones = "M AO1 R F";
        REQUIRE(dict.get_rhyming_part(phones) == "AO1 R F");
        phones = "P UH1 L IY0";
        REQUIRE(dict.get_rhyming_part(phones) == "UH1 L IY0");
        phones = "P EH1 R AH0 L AH0 S";
        REQUIRE(dict.get_rhyming_part(phones) == "EH1 R AH0 L AH0 S");
        phones = "P EH1 R AH0 S K OW2 P";
        REQUIRE(dict.get_rhyming_part(phones) == "OW2 P");
        phones = "DH AH0";
        REQUIRE(dict.get_rhyming_part(phones) == "AH0");
        phones = "DH S K";
        REQUIRE(dict.get_rhyming_part(phones) == "");
    }
    
}

