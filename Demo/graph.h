#ifndef GRAPH_H
#define GRAPH_H

#include <QGraphicsView>
#include <QRandomGenerator>
#include <QKeyEvent>
#include "node.h"
#include "edge.h"

class Graph : public QGraphicsView
{
    Q_OBJECT
public:
    Graph(QWidget *parent = nullptr);

    void itemMoved();
public slots:
    void addNode();
    void shuffle();
    void zoomIn();
    void zoomOut();
protected:
    void keyPressEvent(QKeyEvent *event) override;
    //void contextMenuEvent(QContextMenuEvent *event);
    void mousePressEvent(QMouseEvent *event) override;
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
#endif
    void scaleView(qreal scaleFactor);
    void mouseReleaseEvent(QMouseEvent *event) override;
signals:
    void editItem();
};

#endif // GRAPH_H
