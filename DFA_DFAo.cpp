//
// Created by 曾少勋 on 2019/12/9.
//
#include <iostream>
#include <set>
#include <vector>
#include <map>
#include "type.h"
#include "DFA_DFAo.h"

using namespace std;

bool alreadyExistEqual(const set<set<Edge *>> &edgeGroups,
                       const set<Edge *> &edges,
                       map<Node *, int> &keyMaps);

set<pair<string, int>> changeEdges(const set<Edge *> &edges, map<Node *, int> &keyMaps);

/**
 * @param maps 所有的DFA状态节点
 */
FA *DFA_to_DFAo(set<Node *> &maps, Node *startPoint) {
    vector<set<Node *>> optimized;

    // 组的唯一编号
    // 加速查找过程
    int groupKey = 0;
    // 状态到组号的映射
    map<Node *, int> keyMaps;

    // 初始先划分出终态
    // 非终态的点的集合
    set<Node *> notEnd;
    for (auto node :maps) {
        if (node->type.empty()) {
            notEnd.insert(node);
            keyMaps.emplace(node, groupKey++);
        } else {
            // 如果是终态
            // 那么看看是否已经有相同的终态的集合
            // 如果有则直接加入
            // 如果还没有，则新增加一个集合
            bool found = false;
            // 这里要加上引用，否则无法更改原vector中的set
            for (auto &se:optimized) {
                for (auto e:se) {
                    if (e->type == node->type) {
                        se.insert(node);
                        found = true;
                        keyMaps.emplace(node, keyMaps[e]);
                        break;
                    }
                }
                if (found) break;
            }
            if (!found) {
                set<Node *> end = {node};
                optimized.push_back(end);
                keyMaps.emplace(node, groupKey++);
            }
        }
    }
    if (!notEnd.empty())
        optimized.push_back(notEnd);

    bool changed = true;
    while (changed) {
        changed = false;
        vector<set<Node *>> new_optimized;

        // 遍历当前的所有状态集合
        for (auto state:optimized) {
            int newStateCount = 0;
            map<set<pair<string, int>>, set<Node *>> newStatesMap;
            for (auto node:state) {
                // 将边和到达的点转化为边和到达的组号
                set<pair<string, int>> edge_group = changeEdges(node->next, keyMaps);
                // 如果转化后的出边的集合已经存在
                // 那么加入这个出边集合对应的状态
                if (newStatesMap.find(edge_group) != newStatesMap.end()) {
                    newStatesMap[edge_group].emplace(node);
                }
                    // 当前不存在此出边集合
                    // 新建一个状态
                else {
                    set<Node *> newState = {node};
                    newStatesMap.emplace(edge_group, newState);
                    newStateCount++;
                }
            }
            // 如果有新加入的状态
            if (newStateCount > 1) {
                changed = true;
                for (auto nsm:newStatesMap) {
                    set<Node *> newState;
                    for (auto node:nsm.second) {
                        newState.emplace(node);
                        if (newState.size() == 1) {
                            keyMaps[node] = groupKey++;
                        } else keyMaps[node] = groupKey;
                    }
                    new_optimized.push_back(newState);
                }
            } else {
                new_optimized.push_back(state);
            }
        }
        optimized = new_optimized;
    }
    set<int> sets;
    for (auto state:optimized) {
        set<pair<string, int>> oldStartEdges = changeEdges((*(state.begin()))->next, keyMaps);
        // 遍历原来的起点的出边
        for (auto edge:oldStartEdges) {
            sets.emplace(edge.second);
        }
    }
    cout << sets.size() << endl;
    // 存储组号和新建节点的映射
    map<int, Node *> reversedMap;
    FA *finalFA = new FA;
    for (auto state:optimized) {
        // 新建一个代表节点
        Node *representative = new Node;
        reversedMap.emplace(keyMaps[*(state.begin())], representative);
    }
    cout << reversedMap.size() << endl;
    for (auto state:optimized) {
        Node *representative = reversedMap[keyMaps[*state.begin()]];
        set<pair<string, int>> oldStartEdges = changeEdges((*(state.begin()))->next, keyMaps);
        // 遍历原来的起点的出边
        for (auto edge:oldStartEdges) {
//            if (reversedMap.find(edge.second) == reversedMap.end()) {
//                cout << edge.second << endl;
//                cout << representative->next.size() << endl;
//                cout << *(representative->type.begin()) << endl;
//            }
            Node *node = reversedMap[edge.second];
            representative->next.emplace(new Edge{edge.first, node});
        }
        set<string> endTypes;
        for (auto node:state) {
            if (!node->type.empty()) {
                endTypes.insert(node->type.begin(), node->type.end());
            }
        }
        representative->type = endTypes;
        // 如果该状态包含原来的起点节点
        // 那么将最后的状态机的起始状态设置为这个新建的代表节点
        if (state.find(startPoint) != state.end())
            finalFA->start = representative;
    }

//    cout << optimized.size() << endl;
    return finalFA;
}

/**
 * 将边和到达的点转化为边和到达的组号
 * @param edges
 * @param keyMaps
 * @return
 */
set<pair<string, int>> changeEdges(const set<Edge *> &edges, map<Node *, int> &keyMaps) {
    set<pair<string, int>> edge_group;
    for (auto edge:edges) {
        edge_group.emplace(edge->value, keyMaps[edge->node]);
    }
    return edge_group;
}

