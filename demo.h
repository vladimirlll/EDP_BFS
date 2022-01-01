#ifndef DEMO_H
#define DEMO_H

#include <QWidget>
#include "graph.h"

namespace Ui {
class Demo;
}

class Demo : public QWidget
{
    Q_OBJECT

public:
    explicit Demo(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event) override;
    ~Demo();

private:
    struct BSFState
    {
        QVector<QColor> nodesColors;
        QVector<QColor> nodesTextColors;
        QString stateText;
    };

    Ui::Demo *ui;
    QList <Node*> nodes;                //вершины графа
    QList <Edge*> edges;                //ребра графа
    QVector<BSFState> states;           //состояния вершин при прохождении алгоритма обхода для каждого шага
    int currentState;                   //текущее состояние алгоритма, показываемое на экране (индекс nodesStates)
    Node *_source;                      // Начальная вершина при соединении двух вершин

    // Флаг состояния, обозначает что сейчас нужно делать
    // connFlag == 0: неактивное состояние (ничего не происходит, никакие вершины не выделены, ничего делать не требуется)
    // connFlag == 1: нужно выбирать начальную вершину для соединения
    // connFlag == 2: нужно выбрать конечную вершину для соединения
    // connFlag == 3: нужно выбрать начальную точку для алгоритма обхода
    int connFlag;
    void BSF();
    void loadState(int stateNum);

public slots:
    void on_btnAddVert_clicked();
    void on_btnAddEdge_clicked();
    void on_btnDeleteVert_clicked();
    void on_btnBSF_clicked();
    void on_btnNextStep_clicked();
    void on_btnPrevStep_clicked();
    void sceneSelectionChanged();
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // DEMO_H
