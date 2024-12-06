#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <file.hpp>
#include <ios>
#include <iostream>
#include <vector>

void part1(std::vector<int32_t> first_row, std::vector<int32_t> second_row);
void part2(std::vector<int32_t> first_row, std::vector<int32_t> second_row);

int main() {
    File file("./input.txt", std::ios::in);
    // File file("./input_test.txt", std::ios::in);

    std::vector<int32_t> first_row;
    std::vector<int32_t> second_row;

    uint32_t full_number = 0;
    bool is_first_row = true;

    while (file.get_cursor() < file.m_file_length) {
        uint8_t character = file.read8();
        if (character == 32 || character == 10) {
            while (character == 32 && !(character == 10)) {
                character = file.read8();
            }
            if (is_first_row) {
                first_row.push_back(full_number);
                full_number = 0;
                is_first_row = false;
            } else {
                second_row.push_back(full_number);
                full_number = 0;
                is_first_row = true;
            }
        }
        if (character > 47 && character < 58) {
            int number = character - 48;

            full_number = (full_number * 10) + number;
        }
        if (!((character > 47 && character < 58) || character == 32 ||
              character == 10)) {
            std::cout << "unknown character: " << (uint32_t)character << "\n";
            return 1;
        }
    }

    part1(first_row, second_row);
    part2(first_row, second_row);

    return 0;
}

void part1(std::vector<int32_t> first_row, std::vector<int32_t> second_row) {
    std::sort(first_row.begin(), first_row.end());
    std::sort(second_row.begin(), second_row.end());

    uint32_t total_distance = 0;
    for (int i = 0; i < first_row.size(); i++) {
        total_distance += abs(first_row[i] - second_row[i]);
    }

    std::cout << "total distance: " << total_distance << "\n";
}

void part2(std::vector<int32_t> first_row, std::vector<int32_t> second_row) {
    uint32_t sum = 0;

    for (uint32_t item : first_row) {
        uint32_t count = 0;
        for (uint32_t item1 : second_row) {
            if (item == item1) {
                count++;
            }
        }
        sum += item * count;
    }

    std::cout << "similarity score: " << sum << "\n";
}
