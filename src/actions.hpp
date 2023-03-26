#pragma once

#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include "article.hpp"


std::vector<size_t> Intersect(std::vector<size_t> &first, std::vector<size_t> &second){
    std::vector<size_t> output;
    // std::sort(first.begin(), first.end());
    // std::sort(second.begin(), second.end());
    // for(size_t i = 0; i < first.size(); ++i){
    //     if(read_article(first[i]).title == "Microsoft Publisher"){
    //         std::cout << "Yes first!" << std::endl;
    //     }
        
    //    //std::cout << read_article(ans[i]).url << std::endl;
       
    // }
    // for(size_t i = 0; i < second.size(); ++i){
    //     if(read_article(second[i]).title == "Microsoft Publisher"){
    //         std::cout << "Yes second!" << std::endl;
    //     }
        
    //    //std::cout << read_article(ans[i]).url << std::endl;
       
    // }
    // auto first_index = first.begin();
    // auto second_index = second.begin();
    // while(first_index != first.end() && second_index != second.end()){
    //     if (*first_index == *second_index){
    //         output.push_back(*first_index);
    //         ++first_index;
    //         ++second_index;
    //     }else if(*first_index < *second_index){
    //         ++first_index;
    //     }else{
    //         ++second_index;
    //     }
    // }
    std::set_intersection(first.begin(), first.end(), second.begin(), second.end(),
                          std::back_inserter(output));

    for(size_t i = 0; i < output.size(); ++i){
        if(read_article(output[i]).title == "Microsoft Publisher"){
            std::cout << "Yes output!" << std::endl;
        }
        
       //std::cout << read_article(ans[i]).url << std::endl;
       
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




