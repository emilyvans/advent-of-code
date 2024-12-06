#!/bin/bash
clang++ -std=gnu++23 -I../file/include/ ../file/file.cpp main.cpp -o program && time ./program 
