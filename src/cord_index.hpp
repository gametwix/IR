#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>


#include "config.h"

void save_cord_index(std::vector<std::unordered_map<size_t, std::vector<size_t>>> &cord_index){
    std::ofstream file;
    
    for(size_t list_ind=0; list_ind < cord_index.size(); ++list_ind){
        if(list_ind % CORD_INDEX_FILE_SIZE == 0){
            file.close();
            std::string filepath = std::string(CORD_INDEX_DIR_PATH) + std::to_string(list_ind / CORD_INDEX_FILE_SIZE) + ".ind";
            file.open(filepath,std::ios_base::binary);
        }
        
        size_t size_list = cord_index[list_ind].size();
        file.write(reinterpret_cast<char *>(&size_list), sizeof(size_t));
        for (auto & x : cord_index[list_ind]){
            file.write((char *)(&x.first), sizeof(size_t));
            size_list = x.second.size();
            file.write((char *)(&size_list), sizeof(size_t));
            std::sort(cord_index[list_ind][x.first].begin(), cord_index[list_ind][x.first].end());
            for(size_t elem_ind=0; elem_ind < size_list; ++elem_ind){
                file.write((char *)(&x.second[elem_ind]), sizeof(size_t));
            }
        }
    }
    file.close();
}

std::unordered_map<size_t, std::vector<size_t>> load_cord_index(size_t id){
    if(id == 0){
        return std::unordered_map<size_t, std::vector<size_t>>();
    }
    std::string filename = std::to_string(id / CORD_INDEX_FILE_SIZE) + ".ind";
    std::string filepath = std::string(CORD_INDEX_DIR_PATH) + filename;
    std::ifstream file;
    std::unordered_map<size_t, std::vector<size_t>> output;
    file.open(filepath,std::ios_base::binary);
    if(file.good()){
        size_t ind = id - (id % CORD_INDEX_FILE_SIZE);
        while(file.peek() != EOF and ind <= id){
            size_t size_map;
            file.read(reinterpret_cast<char *> (&size_map), sizeof(size_t));
            if(id == ind){
                for(size_t i = 0; i < size_map; ++i){
                    size_t size_list;
                    size_t key_id;
                    file.read(reinterpret_cast<char *> (&key_id), sizeof(size_t)); //read id
                    file.read(reinterpret_cast<char *> (&size_list), sizeof(size_t));
                    output[key_id] = std::vector<size_t>(size_list);
                    for(size_t j = 0; j < size_list; ++j){
                        file.read((char *)(&output[key_id][j]), sizeof(size_t));
                    }
                }
            }else{
                for(size_t i = 0; i < size_map; ++i){
                    size_t size_list;
                    file.read(reinterpret_cast<char *> (&size_list), sizeof(size_t)); //read id
                    file.read(reinterpret_cast<char *> (&size_list), sizeof(size_t));
                    file.seekg(size_list*sizeof(size_t), std::ios_base::cur);
                }
            }
            ind++;
        } 
    }
    file.close();
    return output;
}