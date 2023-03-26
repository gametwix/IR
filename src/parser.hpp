#pragma once

std::string AND = "&&";
std::string OR = "||";
std::string NOT = "!";

#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <cmath>
#include <algorithm>

#include "article.hpp"
#include "reverse_index.hpp"
#include "cord_index.hpp"
#include "dictionary.hpp"
#include "actions.hpp"

std::vector<size_t> sort_rank(std::vector<std::string> words, std::vector<size_t> to_sort);

class Tocken{
    public:
        // type: 0-wortd, 1-op, 2-quote
        size_t type;
        std::string tocken;
        std::vector<std::string> quote_tockens;
        size_t distance;
        Tocken(std::string _tocken, size_t _type): tocken(_tocken), type(_type){}
        Tocken(std::vector<std::string> _quote_tockens, size_t _type, size_t _distance): quote_tockens(_quote_tockens), type(_type), distance(_distance){}
        Tocken(std::vector<std::string> _quote_tockens, size_t _type): quote_tockens(_quote_tockens), type(_type){
            distance = quote_tockens.size() - 1;
        }
};


std::vector<size_t> Intersect_quote(std::vector<std::string> quote_tockens, size_t distance, std::unordered_map<std::string, size_t> &dictionary){
    
    std::cout << "enter" << std::endl;
    for(int i = 0; i<quote_tockens.size(); ++i){
        std::cout << quote_tockens[i] << " ";
    }
    std::cout << std::endl;
    std::vector<size_t> to_check = load_word_index(dictionary[quote_tockens[0]]);
    for(int i = 1; i < quote_tockens.size(); ++i){
        std::vector<size_t> second = load_word_index(dictionary[quote_tockens[i]]);
        to_check = Intersect(to_check, second);
    }
    if(to_check.size() == 0){
        return to_check;
    }
    std::cout << "before" << std::endl;
    std::unordered_map<size_t, std::vector<size_t>> to_generate = load_cord_index(dictionary[quote_tockens[0]]);
    std::cout << "after" << std::endl;
    std::unordered_map<size_t, std::vector<std::tuple<size_t, size_t>>> first;
    for(int i = 0; i < to_check.size();++i){
        first[to_check[i]] = std::vector<std::tuple<size_t, size_t>>();
        for(int j = 0; j < to_generate[to_check[i]].size(); ++j){
            first[to_check[i]].push_back({to_generate[to_check[i]][j], distance});
        }
    }
    std::cout << "exit" << std::endl;
    for(int i = 1; i < quote_tockens.size(); ++i){
        std::unordered_map<size_t, std::vector<size_t>> second = load_cord_index(dictionary[quote_tockens[i]]);
        std::unordered_map<size_t, std::vector<std::tuple<size_t, size_t>>> next;
        for (const auto& [first_id, first_positions] : first) {
            auto& second_positions = second[first_id];
            std::vector<std::tuple<size_t, size_t>> next_ps;
            for(int j = 0; j < first_positions.size(); ++j){
                for(int k = 0; k < second_positions.size(); ++k){
                    if(second_positions[k] - std::get<0>(first_positions[j]) <= std::get<1>(first_positions[j])){
                        next_ps.push_back({second_positions[k], std::get<1>(first_positions[j]) - (second_positions[k] - std::get<0>(first_positions[j]))});
                    }else if(second_positions[k] - std::get<0>(first_positions[j]) > std::get<1>(first_positions[j])){
                        break;
                    }
                }
            }
            if(next_ps.size() > 0){
                next[first_id] = next_ps;
            }
        }
        first = next;
    }
    std::vector<size_t> ans;
    for (const auto& [first_id, first_positions] : first) {
        ans.push_back(first_id);
    }
    return ans;
}

