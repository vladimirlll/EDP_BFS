#include "edge.h"
#include "node.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

Edge::Edge(Node* startNode, Node* endNode)
{
    this->startNode = startNode; this->endNode = endNode;
    if(!endNode) this->endNode = this->startNode;
    startNode->addEdge(this);
    if(startNode != endNode) endNode->addEdge(this);
    setLine(startNode->x(), startNode->y(), endNode->x(), endNode->y());
}

Edge::~Edge()
{
    startNode->removeEdge(this);
    if (startNode != endNode)
        endNode->removeEdge(this);
}

Node* Edge::GetStartNode() const noexcept {return startNode;}
Node* Edge::GetEndNode() const noexcept {return endNode;}

