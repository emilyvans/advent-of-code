#!/bin/bash
clang++ -std=gnu++23 -ggdb -I../file/include/ ../file/file.cpp main.cpp -o program && time ./program $1
