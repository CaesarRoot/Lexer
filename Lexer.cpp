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


//    string re1 = parse_to_prefix_form("((|0|1|2|3|4|5|6|7|8|9)*)");
//    string re2 = parse_to_prefix_form("ca");
//    string re3 = parse_to_prefix_form("a*");
//    regs.push_back({"name1", re1});
//    regs.push_back({"name2", re2});
//    regs.push_back({"name3", re3});
//    NFA_to_DFA(REs_to_NFA(regs));
//    string re = parse_to_prefix_form("a|b");
//    vector<RE> regs;
//    regs.push_back({"A|B", re});
//    NFA_to_DFA(REs_to_NFA(regs));
    return 0;
}
