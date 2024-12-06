#include <algorithm>
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <file.hpp>
#include <ios>
#include <iostream>
#include <vector>

void part1(std::vector<std::vector<uint32_t>> data);
void part2();

template <typename T>
void remove_at(std::vector<T> &a,
               int index) // function template to work with all type of vectors
{
    for (int i = index; i < a.size() - 1; i++) {
        std::swap(a[i], a[i + 1]); // swaping elements till the end
    }
    a.pop_back();
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " <filename>\n";
        return 1;
    }
    const char *filepath = argv[1];
    File file(filepath, std::ios::in);

    std::vector<std::vector<uint32_t>> data;

    uint32_t number = 0;

    std::vector<uint32_t> row;

    while (file.get_cursor() < file.m_file_length) {
        uint8_t character = file.read8();
        if (character > 47 && character < 58) {
            uint32_t temp_number = character - 48;
            number = number * 10 + temp_number;
        } else if (character == 32) {
            row.push_back(number);
            number = 0;
        } else if (character == 10) {
            row.push_back(number);
            number = 0;
            data.push_back(row);
            row = std::vector<uint32_t>();
        }
    }

    uint32_t safe_count = 0;
    std::vector<bool> safes;
    std::vector<std::vector<uint32_t>> data_fixed;

    for (std::vector<uint32_t> sub_data : data) {
        std::vector<uint32_t> sub_data_fixed;
        for (int i = 0; i < sub_data.size(); i++) {
            sub_data_fixed.push_back(sub_data[i]);
        }
        data_fixed.push_back(sub_data_fixed);
    }

    for (uint32_t j = 0; j < data.size(); j++) {
        std::vector<uint32_t> sub_data = data[j];
        bool safe = true;
        bool first_unsafe_happend = false;
        bool accending = true;

        for (int i = 0; i < sub_data.size(); i++) {
            // if (i != 0)
            // std::cout << " ";
            // std::cout << sub_data[i];
            if (i != sub_data.size() - 1) {
                if (i == 0) {
                start:
                    if (sub_data[i] > sub_data[i + 1]) {
                        accending = false;
                    } else {
                        accending = true;
                    }
                }
                int32_t number;
                if (accending) {
                    number = sub_data[i + 1] - sub_data[i];
                } else {
                    number = sub_data[i] - sub_data[i + 1];
                }

                if (!(number > 0 && number < 4)) {
                    // std::cout << "[" << number << "]";
                    if (first_unsafe_happend) {
                        safe = false;
                    } else {
                        // std::cout << "hit\n";
                        // remove_at(sub_data, i);
                        //  sub_data.erase(sub_data.begin() + i);
                        first_unsafe_happend = true;
                        goto start;
                    }
                }
            }
        }
        safes.push_back(safe);
        // std::cout << "| safe: " << safe << "\n";
        if (safe)
            safe_count += 1;
    }

    uint32_t n = 0;
    safes[1] = true; // known unsafe
    safes[2] = true; // known unsafe

    for (int i = 0; i < data.size(); i++) {
        std::vector<uint32_t> sub_data = data[i];
        std::vector<uint32_t> sub_data_fixed = data_fixed[i];
        if (!safes[i]) {
            std::cout << i + 1 << ": ";
            for (int j = 0; j < sub_data.size(); j++) {
                if (j != 0) {
                    std::cout << " ";
                }
                std::cout << sub_data[j];
            }
            // std::cout << "\\n";
            std::cout << "\n" << i + 1 << ": ";

            for (int j = 0; j < sub_data_fixed.size(); j++) {
                if (j != 0) {
                    std::cout << " ";
                }
                std::cout << sub_data_fixed[j];
            }

            std::cout << "\n";
            // std::cout << " | safe: " << safes[i] << "\n";
            n += 1;
        }
    }

    std::cout << "safe count: " << safe_count << "\n";

    return 0;
}
