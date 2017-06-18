#include "graph.hpp"

Graph::Node *Graph::Get(const ivec2 &pos)
{
    for (uint i = 0; i < m_nodes.size(); i++)
    {
        if (m_nodes.at(i).position == pos)
            return &m_nodes.at(i);
    }
    return nullptr;
}

Graph::Node* Graph::NewNode(const ivec2& position)
{
    m_nodes.push_back(Node(position));
    return &(m_nodes.front());
}

Graph::Node::Node(ivec2 pos)
    : position(pos)
{
}
