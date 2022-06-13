#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>
#include <QString>
#include <QPen>

class Edge;

class Node : public QGraphicsItem
{
public:
    Node(QColor nodeClr = Qt::white, QString text = nullptr, QColor textColor = Qt::black);
    ~Node();

    void addEdge(Edge *edge);
    void removeEdge(Edge *edge);
    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const override { return Type; }
    enum { Radius = 40 };

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int GetId() const noexcept;     //получить id вершины
    static int GetidStatic();       //получить количество вершин

    QColor GetNodeColor() const noexcept;
    void SetNodeColor(QColor newColor, bool updating = true) noexcept;

    QColor GetPenColor() const noexcept;
    void SetPenColor(QColor penColor, bool updating = true) noexcept;

private:
    QPen pen;
    QColor nodeColor;
    QList<Edge *> edgeList;
    QPointF newPos;
    int id;                     //id текущей вершины (объекта Node), также может интерпретироваться как индекс в матрице смежности
    static int idStatic;        //количество всех вершин
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    QString textContent;
};

#endif // NODE_H
