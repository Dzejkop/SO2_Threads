#include "graph.hpp"

Graph::Node *Graph::Get(const ivec2 &pos)
{
    for (uint i = 0; i < _nodes.size(); i++)
    {
        if (_nodes.at(i).position == pos)
            return &_nodes.at(i);
    }
    return nullptr;
}

Graph::Node* Graph::NewNode(const ivec2& position)
{
    _nodes.push_back(Node(position));
    return &(_nodes.front());
}

Graph::Node::Node(ivec2 pos)
    : position(pos)
{
}
