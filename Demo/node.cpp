#include "node.h"
#include <QPainter>
#include <QStyleOption>
#include "edge.h"

int Node::idStatic = 0;

Node::Node(QColor nodeClr, QString text, QColor textColor) : nodeColor(nodeClr), textContent(text)
{
    id = idStatic;
    idStatic++;
    pen.setColor(textColor); pen.setWidth(2);
    if(text.isEmpty()) textContent = QString::number(idStatic);
    setFlag(ItemIsSelectable);
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(10);
}

Node::~Node()
{
    foreach(Edge *edge, edgeList) delete edge;
    idStatic--;
}

int Node::GetId() const noexcept {return id;}

int Node::GetidStatic() {return idStatic;}

void Node::addEdge(Edge *edge)
{
    if (!edgeList.contains(edge))
        edgeList << edge;
}

void Node::removeEdge(Edge *edge)
{
    QList<Edge*>::iterator it = edgeList.begin();
    while (*it != edge && it != edgeList.end()) ++it;
    if(it != edgeList.end()) edgeList.erase(it);
}

QList<Edge*> Node::edges() const
{
    return edgeList;
}

QRectF Node::boundingRect() const
{
    return QRectF(-Radius - pen.width()/2, -Radius - pen.width()/2,
                  2*Radius+pen.width(), 2*Radius+pen.width());
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    painter->setBrush((option->state & QStyle::State_Selected ? Qt::red: nodeColor));
    painter->setPen(pen);
    painter->drawEllipse(-Radius, -Radius, 2*Radius, 2*Radius);
    painter->setFont(QFont("Times", 12, QFont::Bold));
    painter->drawText(boundingRect(), Qt::AlignCenter, textContent);
    painter->restore();
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
        {
            edge->NodeUpdated(this);
        }
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

QColor Node::GetNodeColor() const noexcept
{
    return nodeColor;
}

void Node::SetNodeColor(QColor newColor, bool updating) noexcept
{
    nodeColor = newColor;
    if(updating) update(boundingRect());
}

QColor Node::GetPenColor() const noexcept
{
    return pen.color();
}

void Node::SetPenColor(QColor penColor, bool updating) noexcept
{
    pen.setColor(penColor);
    if(updating) update(boundingRect());
}
