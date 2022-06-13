#include "graph.h"

Graph::Graph(QWidget *parent)
    : QGraphicsView(parent)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200, -200, 500, 500);
    setMinimumSize(500, 500);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
}

void Graph::addNode()
{
    static int x = 0, y = -100;
    Node *node1 = new Node();
    scene()->addItem(node1);
    switch ((Node::GetidStatic() - 1) % 2)
    {
    case 0:
        x -= 2 * Node::Radius + 10;
        y += 2 * Node::Radius + 10;
        break;
    case 1:
        x += 2 * Node::Radius + 10;
        break;
    }
    node1->setPos(x, y);
}

void Graph::mousePressEvent(QMouseEvent *event){
    switch(event->button())
    {
    case Qt::RightButton:
        if (scene()->selectedItems().size()>0)
        {
            emit Graph::editItem();
        }
        break;

    default:

        break;
    }
    QGraphicsView::mousePressEvent(event);
}

void Graph::keyPressEvent(QKeyEvent *event)
{
    Node *selectedItem = nullptr;
    if (scene()->selectedItems().size() == 1)
    {
        selectedItem = dynamic_cast<Node *> (scene()->selectedItems().at(0));
    }
    switch (event->key()) {
    case Qt::Key_Up:
        if (selectedItem)
            selectedItem->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        if (selectedItem)
            selectedItem->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        if (selectedItem)
            selectedItem->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        if (selectedItem)
            selectedItem->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
        shuffle();
        break;
    case Qt::RightButton:
        //emit editItem();
        //QGraphicsView::keyPressEvent(event);
        break;
    default:
        break;
    }
    QGraphicsView::keyPressEvent(event);
}

#if QT_CONFIG(wheelevent)
void Graph::wheelEvent(QWheelEvent *event)
{
    scaleView(pow(double(2), -event->delta() / 240.0));
}
#endif

void Graph::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void Graph::shuffle()
{
    foreach (QGraphicsItem *item, scene()->items())
    {
        if (qgraphicsitem_cast<Node *>(item))
            item->setPos(-150 + QRandomGenerator::global()->bounded(300), -150 + QRandomGenerator::global()->bounded(300));
    }
}

void Graph::zoomIn()
{
    scaleView(qreal(1.2));
}

void Graph::zoomOut()
{
    scaleView(1 / qreal(1.2));
}

void Graph::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
}

