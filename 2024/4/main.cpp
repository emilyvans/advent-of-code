#include <cstdint>
#include <file.hpp>
#include <iostream>
#include <sys/types.h>
#include <vector>

void part1(std::vector<uint8_t> data, int width);
void part2(std::vector<uint8_t> data, int width);

enum class Direction {
    none,
    up,
    rightup,
    right,
    rightdown,
    down,
    leftdown,
    left,
    leftup,
};

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " <filename>\n";
        return 1;
    }
    const char *filepath = argv[1];
    File file(filepath);
    std::vector<uint8_t> data;
    uint32_t width = 0;
    uint8_t character;

    while (file.get_cursor() < file.m_file_length) {
        character = file.read8();
        if (character != 10) {
            data.push_back(character);
        } else if (width == 0) {
            width = file.get_cursor() - 1;
        }
    }

    std::cout << "width: " << width << "\n";

    // part1(data, width);
    part2(data, width);
}

Direction delta_to_direction(int32_t delta_x, int32_t delta_y) {
    if (delta_y == -1) {
        // up
        if (delta_x == -1) {
            // left
            return Direction::leftup;
        } else if (delta_x == 1) {
            // right
            return Direction::rightup;
        } else {
            // none
            return Direction::up;
        }
    } else if (delta_y == 1) {
        // down
        if (delta_x == -1) {
            // left
            return Direction::leftdown;
        } else if (delta_x == 1) {
            // right
            return Direction::rightdown;
        } else {
            // none
            return Direction::down;
        }
    } else {
        // none
        if (delta_x == -1) {
            // left
            return Direction::left;
        } else if (delta_x == 1) {
            // right
            return Direction::right;
        } else {
            // none
            return Direction::none;
        }
    }
}

int32_t direction_to_delta_x(Direction dir) {
    if (dir == Direction::leftdown || dir == Direction::leftup ||
        dir == Direction::left) {
        return -1;
    } else if (dir == Direction::rightdown || dir == Direction::rightup ||
               dir == Direction::right) {
        return 1;
    } else {
        return 0;
    }
}

int32_t direction_to_delta_y(Direction dir) {
    if (dir == Direction::leftup || dir == Direction::rightup ||
        dir == Direction::up) {
        return -1;
    } else if (dir == Direction::leftdown || dir == Direction::rightdown ||
               dir == Direction::down) {
        return 1;
    } else {
        return 0;
    }
}

uint32_t check_xmas(std::vector<uint8_t> data, uint32_t width, uint32_t x,
                    uint32_t y, Direction direction, char character_to_find) {
    uint32_t height = data.size() / width;
    uint32_t count = 0;
    if (direction == Direction::none) {
        uint32_t min_y = y - 1;
        uint32_t max_y = y + 1;
        uint32_t min_x = x - 1;
        uint32_t max_x = x + 1;

        if (((int32_t)y - 3) < 0) {
            min_y = y;
        }
        if (((int32_t)y + 3) > height) {
            max_y = height - 1;
        }
        if (((int32_t)x - 3) < 0) {
            min_x = x;
        }
        if (((int32_t)x + 3) > width) {
            max_x = width - 1;
        }
        // MMM
        // MXM
        // MMM
        for (uint32_t y1 = min_y; y1 <= max_y; y1++) {
            for (uint32_t x1 = min_x; x1 <= max_x; x1++) {
                int m = data[x1 + y1 * width];
                if (m == 'M') {
                    int32_t delta_y = y1 - y;
                    int32_t delta_x = x1 - x;
                    std::cout << "----------------------------------------\n";
                    std::cout << "[ ]M: (" << x1 << "," << y1 << ") (" << x
                              << "," << y << ") (" << delta_x << "," << delta_y
                              << ") ";
                    Direction direction1 = delta_to_direction(delta_x, delta_y);
                    bool correct =
                        check_xmas(data, width, x1, y1, direction1, 'A');
                    std::cout << correct << "\n";
                    if (correct == 1)
                        count += 1;
                }
            }
        }
        return count;
    }

    char character;

    switch (direction) {
    case Direction::none:
        break;
    case Direction::up:
        character = data[x + (y - 1) * width];
        break;
    case Direction::down:
        character = data[x + (y + 1) * width];
        break;
    case Direction::left:
        character = data[(x - 1) + y * width];
        break;
    case Direction::right:
        character = data[(x + 1) + y * width];
        break;
    case Direction::leftup:
        character = data[(x - 1) + (y - 1) * width];
        break;
    case Direction::rightup:
        character = data[(x + 1) + (y - 1) * width];
        break;
    case Direction::leftdown:
        character = data[(x - 1) + (y + 1) * width];
        break;
    case Direction::rightdown:
        character = data[(x + 1) + (y + 1) * width];
        break;
    }

    if (character == character_to_find) {
        if (character == 'S') {
            return 1;
        } else {
            return check_xmas(data, width, x + direction_to_delta_x(direction),
                              y + direction_to_delta_y(direction), direction,
                              'S');
        }
    } else {
        return 0;
    }
}

