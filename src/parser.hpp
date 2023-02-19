#pragma once

std::string AND = "&&";
std::string OR = "||";
std::string NOT = "!";

#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <map>

#include "article.hpp"
#include "reverse_index.hpp"
#include "dictionary.hpp"
#include "actions.hpp"


class Tocken{
    public:
        // type: 0-wortd, 1-op
        size_t type;
        std::string tocken;
        Tocken(std::string _tocken, size_t _type): tocken(_tocken), type(_type){}
};


std::vector<Tocken> parse_request(std::string request){
    std::string word;
    std::vector<Tocken> answer;
    bool be_op = true;
    for(size_t i = 0; i < request.size();){
        if(isalnum(request[i])){
            if(!be_op){
                answer.push_back(Tocken(AND, 1));
            }
            while(isalnum(request[i]))
            {
                word += tolower(request[i]);
                ++i;
            }
            answer.push_back(Tocken(word, 0));
            word.clear();
            be_op = false;
        }else if(request[i] == ' '){
            ++i;
        }else{
            if(request[i] == '('){
                if(!be_op){
                    answer.push_back(Tocken(AND, 1));
                }
                be_op = true;
                answer.push_back(Tocken("(", 1));
                ++i;
            }else if(request[i] == ')'){
                answer.push_back(Tocken(")", 1));
                ++i;
            }else if(request.substr(i,AND.size()) == AND){
                be_op = true;
                answer.push_back(Tocken(AND, 1));
                i+= AND.size();
            }else if(request.substr(i,OR.size()) == OR){
                be_op = true;
                answer.push_back(Tocken(OR, 1));
                i+= OR.size();
            }else if(request.substr(i,NOT.size()) == NOT){
                if(!be_op){
                    answer.push_back(Tocken(AND, 1));
                }
                be_op = true;
                answer.push_back(Tocken(NOT, 1));
                i+= NOT.size();
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
        if(input[i].type == 0){
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

    std::stack<std::vector<size_t>> stack_data;
    for(size_t i = 0; i < queue.size(); ++i){
        if(queue[i].type == 0){
            stack_data.push(load_word_index(dictionary[queue[i].tocken]));
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
    return stack_data.top();
}