// std::vector<size_t> Intersect_quote(const std::vector<std::string>& quote_tokens, const size_t distance, const std::unordered_map<std::string, size_t>& dictionary) {
//     std::vector<size_t> to_check = load_word_index(dictionary.at(quote_tokens[0]));
//     for (size_t i = 1; i < quote_tokens.size(); ++i) {
//         std::vector<size_t> second = load_word_index(dictionary.at(quote_tokens[i]));
//         to_check = Intersect(to_check, second);
//     }
//     if (to_check.empty()) {
//         return to_check;
//     }
//     const std::unordered_map<size_t, std::vector<size_t>> to_generate = load_cord_index(dictionary.at(quote_tokens[0]));
//     std::unordered_map<size_t, std::vector<std::tuple<size_t, size_t>>> first;
//     for (const size_t id : to_check) {
//         std::vector<std::tuple<size_t, size_t>> positions;
//         const std::vector<size_t> generate_positions = to_generate.at(id);
//         positions.reserve(generate_positions.size());
//         for (const size_t pos : generate_positions) {
//             positions.emplace_back(pos, distance);
//         }
//         first.emplace(id, std::move(positions));
//     }
//     for (size_t i = 1; i < quote_tokens.size(); ++i) {
//         const std::unordered_map<size_t, std::vector<size_t>> second = load_cord_index(dictionary.at(quote_tokens[i]));
//         std::unordered_map<size_t, std::vector<std::tuple<size_t, size_t>>> next;
//         for (const auto& [id, positions] : first) {
//             const auto iter = second.find(id);
//             if (iter == second.end()) {
//                 continue;
//             }
//             const std::vector<size_t>& second_positions = iter->second;
//             std::vector<std::tuple<size_t, size_t>> next_positions;
//             for (const auto& [pos, dist] : positions) {
//                 for (const size_t second_pos : second_positions) {
//                     if (second_pos - pos <= dist) {
//                         next_positions.emplace_back(second_pos, dist - (second_pos - pos));
//                     }
//                     else {
//                         break;
//                     }
//                 }
//             }
//             if (!next_positions.empty()) {
//                 next.emplace(id, std::move(next_positions));
//             }
//         }
//         first = std::move(next);
//     }
//     std::vector<size_t> ans;
//     ans.reserve(first.size());
//     for (const auto& [id, positions] : first) {
//         ans.push_back(id);
//     }
//     return ans;
// }


