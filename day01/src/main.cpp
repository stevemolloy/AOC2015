#include <cassert>
#include <expected>
#include <string>
#include <vector>
#include <print>
#include <fstream>

using std::println;
using std::vector;
using std::string;

std::expected<vector<string>, string> read_file(const string &filename);

int main(void) {
    // string filename = "data/test.txt";
    string filename = "data/input.txt";
    auto result = read_file(filename);
    if (!result) {
        println("ERROR: Filename = '{}'", filename);
        return 1;
    }
    vector<string> file_contents = result.value();

    long part1 = 0;
    for (const auto& line: file_contents) {
        for (const auto& c: line) {
            if (c=='(') part1 += 1;
            else if (c==')') part1 -= 1;
            else {
                println("ERROR: Unexpected char: {}", c);
                return 1;
            }
        }
    }
    if (filename == "data/input.txt") assert(part1 = 138);

    println("Part 1: {}", part1);

    return 0;
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