void part1(std::vector<uint8_t> data, int width) {
    uint32_t height = data.size() / width;
    uint32_t count = 0;
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            uint8_t character = data[x + y * width];
            if (character == 'X') {
                // std::cout << 'X';
                count += check_xmas(data, width, x, y, Direction::none, 'M');
            }
        }
    }

    std::cout << "count: " << count << "\n";
}

void part2(std::vector<uint8_t> data, int width) {
    uint32_t height = data.size() / width;
    uint32_t count = 0;
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            uint8_t character = data[x + y * width];
            if (character == 'A') {
                if (y <= 0 || x <= 0 || y >= height - 1 || x >= width - 1)
                    continue;
                uint32_t top_left_char = data[(x - 1) + (y - 1) * width];
                uint32_t top_right_char = data[(x + 1) + (y - 1) * width];
                uint32_t bottom_left_char = data[(x - 1) + (y + 1) * width];
                uint32_t bottom_right_char = data[(x + 1) + (y + 1) * width];

                uint32_t m_count = 0;
                uint32_t s_count = 0;

                if (top_left_char == 'M') {
                    m_count += 1;
                }

                if (top_right_char == 'M') {
                    m_count += 1;
                }

                if (bottom_left_char == 'M') {
                    m_count += 1;
                }

                if (bottom_right_char == 'M') {
                    m_count += 1;
                }

                if (top_left_char == 'S') {
                    s_count += 1;
                }

                if (top_right_char == 'S') {
                    s_count += 1;
                }

                if (bottom_left_char == 'S') {
                    s_count += 1;
                }

                if (bottom_right_char == 'S') {
                    s_count += 1;
                }

                if (m_count != 2 || s_count != 2)
                    continue;

#if 1
                if (top_left_char == 'M' && bottom_right_char == 'M')
                    continue;
                if (top_right_char == 'M' && bottom_left_char == 'M')
                    continue;
                if (top_left_char == 'S' && bottom_right_char == 'S')
                    continue;
                if (top_right_char == 'S' && bottom_left_char == 'S')
                    continue;
#elif 0
                if (top_left_char == 'M') {
                    if (bottom_right_char != 'S') {
                        continue;
                    }
                }

                if (top_right_char == 'M') {
                    if (bottom_left_char != 'S') {
                        continue;
                    }
                }
                if (bottom_left_char == 'M') {
                    if (top_right_char != 'S') {
                        continue;
                    }
                }

                if (bottom_right_char == 'M') {
                    if (top_left_char != 'S') {
                        continue;
                    }
                }
#endif
                std::cout << "----------" << x << "," << y << "------------\n"
                          << (uint8_t)top_left_char << "."
                          << (uint8_t)top_right_char << "\n."
                          << (uint8_t)character << ".\n"
                          << (uint8_t)bottom_left_char << "."
                          << (uint8_t)bottom_right_char << "\n\n";

                count += 1;
            }
        }
    }

    std::cout << "count: " << count << "\n";
}
