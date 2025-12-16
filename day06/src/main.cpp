#include <cassert>
#include <charconv>
#include <expected>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>
#include <print>
#include <fstream>

using std::println;
using std::vector;
using std::string;

constexpr int ROWS = 1000;
constexpr int COLS = 1000;

enum ActionType {
    TURNON,
    TURNOFF,
    TOGGLE,
};

struct Coord {
    int x, y;
};

struct Action {
    ActionType type;
    Coord p1;
    Coord p2;
    int x_dir;
    int y_dir;
    Action(ActionType type, Coord p1, Coord p2);
    Action(ActionType type, int x1, int y1, int x2, int y2);
    void apply(vector<vector<int>>& grid) const;
};

Action::Action(ActionType type, Coord p1, Coord p2) : type(type), p1(p1), p2(p2) {
    x_dir = (p2.x > p1.x) ? 1 : -1;
    y_dir = (p2.y > p1.y) ? 1 : -1;
}

Action::Action(ActionType type, int x1, int y1, int x2, int y2) : type(type) {
    p1 = Coord{x1, y1};
    p2 = Coord{x2, y2};
    x_dir = (p2.x > p1.x) ? 1 : -1;
    y_dir = (p2.y > p1.y) ? 1 : -1;
}

void Action::apply(vector<vector<int>>& grid) const {
    for (int x=p1.x; x!=p2.x+x_dir; x+=x_dir) {
        for (int y=p1.y; y!=p2.y+y_dir; y+=y_dir) {
            if (type==TURNOFF) grid[x][y] = 0;
            else if (type==TURNON) grid[x][y] = 1;
            else if (type==TOGGLE) grid[x][y] = (grid[x][y] - 1) * -1;
            else {
                println("ERROR: Unexpected type!");
                exit(1);
            }
        }
    }
}

std::expected<vector<string>, string> read_file(const string &filename);

constexpr string TURNOFFSTR = "turn off";
constexpr string TURNONSTR = "turn on";
constexpr string TOGGLESTR = "toggle";
constexpr string THRUDELIM = "through";

int main(void) {
    string filename = "data/input.txt";
    auto result = read_file(filename);
    if (!result) {
        println("ERROR: Filename = '{}'", filename);
        return 1;
    }
    vector<string> file_contents = result.value();

    vector<vector<int>> grid(ROWS, std::vector<int>(COLS, 0));

    vector<Action> actions;
    actions.reserve(file_contents.size());
    for (const auto& line: file_contents) {
        std::string_view sv(line);
        ActionType at;
        if (sv.starts_with(TURNOFFSTR)) {
            at = TURNOFF;
            sv.remove_prefix(TURNOFFSTR.size() + 1);
        } else if (sv.starts_with(TURNONSTR)) {
            at = TURNON;
            sv.remove_prefix(TURNONSTR.size() + 1);
        } else if (sv.starts_with(TOGGLESTR)) {
            at = TOGGLE;
            sv.remove_prefix(TOGGLESTR.size() + 1);
        } else {
            println("ERROR: Unexpected string: '{}'", line);
            return 1;
        }
        int x1, y1, x2, y2;

        std::from_chars(sv.data(), sv.data()+sv.size(), x1);

        sv.remove_prefix(sv.find_first_of(',') + 1);
        std::from_chars(sv.data(), sv.data()+sv.size(), y1);

        sv.remove_prefix(sv.find(THRUDELIM) + THRUDELIM.size() + 1);
        std::from_chars(sv.data(), sv.data()+sv.size(), x2);

        sv.remove_prefix(sv.find_first_of(',') + 1);
        std::from_chars(sv.data(), sv.data()+sv.size(), y2);

        actions.emplace_back(at, x1, y1, x2, y2);
    }

    int sum = 0;
    for (const auto& row: grid)
        sum += std::reduce(row.begin(), row.end());
    println("sum = {}", sum);

    for (const auto& action: actions)
        action.apply(grid);

    long part1 = 0;
    for (const auto& row : grid) {
        part1 += std::reduce(row.begin(), row.end());
    }
    assert(part1 == 543903);

    long part2 = 0;

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

