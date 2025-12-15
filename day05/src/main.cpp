#include <cassert>
#include <expected>
#include <string>
#include <vector>
#include <print>
#include <fstream>

using std::println;
using std::vector;
using std::string;

bool string_is_nice_part1(const string& str);
std::expected<vector<string>, string> read_file(const string &filename);
int count_vowels_in_str(const string& str);

int main(void) {
    assert(count_vowels_in_str("aaa") == 3);
    assert(string_is_nice_part1("ugknbfddgicrmopn"));
    assert(string_is_nice_part1("aaa"));
    assert(!string_is_nice_part1("jchzalrnumimnmhp"));
    assert(!string_is_nice_part1("haegwjzuvuyypxyu"));
    assert(!string_is_nice_part1("dvszwmarrgswjxmb"));
    // string filename = "data/test.txt";
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

    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}

int count_vowels_in_str(const string& str) {
    string vowels = "aeiou";
    int result = 0;
    for (char c: str) {
        if (vowels.find(c) != vowels.npos) result += 1;
    }
    return result;
}

bool str_has_two_in_row(string str) {
    for (size_t i=0; i<str.size()-1; i++) {
        if (str[i] == str[i+1]) return true;
    }
    return false;
}

bool string_is_nice_part1(const string& str) {
    // ab, cd, pq, or xy,
    if (str.find("ab") != str.npos) return false;
    if (str.find("cd") != str.npos) return false;
    if (str.find("pq") != str.npos) return false;
    if (str.find("xy") != str.npos) return false;

    int vowel_cnt = count_vowels_in_str(str);
    bool two_in_row = str_has_two_in_row(str);

    if (two_in_row && (vowel_cnt > 2))
        return true;

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

