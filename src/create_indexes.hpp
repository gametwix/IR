#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "config.h"

void create_indexes(std::vector<std::vector<size_t>> &reverse_index,
    std::vector<std::unordered_map<size_t, std::vector<size_t>>> &cord_index,
    std::unordered_map<std::string, size_t> &dictionary,
    const std::string &text, size_t id){
    std::string word;
    size_t pos = 0;
    
    for(const auto &c: text){
        // std::cout << "start" << std::endl;
        // std::cout << "word: "<< word << " " << dictionary[word] << std::endl;
        if(isalnum(c)){
            word += tolower(c);
        }else if(word.length() > 0){
            if(dictionary[word] == 0){
                word.clear();
                continue;
            }
            if(reverse_index[dictionary[word]].size() == 0 || 
                reverse_index[dictionary[word]][reverse_index[dictionary[word]].size()-1] != id){
                    reverse_index[dictionary[word]].push_back(id);
                }

            if(cord_index[dictionary[word]].find(id) == cord_index[dictionary[word]].end()){
                // std::cout << id << std::endl;
                cord_index[dictionary[word]][id] = std::vector<size_t>();
            }
            // std::cout << "add " << pos << std::endl;
            cord_index[dictionary[word]][id].push_back(pos);
            // std::cout << "clear" << std::endl;
            word.clear();
            pos++;
        }
    }
    if(word.length() > 0){
        if(reverse_index[dictionary[word]].size() == 0 || 
            reverse_index[dictionary[word]][reverse_index[dictionary[word]].size()-1] != id){
                reverse_index[dictionary[word]].push_back(id);
            }
        if(reverse_index[dictionary[word]].size() == 0 || 
                reverse_index[dictionary[word]][reverse_index[dictionary[word]].size()-1] != id){
                    reverse_index[dictionary[word]].push_back(id);
                }

            if(cord_index[dictionary[word]].find(id) == cord_index[dictionary[word]].end()){
                // std::cout << id << std::endl;
                cord_index[dictionary[word]][id] = std::vector<size_t>();
            }
            // std::cout << "add " << pos << std::endl;
            cord_index[dictionary[word]][id].push_back(pos);
            // std::cout << "clear" << std::endl;
            word.clear();
            pos++;
    }
}