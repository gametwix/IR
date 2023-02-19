#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

std::unordered_map<std::string, size_t> load_dicionary(){
    std::ifstream file;
    file.open(std::string(DICTIONARY_DIR_PATH) + "dictionary.words", std::ios_base::binary);
    size_t id;
    size_t str_size;
    std::string word;
    std::unordered_map<std::string, size_t> dicionary;
    while(file.peek() != EOF){
        file.read(reinterpret_cast<char *> (&id), sizeof(size_t));
        file.read(reinterpret_cast<char *> (&str_size), sizeof(size_t));
        word.resize(str_size);
        file.read(&word[0], str_size);
        dicionary[word] = id;
    }
    file.close();
    return dicionary;
}


