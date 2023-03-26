#pragma once

#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>



#include "config.h"

void reverse_index_add(std::vector<std::vector<size_t>> &reverse_index,
    std::unordered_map<std::string, size_t> &dictionary,
    const std::string &text, size_t id){
        std::string word;
        for(const auto &c: text){
            if(isalnum(c)){
                word += tolower(c);
            }else if(word.length() > 0){
                if(reverse_index[dictionary[word]].size() == 0 || 
                    reverse_index[dictionary[word]][reverse_index[dictionary[word]].size()-1] != id){
                        reverse_index[dictionary[word]].push_back(id);
                    }
                word.clear();
            }
        }
        if(word.length() > 0){
            if(reverse_index[dictionary[word]].size() == 0 || 
                reverse_index[dictionary[word]][reverse_index[dictionary[word]].size()-1] != id){
                    reverse_index[dictionary[word]].push_back(id);
                }
        }
    }

void save_reverse_index(std::vector<std::vector<size_t>> &reverse_index){
    std::ofstream file;
    
    for(size_t list_ind=0; list_ind < reverse_index.size(); ++list_ind){
        if(list_ind % BIN_REVESE_INDEX_FILE_SIZE == 0){
            file.close();
            std::string filepath = std::string(REVERSE_INDEX_DIR_PATH) + std::to_string(list_ind / BIN_REVESE_INDEX_FILE_SIZE) + ".ind";
            file.open(filepath,std::ios_base::binary);
        }
        std::sort(reverse_index[list_ind].begin(), reverse_index[list_ind].end());
        size_t size_list = reverse_index[list_ind].size();
        file.write(reinterpret_cast<char *>(&size_list), sizeof(size_t));
        for(size_t elem_ind=0; elem_ind < size_list; ++elem_ind){
            file.write((char *)(&reverse_index[list_ind][elem_ind]), sizeof(size_t));
        }
        
    }
    file.close();
}

std::vector<size_t> load_word_index(size_t id){
    // std::cout << "inside" << std::endl;
    if(id == 0){
        return std::vector<size_t>();
    }
    // std::cout << "not 0" << std::endl;
    std::string filename = std::to_string(id / BIN_REVESE_INDEX_FILE_SIZE) + ".ind";
    std::string filepath = std::string(REVERSE_INDEX_DIR_PATH) + filename;
    std::ifstream file;
    // std::cout << filepath << std::endl;
    std::vector<size_t> output;
    file.open(filepath,std::ios_base::binary);
    if(file.good()){
        // std::cout << "file open" << std::endl;
        size_t ind = id - (id % BIN_REVESE_INDEX_FILE_SIZE);
        // std::cout << "start index  " << ind << std::endl;
        while(file.peek() != EOF and ind <= id){
            //  std::cout << "ind  " << ind << std::endl;
            size_t size_list;
            file.read(reinterpret_cast<char *> (&size_list), sizeof(size_t));
            if(id == ind){
                output.resize(size_list);
                for(size_t i = 0; i < size_list; ++i){
                    file.read((char *)(&output[i]), sizeof(size_t));
                }
            }else{
                file.seekg(size_list*sizeof(size_t), std::ios_base::cur);
            }
            ind++;
        } 
        // std::cout << "before close" << std::endl;
        file.close();
    }
    // std::cout << "exit" << std::endl;
    return output;
}