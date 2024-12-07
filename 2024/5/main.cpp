#include <cstdint>
#include <file.hpp>
#include <iostream>
#include <vector>

struct Instruction {
    uint32_t before;
    uint32_t after;
};

void part1(std::vector<Instruction> instructions,
           std::vector<std::vector<uint32_t>> rows);
void part2(std::vector<Instruction> instructions,
           std::vector<std::vector<uint32_t>> rows);

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " <filename>\n";
        return 1;
    }
    const char *filepath = argv[1];
    File file(filepath);

    std::vector<Instruction> instructions;
    std::vector<std::vector<uint32_t>> rows;

    bool is_instruction_section = true;
    uint8_t character;

    while (file.get_cursor() < file.m_file_length) {
        if (is_instruction_section) {
            character = file.read8();
            Instruction instruction{};
            if (character == '\n') {
                is_instruction_section = false;
                continue;
            }
            while (character != '|') {
                if (character > 47 && character < 58) { // 0-9
                    instruction.before =
                        instruction.before * 10 + (character - 48);
                } else {
                    std::cerr << "uknown char: " << character << "("
                              << (uint32_t)character << ")\n";
                    return 1;
                }
                character = file.read8();
            }
            character = file.read8();

            while (character != '\n') {
                if (character > 47 && character < 58) { // 0-9
                    instruction.after =
                        instruction.after * 10 + (character - 48);
                } else {
                    std::cerr << "uknown char: " << character << "("
                              << (uint32_t)character << ")\n";
                    return 1;
                }
                character = file.read8();
            }
            instructions.push_back(instruction);
        } else {

            std::vector<uint32_t> row;
            character = file.read8();
            bool reading_row = true;
            while (reading_row) {
                uint32_t number = 0;
                bool reading_number = true;

                while (reading_number) {
                    if (character > 47 && character < 58) { // 0-9
                        number = number * 10 + (character - 48);
                    } else {
                        std::cerr << "uknown char: " << character << "("
                                  << (uint32_t)character << ")\n";
                        return 1;
                    }
                    character = file.read8();
                    if (character == ',') {
                        character = file.read8();
                        reading_number = false;
                    } else if (character == '\n') {
                        reading_number = false;
                        reading_row = false;
                    }
                }
                row.push_back(number);
            }
            rows.push_back(row);
        }
    }

    for (Instruction inst : instructions) {
        std::cout << inst.before << ">" << inst.after << "\n";
    }
    for (std::vector<uint32_t> row : rows) {

        for (uint32_t i = 0; i < row.size(); i++) {
            if (i != 0) {
                std::cout << ", ";
            }
            std::cout << row[i];
        }
        std::cout << "\n";
    }
    std::cout << "----------------\n";
    part1(instructions, rows);
    part2(instructions, rows);
}

void part1(std::vector<Instruction> instructions,
           std::vector<std::vector<uint32_t>> rows) {
    std::vector<bool> corrects;
    for (std::vector<uint32_t> row : rows) {
        bool correct = true;
        for (Instruction instruction : instructions) {
            int32_t before_index = -1;
            int32_t after_index = -1;
            for (uint32_t i = 0; i < row.size(); i++) {
                uint32_t item = row[i];
                if (item == instruction.before) {
                    before_index = i;
                } else if (item == instruction.after) {
                    after_index = i;
                }
            }
            if ((before_index > after_index) && (before_index != -1) &&
                (after_index != -1)) {
                correct = false;
            }
        }
        corrects.push_back(correct);
    }

    uint32_t sum = 0;
    for (uint32_t i = 0; i < rows.size(); i++) {
        if (!corrects[i])
            continue;

        std::vector<uint32_t> row = rows[i];
        uint32_t middle = row.size() / 2;

        for (uint32_t i = 0; i < row.size(); i++) {
            if (i != 0) {
                std::cout << ", ";
            }
            if (i == middle) {
                std::cout << "\033[42;1;1m" << row[i] << "\033[0m";
                sum += row[i];
            } else {
                std::cout << row[i];
            }
        }
        std::cout << "\n";
    }

    std::cout << "sum = " << sum << "\n";
}

void part2(std::vector<Instruction> instructions,
           std::vector<std::vector<uint32_t>> rows) {
    std::vector<bool> corrects;
    for (std::vector<uint32_t> row : rows) {
        bool correct = true;
        for (Instruction instruction : instructions) {
            int32_t before_index = -1;
            int32_t after_index = -1;
            for (uint32_t i = 0; i < row.size(); i++) {
                uint32_t item = row[i];
                if (item == instruction.before) {
                    before_index = i;
                } else if (item == instruction.after) {
                    after_index = i;
                }
            }
            if ((before_index > after_index) && (before_index != -1) &&
                (after_index != -1)) {
                correct = false;
            }
        }
        corrects.push_back(correct);
    }

    int sum = 0;

    for (uint32_t i = 0; i < rows.size(); i++) {
        if (corrects[i])
            continue;
        std::vector<uint32_t> row = rows[i];
        bool correct = false;
        bool is_first_instruction = true;
        while (!correct) {
            std::vector<bool> instruction_corrects;
            for (Instruction instruction : instructions) {
                int32_t before_index = -1;
                int32_t after_index = -1;
                for (uint32_t i = 0; i < row.size(); i++) {
                    uint32_t item = row[i];
                    if (item == instruction.before) {
                        before_index = i;
                    } else if (item == instruction.after) {
                        after_index = i;
                    }
                }
                if ((before_index > after_index) && (before_index != -1) &&
                    (after_index != -1)) {
                    instruction_corrects.push_back(false);
                    uint32_t after = row[after_index];
                    row[after_index] = row[before_index];
                    row[before_index] = after;
                } else {
                    instruction_corrects.push_back(true);
                }
            }
            correct = true;
            for (bool instruction_correct : instruction_corrects) {
                if (!instruction_correct) {
                    correct = false;
                }
            }
        }
        sum += row[row.size() / 2];
    }
    std::cout << "sum = " << sum << "\n";
}
