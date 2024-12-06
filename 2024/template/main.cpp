#include <file.hpp>
#include <iostream>

void part1();
void part2();

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " <filename>\n";
        return 1;
    }
    const char *filepath = argv[1];
}
