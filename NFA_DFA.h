//
// Created by 曾少勋 on 2019/12/9.
//

#ifndef LEXER_NFA_DFA_H
#define LEXER_NFA_DFA_H

#include "type.h"
#include <map>
#include <set>

using namespace std;

void NFA_to_DFA(FA *fa,
                map<set<Node *>, Node *> &maps,
                map<Node *, set<Node *>> &reverseMap,
                map<set<Node *>, int> &numsMap,
                Node *startPoint,
                set<Node *> &endPoints);
void predict(string word, Node *startPoint);

#endif //LEXER_NFA_DFA_H
