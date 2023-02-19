#pragma once

#include <vector>


std::vector<size_t> Intersect(std::vector<size_t> &first, std::vector<size_t> &second){
    std::vector<size_t> output;
    auto first_index = first.begin();
    auto second_index = second.begin();
    while(first_index != first.end() && second_index != second.end()){
        if (*first_index == *second_index){
            output.push_back(*first_index);
            ++first_index;
            ++second_index;
        }else if(*first_index < *second_index){
            ++first_index;
        }else{
            ++second_index;
        }
    }
    return output;
}


std::vector<size_t> Union(std::vector<size_t> first, std::vector<size_t> second){
    std::vector<size_t> output;
    auto first_index = first.begin();
    auto second_index = second.begin();
    while(first_index != first.end() || second_index != second.end()){
        if(first_index != first.end() && second_index != second.end()){
            if (*first_index == *second_index){
                output.push_back(*first_index);
                ++first_index;
                ++second_index;
            }else if(*first_index < *second_index){
                output.push_back(*first_index);
                ++first_index;
            }else{
                output.push_back(*second_index);
                ++second_index;
            }
        } else if(first_index != first.end()){
            output.push_back(*first_index);
            ++first_index;
        } else {
            output.push_back(*second_index);
            ++second_index;
        }
    }
    return output;
}


std::vector<size_t> Negative(std::vector<size_t> &input, size_t documents_total){
    std::vector<size_t> output;
    auto index = input.begin();
    for(size_t i=1; i <= documents_total; ++i){
        if(index == input.end() || i < *index){
            output.push_back(i);
        }else if(i == *index){
            ++index;
        }
    }
    return output;
}




