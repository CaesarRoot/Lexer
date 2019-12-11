//
// Created by 曾少勋 on 2019/12/8.
//
#include <iostream>
#include "RE_Suffix.h"
#include "RE_NFA.h"
#include "type.h"
#include "NFA_DFA.h"
#include "READ_file.h"
#include <map>
#include <set>
#include "DFA_DFAo.h"
#include "Utils.h"

using namespace std;

int main() {
    vector<RE> regs;
    readFile(regs);
    for (auto r:regs) {
        cout << r.rules << endl;
    }

    map<set<Node *>, Node *> maps;
    map<Node *, set<Node *>> reverseMap;
    map<set<Node *>, int> numsMap;
    Node *startPoint = new Node;
    set<Node *> endPoints;

    FA *fa = REs_to_NFA(regs);
    NFA_to_DFA(fa, maps, reverseMap, numsMap, startPoint, endPoints);
    set<Node*> nodes;
    for(auto n:maps){
        nodes.emplace(n.second);
    }
    FA* finalFA = DFA_to_DFAo(nodes, startPoint);
    printTable(finalFA);

    predict("aaaaaaaa", finalFA->start);
    predict("10.0", finalFA->start);
    predict("1a", finalFA->start);
    predict("3", finalFA->start);
    predict("while", finalFA->start);
    predict("auto", finalFA->start);
    return 0;
}
