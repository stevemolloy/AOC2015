#include <cassert>
#include <expected>
#include <map>
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

    std::map<long, std::map<long, long>> part1_addresses;
    long x = 0, y = 0;
    part1_addresses[x][y] = 1;
    long part1 = 1;
    for (const auto& line: file_contents) {
        for (const auto& c: line) {
            if (c == '<') x -= 1;
            else if (c == '>') x += 1;
            else if (c == 'v') y -= 1;
            else if (c == '^') y += 1;
            else {
                println("ERROR: Unexpected char: '{}'", c);
                return 1;
            }
            if (part1_addresses.contains(x) && part1_addresses[x].contains(y)) part1_addresses[x][y] += 1;
            else {
                part1_addresses[x][y] = 1;
                part1 += 1;
            }
        }
    }
    if (filename == "data/input.txt") assert(part1 == 2572);

    long part2 = 1;
    long sx = 0, sy = 0, rx = 0, ry = 0;
    std::map<long, std::map<long, long>> part2_addresses;
    part2_addresses[sx][sy] = 2;
    bool santa = true;
    for (const auto& line: file_contents) {
        for (const auto& c: line) {
            if (c == '<')      {
                if (santa) sx -= 1;
                else       rx -= 1;
            } else if (c == '>') {
                if (santa) sx += 1;
                else       rx += 1;
            } else if (c == 'v') {
                if (santa) sy -= 1;
                else       ry -= 1;
            } else if (c == '^') {
                if (santa) sy += 1;
                else       ry += 1;
            } else {
                println("ERROR: Unexpected char: '{}'", c);
                return 1;
            }

            if (santa) {
                if (part2_addresses.contains(sx) && part2_addresses[sx].contains(sy)) part2_addresses[sx][sy] += 1;
                else {
                    part2_addresses[sx][sy] = 1;
                    part2 += 1;
                }
            } else {
                if (part2_addresses.contains(rx) && part2_addresses[rx].contains(ry)) part2_addresses[rx][ry] += 1;
                else {
                    part2_addresses[rx][ry] = 1;
                    part2 += 1;
                }
            }
            santa = !santa;
        }
    }
    if (filename == "data/input.txt") assert(part2 == 2631);

    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

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

