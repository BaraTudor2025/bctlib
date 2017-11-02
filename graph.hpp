#pragma once
#include <vector>

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
		return this->are_connected(node, nodeAdjacent);
	}

	bool are_connected(int node, int nodeAdjacent) const
	{
		if (node < nodeAdjacent)
			std::swap(node, nodeAdjacent);
		const int pos = group_position(node);
		const bool areConnected = data[pos + nodeAdjacent - 1];
		return areConnected;
	}

	void make_connection(int node1, int node2)
	{
		if (node1 < node2)
            std::swap(node1, node2);
		int pos = group_position(node1);
		data[pos + node2 - 1] = true;
	}

	void cut_connection(int node1, int node2)
	{
		if (node1 < node2)
			std::swap(node1, node2);
		int pos = group_position(node1);
		data[pos + node2 - 1] = false;
	}

	void remove_node(int node)
	{
        for(int i=1; i<this->size(); i++)
            this->cut_connection(i, node);
	}

	std::vector<int> adjacent_nodes(int node) const
	{
		std::vector<int> nodes;
		nodes.reserve(groups);
		auto func = [&nodes](int node) { nodes.push_back(node); };
        	this->iterate(node, func);
		return nodes;
	}

	int grade(int node) const
	{
		int counter = 0;
		auto func = [&counter](int node) { counter++; };
		this->iterate(node, func);
		return counter;
	}

	bool is_subgraph_of(const GrafNeorientat graf)
	{

	}

private:

    template <typename F>
	void iterate(int node, F func) const
	{
		int group_pos = group_position(node);

		// count the adjacent nodes in node's group
		for (int i = 1; i <= node; i++)
			if (data[group_pos + i - 1])
				func(i);

		/* with starting index 1 */
 		// count how many times node appears in the next groups
		// pos: advance to the next group and to node's position in group
		for (int curr_group = node,  pos = group_pos + curr_group + node - 1;
			 curr_group < groups;
			 curr_group ++, pos += curr_group)
			if (data[pos])
				func(curr_group + 1);

		/* with starting index 0, just in case */
		/*
		for (int curr_group = node + 1, pos = group_pos + curr_group + node;
			 curr_group < groups;
			 curr_group ++, pos += curr_group)
			if (data[pos])
				func(curr_group, NEXT_GROUPS);
		*/
	}

    //enum { FIRST_GROUP, NEXT_GROUPS};
	const int groups;
	std::vector<bool> data; // for debug use deque

	int group_position(int group) const { return group * (group - 1) / 2; }
};
