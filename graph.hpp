#pragma once
#include <vector>

/**
 * Implementaion of memory efficient graph
 * TODO: documentatie
*/

class GrafNeorientat {

public:
    GrafNeorientat(int nodes)
    : groups (nodes)
    , data (nodes * (nodes + 1) / 2)
    { }

    bool operator()(int node, int nodeAdjacent)
    {
        return this->areAdjacent(node, nodeAdjacent);
    }

    bool areAdjacent(int node, int nodeAdjacent)
    {
        if(node < nodeAdjacent)
            swap(node, nodeAdjacent);
        const int pos = group_position(node);
        const bool areConnected = data[pos + nodeAdjacent - 1];
        return areConnected;
    }

    void makeConnection(int node1, int node2)
    {
        if(node1 < node2)
            swap(node1, node2);
        int pos = group_position(node1);
        data[pos + node2 - 1] = true;
    }

    void cutConnection(int node1, int node2)
    {
        if(node1 < node2)
            swap(node1, node2);
        int pos = group_position(node1);
        data[pos + node2 - 1] = false;
    }

    vector<int> adjacentNodes(int node)
    {
        vector<int> nodes;
        nodes.reserve(groups);
        int group = group_position(node);

        // numaram aparitiile lui node in grupa lui
        for(int i=1; i<=node; i++)
            if(data[group + i - 1])
                nodes.push_back(i);

        // numara aparitiile lui node in grupele urmatoare
        // pe pozitiile lui node
        int pos = node;
        for(int group_number = pos + 1, group += pos;
            group_number <= groups;
            group += pos, group_number++)
            if(data[group + pos - 1])
                nodes.push_back(pos);
        return nodes;
    }

private:
    const int groups;
    std::vector<bool> data;

    int group_position(int group) { return group * (group - 1) / 2; }
};