std::vector<Tocken> parse_request(std::string request){
    std::string word;
    std::vector<Tocken> answer;
    bool be_op = true;
    bool start_quote = false;
    bool wait_distance = false;
    bool start_distance = false;
    bool is_fuzzy = true;
    Tocken quote(std::vector<std::string>(), 2);
    for(size_t i = 0; i < request.size();){
        if(isalnum(request[i])){
            if(!be_op and !start_quote and !start_distance){
               if(quote.quote_tockens.size() > 0){
                    quote.distance = quote.quote_tockens.size() - 1;
                    answer.push_back(quote);
                    std::cout << 0 << std::endl;
                    quote = Tocken(std::vector<std::string>(), 2);
                }
                wait_distance = false;
                // start_distance = false;
                answer.push_back(Tocken(AND, 1));
            }
            while(isalnum(request[i]))
            {
                word += tolower(request[i]);
                ++i;
            }
            if(start_quote){
                quote.quote_tockens.push_back(word);
            }else if(start_distance){
                if(quote.quote_tockens.size() > 0){
                    quote.distance = std::stoul(word);
                    std::cout << 1 << std::endl;
                    answer.push_back(quote);
                    quote = Tocken(std::vector<std::string>(), 2);
                }
                start_distance = false;
                be_op = false;
            }else{
                answer.push_back(Tocken(word, 0));
                be_op = false;
            }
            
            word.clear();
            
        }else if(request[i] == ' '){
            ++i;
        }else{
            if(request[i] == '('){
                is_fuzzy = false;
                if(quote.quote_tockens.size() > 0){
                    quote.distance = quote.quote_tockens.size() - 1;
                    std::cout << 3 << std::endl;
                    answer.push_back(quote);
                    quote = Tocken(std::vector<std::string>(), 2);
                }
                wait_distance = false;
                start_distance = false;
                if(!be_op){
                    answer.push_back(Tocken(AND, 1));
                }
                be_op = true;
                answer.push_back(Tocken("(", 1));
                ++i;
            }else if(request[i] == ')'){
                is_fuzzy = false;
                if(quote.quote_tockens.size() > 0){
                    quote.distance = quote.quote_tockens.size() - 1;
                    std::cout << 4 << std::endl;
                    answer.push_back(quote);
                    quote = Tocken(std::vector<std::string>(), 2);
                }
                wait_distance = false;
                // start_distance = false;
                answer.push_back(Tocken(")", 1));
                ++i;
            }else if(request.substr(i,AND.size()) == AND){
                is_fuzzy = false;
                if(quote.quote_tockens.size() > 0){
                    quote.distance = quote.quote_tockens.size() - 1;
                    std::cout << 5 << std::endl;
                    answer.push_back(quote);
                    quote = Tocken(std::vector<std::string>(), 2);
                }
                wait_distance = false;
                // start_distance = false;
                be_op = true;
                answer.push_back(Tocken(AND, 1));
                i+= AND.size();
            }else if(request.substr(i,OR.size()) == OR){
                is_fuzzy = false;
                if(quote.quote_tockens.size() > 0){
                    quote.distance = quote.quote_tockens.size() - 1;
                    std::cout << 6 << std::endl;
                    answer.push_back(quote);
                    quote = Tocken(std::vector<std::string>(), 2);
                }
                wait_distance = false;
                // start_distance = false;
                be_op = true;
                answer.push_back(Tocken(OR, 1));
                i+= OR.size();
            }else if(request.substr(i,NOT.size()) == NOT){
                is_fuzzy = false;
                if(quote.quote_tockens.size() > 0){
                    quote.distance = quote.quote_tockens.size() - 1;
                    std::cout << 7 << std::endl;
                    answer.push_back(quote);
                    quote = Tocken(std::vector<std::string>(), 2);
                }
                wait_distance = false;
                // start_distance = false;
                if(!be_op){
                    answer.push_back(Tocken(AND, 1));
                }
                be_op = true;
                answer.push_back(Tocken(NOT, 1));
                i+= NOT.size();
            }else if(request[i] == '\"'){
                is_fuzzy = false;
                
                wait_distance = false;
                ++i;
                start_quote = !start_quote;
                if(start_quote == false){
                    wait_distance = true;
                    be_op = false;
                }else{
                    if(quote.quote_tockens.size() > 0){
                        quote.distance = quote.quote_tockens.size() - 1;
                        std::cout << 8 << std::endl;
                        answer.push_back(quote);
                        quote = Tocken(std::vector<std::string>(), 2);
                    }
                    wait_distance = false;
                }
                              
            }else if(request[i] == '/'){
                is_fuzzy = false;
                ++i;
                wait_distance = false;
                start_distance = true;
            }else{
                is_fuzzy = false;
                i++;
            }
        }
    }
    if(quote.quote_tockens.size() > 0){
        is_fuzzy = false;
        quote.distance = quote.quote_tockens.size() - 1;
        std::cout << 9 << std::endl;
        answer.push_back(quote);
        quote = Tocken(std::vector<std::string>(), 2);
    }
    if(is_fuzzy){
        for(int i = 0; i < answer.size();++i){
            if(answer[i].type == 1){
                answer[i].tocken = OR;
            }
        }
    }
    return answer;
}

std::vector<Tocken> to_reverse_polish(std::vector<Tocken> input){
    std::vector<Tocken> output;
    std::stack<Tocken> stack_op;
    std::map<std::string, int> operationPriority = {
        {"(", 0},
        {OR, 1},
        {AND, 2},
        {NOT, 3}    
        };
    for(size_t i = 0; i < input.size(); ++i){
        if(input[i].type == 0 || input[i].type == 2){
            output.push_back(input[i]);
        }else if(input[i].tocken == "("){
            stack_op.push(input[i]);
        }else if(input[i].tocken == ")"){
            while (!stack_op.empty() && stack_op.top().tocken != "("){
                output.push_back(stack_op.top());
                stack_op.pop();
            }
            stack_op.pop();
        }else if(!stack_op.empty() and operationPriority[stack_op.top().tocken] > operationPriority[input[i].tocken]){
            while (!stack_op.empty() && operationPriority[stack_op.top().tocken] > operationPriority[input[i].tocken]){
                output.push_back(stack_op.top());
                stack_op.pop();
            }
            stack_op.push(input[i]);
        }else{
            stack_op.push(input[i]);
        }
    }
    while(!stack_op.empty()){
        output.push_back(stack_op.top());
        stack_op.pop();
    }
    return output;
}

