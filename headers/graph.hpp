#pragma once
#include "common.hpp"

class Graph
{
public:
    struct Node;
    typedef std::pair<const Node*, int> Edge;

    struct Node
    {
        Node(ivec2 pos);

        ivec2 position;
        std::vector<Edge> neighbours;
    };

    std::vector<ivec2> PathTo(const ivec2& position) const;
    const Node* Get(int at) const;

    Node* Get(const ivec2& pos);
    Node* NewNode(const ivec2& position);

private:
    std::vector<Node> m_nodes;
};
