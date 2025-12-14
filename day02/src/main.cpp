#include <algorithm>
#include <cassert>
#include <expected>
#include <numeric>
#include <sstream>
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
    long part2 = 0;

    for (const auto& line: file_contents) {
        char delim;
        long x, y, z;
        std::stringstream ss(line);
        ss >> x >> delim >> y >> delim >> z;

        vector<long> faces{x*y, x*z, y*z};
        vector<long> face_perims{2*(x+y), 2*(x+z), 2*(y+z)};
        long volume = x*y*z;

        long min_face = std::ranges::min(faces);
        long min_perim = std::ranges::min(face_perims);
        long face_sum = std::reduce(faces.begin(), faces.end());

        part1 += 2*face_sum + min_face;
        part2 += min_perim + volume;
    }
    if (filename == "data/input.txt") {
        assert(part1 == 1586300);
        assert(part2 == 3737498);
    }

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

