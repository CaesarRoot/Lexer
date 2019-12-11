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

    FA *fa = REs_to_NFA(regs);
    fa = NFA_to_DFA(fa);
    printTable(fa);
    cout << " ----------------- " << endl;
    FA* finalFA = DFA_to_DFAo(fa);
    printTable(finalFA);

    predict("aaaaaaaa", finalFA->start);
    predict("10.0", finalFA->start);
    predict("1a", finalFA->start);
    predict("3", finalFA->start);
    predict("while", finalFA->start);
    predict("auto", finalFA->start);
    return 0;
}