std::vector<size_t> run_queue(std::vector<Tocken> queue){
    auto dictionary = load_dicionary();
    auto total_documents = get_total_documents();
    std::vector<std::string> words;

    std::stack<std::vector<size_t>> stack_data;
    for(size_t i = 0; i < queue.size(); ++i){
        
        if(queue[i].type == 0){
            std::cout << dictionary[queue[i].tocken] << std::endl;
            stack_data.push(load_word_index(dictionary[queue[i].tocken]));
            // std::cout << "word_index "<< queue[i].tocken << "   " << dictionary[queue[i].tocken] << std::endl;
            // for(int i = 0; i< load_word_index(dictionary[queue[i].tocken]).size(); ++i){
            //     load_word_index(dictionary[queue[i].tocken])[i];
            // }
            //std::cout << "word_index "<< queue[i].tocken << "   " << dictionary[queue[i].tocken] << std::endl;
            words.push_back(queue[i].tocken);
        }else if(queue[i].type == 2){
            std::cout << dictionary[queue[i].tocken] << std::endl;
            stack_data.push(Intersect_quote(queue[i].quote_tockens, queue[i].distance, dictionary));
            for(const auto& elem: queue[i].quote_tockens){
                words.push_back(elem);
            }
        }else if(queue[i].tocken == AND){
            auto first = stack_data.top();
            stack_data.pop();
            auto second = stack_data.top();
            stack_data.pop();
            stack_data.push(Intersect(first, second));
        }else if(queue[i].tocken == OR){
            auto first = stack_data.top();
            stack_data.pop();
            auto second = stack_data.top();
            stack_data.pop();
            stack_data.push(Union(first, second));
        }else if(queue[i].tocken == NOT){
            auto first = stack_data.top();
            stack_data.pop();
            stack_data.push(Negative(first, total_documents));
        }
    }
    //std::cout << stack_data.top().size() << std::endl;
    return sort_rank(words,stack_data.top());
}

std::vector<size_t> sort_rank(std::vector<std::string> words, std::vector<size_t> to_sort){

    for(size_t i = 0; i < to_sort.size(); ++i){
        if(read_article(to_sort[i]).title == "Microsoft Publisher"){
            std::cout << "Yes" << std::endl;
        }
        
       //std::cout << read_article(ans[i]).url << std::endl;
       
    }
    auto total_documents = get_total_documents();
    auto dictionary = load_dicionary();
    std::vector<double> ranks(to_sort.size(), 0.0);
    std::cout<< "-" << std::endl;
    for(int i = 0; i<to_sort.size(); ++i){
        article doc = read_article(to_sort[i]);
        std::string text = doc.title + " " + doc.text;
        size_t count_words = 0;
        std::unordered_map<std::string, size_t> count_word;
        std::string word;
        // std::cout << "one" << std::endl;
        for(const auto &c: text){
            if(isalnum(c)){
                word += tolower(c);
            }else if(word.length() > 0){
                if (count_word.find(word) != count_word.end()) {
                    
                    count_word[word]++;
                }else {
                    count_word[word] = 1;
                }
                count_words++;
                word.clear();
            }
        }
        // std::cout << "two" << std::endl;
        if(word.length() > 0){
            if (count_word.find(word) != count_word.end()) {
                count_word[word]++;
            }else {
                count_word[word] = 1;
            }
            count_words++;
        }
        // std::cout << "three" << std::endl;
        for(int j = 0; j < words.size(); ++j){
            if(dictionary[words[j]] == 0){
                continue;
            }
            size_t docs_with_word = load_word_index(dictionary[words[j]]).size();
            double tf = double(count_word[words[j]]) / double(count_words);
            double idf = log10(double(total_documents) / double(docs_with_word));
            ranks[i] += tf*idf;
            // std::cout << ranks[i] << std::endl; 
        }
        // std::cout << "end" << std::endl; 
    }

    std::vector<std::pair<double, size_t>> temp;
    for (int i = 0; i < to_sort.size(); i++) {
        temp.push_back(std::make_pair(ranks[i], to_sort[i]));
    }
    
    // сортируем временный вектор по значениям вторых элементов
    std::sort(temp.rbegin(), temp.rend());
    
    // перезаписываем отсортированные элементы первого массива в обратном порядке
    for (std::vector<std::pair<double, size_t>>::reverse_iterator it = temp.rbegin(); it != temp.rend(); ++it) {
        to_sort[temp.rend() - it - 1] = (*it).second;
    }
    std::cout<< "-" << std::endl;
    // std::cout << temp[0].first << std::endl;
    return to_sort;
}