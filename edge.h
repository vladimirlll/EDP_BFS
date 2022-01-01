#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsLineItem>
#include <QString>
#include <QList>

class Node;

class Edge : public QGraphicsLineItem
{
public:
    Edge(Node* startNode, Node* endNode = nullptr);
    ~Edge();

    enum { Type = UserType + 2 };

    Node* GetStartNode() const noexcept;
    Node* GetEndNode() const noexcept;

protected:
    Node* startNode;        //указатель на начальную вершину
    Node* endNode;          //указатель на конечную вершинцу
};

#endif // EDGE_H
