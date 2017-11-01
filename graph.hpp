#pragma once
#include <vector>
using namespace std;

/**
 * Implementaion of memory efficient graph
 * since nonoriented graphs are miror on the axis of the first diag we can cut one half
 * and keep the lower one
 * if we put the remaining matrix in a vector we can organize it in "groups"
 * groups grow in size gauss i.g. 0|10|010|0011|10100|100110|...
*/

class GrafNeorientat {

public:
	GrafNeorientat(int nodes)
		: groups(nodes)
		, data(nodes * (nodes + 1) / 2)
	{
	}

	int size() const { return groups; }

	bool operator()(int node, int nodeAdjacent) const
	{
		return this->areConnected(node, nodeAdjacent);
	}

	bool areConnected(int node, int nodeAdjacent) const
	{
		if (node < nodeAdjacent)
			swap(node, nodeAdjacent);
		const int pos = group_position(node);
		const bool areConnected = data[pos + nodeAdjacent - 1];
		return areConnected;
	}

	void makeConnection(int node1, int node2)
	{
		if (node1 < node2)
			swap(node1, node2);
		int pos = group_position(node1);
		data[pos + node2 - 1] = true;
	}

	void cutConnection(int node1, int node2)
	{
		if (node1 < node2)
			swap(node1, node2);
		int pos = group_position(node1);
		data[pos + node2 - 1] = false;
	}

	vector<int> adjacentNodes(int node) const
	{
		vector<int> nodes;
		nodes.reserve(groups);
		int group_pos = group_position(node);

		// count the adjacent nodes in node's group
		for (int i = 1; i <= node; i++)
			if (data[group_pos + i - 1])
				nodes.push_back(i);
		
		/* with index 0 */
 		// count how many times node appears in the next groups
		// pos: advance to the next group and to node's position in group
		for (int curr_group = node,  pos = group_pos + curr_group + node - 1;
			 curr_group < groups;
			 curr_group ++, pos += curr_group)
			if (data[pos])
				nodes.push_back(curr_group + 1);

		/* with index 0 */
		/*
		for (int curr_group = node + 1, pos = group_pos + curr_group + node;
			 curr_group < groups;
			 curr_group ++, pos += curr_group)
			if (data[pos])
				nodes.push_back(curr_group);
		*/
		return nodes;
	}

	int grade(int node) const
	{
		// TODO: de optimizat
		return adjacentNodes(node).size();
	}

	const int groups;
	std::vector<bool> data; // for debug use deque

	int group_position(int group) const { return group * (group - 1) / 2; }
};
