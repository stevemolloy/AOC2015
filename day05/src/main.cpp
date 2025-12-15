#include <cassert>
#include <expected>
#include <string>
#include <vector>
#include <print>
#include <fstream>

using std::println;
using std::vector;
using std::string;

struct Triple {
    char first, second, third;
    int loc;
    Triple(char first, char second, char third, int loc);
    Triple(string str, int loc);
    bool nice(void) const;
};

Triple::Triple(char first, char second, char third, int loc) :
    first(first), second(second), third(third), loc(loc) {}

Triple::Triple(string str, int loc) : loc(loc) {
    assert(str.size() > 2);
    first = str[0];
    second = str[1];
    third = str[2];
}

bool Triple::nice(void) const {
    return first == third;
}

struct Pair {
    char first, second;
    int loc;
    Pair(char first, char second, int loc);
    Pair(string str, int loc);
    bool operator==(const Pair& other) const;
};

Pair::Pair(char first, char second, int loc) : first(first), second(second), loc(loc) {}
Pair::Pair(string str, int loc) : loc(loc) {
    assert(str.size() > 1);
    first = str[0];
    second = str[1];
}

bool Pair::operator==(const Pair& other) const {
    return (first == other.first) && (second == other.second);
}

bool string_is_nice_part1(const string& str);
bool string_is_nice_part2(const string& str);
std::expected<vector<string>, string> read_file(const string &filename);
int count_vowels_in_str(const string& str);
bool pair_list_is_nice(vector<Pair> pairs);
bool triple_list_is_nice(vector<Triple> triples);
vector<Pair> get_all_pairs(string str);
vector<Triple> get_all_triples(string str);

int main(void) {
    {  // Part 1 tests
        assert(count_vowels_in_str("aaa") == 3);
        assert(string_is_nice_part1("ugknbfddgicrmopn"));
        assert(string_is_nice_part1("aaa"));
        assert(!string_is_nice_part1("jchzalrnumimnmhp"));
        assert(!string_is_nice_part1("haegwjzuvuyypxyu"));
        assert(!string_is_nice_part1("dvszwmarrgswjxmb"));
    }

    { // Part 2 tests
        string test_str;
        assert(string_is_nice_part2("qjhvhtzxzqqjkmpb"));
        assert(string_is_nice_part2("xxyxx"));
        assert(!string_is_nice_part2("uurcxstgmygtbstg"));
        assert(!string_is_nice_part2("ieodomkazucvgmuy"));
    }

    string filename = "data/input.txt";
    auto result = read_file(filename);
    if (!result) {
        println("ERROR: Filename = '{}'", filename);
        return 1;
    }
    vector<string> file_contents = result.value();

    long part1 = 0;
    for (const auto& str: file_contents)
        if (string_is_nice_part1(str)) part1 += 1;
    assert(part1 == 258);

    long part2 = 0;
    for (const auto& str: file_contents) {
        if (string_is_nice_part2(str)) part2 += 1;
    }

    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}

bool pair_list_is_nice(vector<Pair> pairs) {
    for (size_t i=0; i<pairs.size()-1; i++) {
        for (size_t j=i+1; j<pairs.size(); j++) {
            Pair p1 = pairs[i];
            Pair p2 = pairs[j];
            if ((p1 == p2) && abs(p2.loc - p1.loc) > 1) return true;
        }
    }
    return false;
}

bool triple_list_is_nice(vector<Triple> triples) {
    for (const auto& t: triples)
        if (t.nice()) return true;
    return false;
}

vector<Pair> get_all_pairs(string str) {
    int num_pairs = str.size() - 1;
    vector<Pair> result;
    result.reserve(num_pairs);

    for (int i=0; i<num_pairs; i++)
        result.emplace_back(str[i], str[i+1], i);

    return result;
}

vector<Triple> get_all_triples(string str) {
    int num_triples = str.size() - 2;
    vector<Triple> result;
    result.reserve(num_triples);

    for (int i=0; i<num_triples; i++)
        result.emplace_back(str[i], str[i+1], str[i+2], i);

    return result;
}

int count_vowels_in_str(const string& str) {
    string vowels = "aeiou";
    int result = 0;
    for (char c: str) 
        if (vowels.find(c) != vowels.npos) result += 1;
    return result;
}

bool str_has_two_in_row(string str) {
    for (size_t i=0; i<str.size()-1; i++)
        if (str[i] == str[i+1]) return true;
    return false;
}

bool string_is_nice_part2(const string& str) {
    vector<Pair> pairs = get_all_pairs(str);
    vector<Triple> triples = get_all_triples(str);
    return pair_list_is_nice(pairs) && triple_list_is_nice(triples);
}

bool string_is_nice_part1(const string& str) {
    // ab, cd, pq, or xy,
    if (str.find("ab") != str.npos) return false;
    if (str.find("cd") != str.npos) return false;
    if (str.find("pq") != str.npos) return false;
    if (str.find("xy") != str.npos) return false;

    int vowel_cnt = count_vowels_in_str(str);
    bool two_in_row = str_has_two_in_row(str);

    if (two_in_row && (vowel_cnt > 2)) return true;

    return false;
}

std::expected<vector<string>, string> read_file(const string &filename) {
    std::ifstream file_stream(filename);
    if (!file_stream.is_open())
        return std::unexpected("Failed to open file: " + filename);

    vector<string> file_contents;
    string line;
    while (std::getline(file_stream, line)) file_contents.push_back(std::move(line));
    return file_contents;
}

