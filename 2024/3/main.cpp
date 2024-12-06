#include <array>
#include <cstdint>
#include <file.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

enum class Instruction_type {
    Mult,
    Do,
    Dont,
};

struct Instruction {
    Instruction_type type;
    uint32_t left;
    uint32_t right;
};

void part1(std::vector<Instruction> data);
void part2(std::vector<Instruction> data);

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " <filename>\n";
        return 1;
    }
    const char *filepath = argv[1];
    std::ifstream file_stream(filepath);
    if (!file_stream.good()) {
        std::cout << "file does not exit: " << filepath << "\n";
        return 1;
    }
    std::ostringstream sstr;
    sstr << file_stream.rdbuf();
    std::string file = sstr.str();

    std::regex prog("(mul)\\(([0-9]+),([0-9]+)\\)|((do)\\(\\))|((don't)\\(\\))",
                    std::regex_constants::ECMAScript);
    std::cout << file << "\n";

    std::sregex_iterator begin =
        std::sregex_iterator(file.begin(), file.end(), prog);
    std::sregex_iterator end = std::sregex_iterator();

    std::cout << "found " << std::distance(begin, end) << "\n";

    std::vector<Instruction> rows;

    for (std::sregex_iterator i = begin; i != end; i++) {
        std::smatch match = *i;
        std::string multype = match.str(1);
        std::string dotype = match.str(5);
        std::string donttype = match.str(7);
        Instruction row;
        if (multype == "mul") {
            uint32_t left = std::stoi(match.str(2));
            uint32_t right = std::stoi(match.str(3));
            row = {Instruction_type::Mult, left, right};
        } else if (dotype == "do") {
            row = {Instruction_type::Do};
        } else if (donttype == "don't") {
            row = {Instruction_type::Dont};
        }
        std::cout << (int)row.type << "\n";
        rows.push_back(row);
    }

    part1(rows);
    part2(rows);

    return 0;
}

void part1(std::vector<Instruction> data) {
    uint32_t number = 0;
    for (Instruction row : data) {
        if (row.type != Instruction_type::Mult)
            continue;
        uint32_t sum = row.left * row.right;
        number += sum;
        std::cout << row.left << "*" << row.right << "=" << sum << "\n";
    }
    std::cout << "part1 number: " << number << "\n";
}

void part2(std::vector<Instruction> data) {
    uint32_t number = 0;
    bool add = true;
    for (Instruction row : data) {
        switch (row.type) {
        case Instruction_type::Do:
            add = true;
            break;
        case Instruction_type::Dont:
            add = false;
            break;
        case Instruction_type::Mult: {
            if (add) {
                number += row.left * row.right;
            }
        } break;
        }
    }
    std::cout << "part2 number: " << number << "\n";
}
