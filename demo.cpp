#include "demo.h"
#include "ui_demo.h"
#include <QDebug>
#include <QQueue>

Demo::Demo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Demo)
{
    ui->setupUi(this);
    connect(ui->graph->scene(), &QGraphicsScene::selectionChanged, this, &Demo::sceneSelectionChanged);
}

Demo::~Demo()
{
    ui->graph->scene()->clearSelection();
    nodes.clear();
    edges.clear();
    delete ui;
}

void Demo::closeEvent(QCloseEvent */*event*/)
{
    // Важно! disconnect нужен для корректного выхода из приложения!
    disconnect(ui->graph->scene(), nullptr, nullptr, nullptr);
}

void Demo::on_btnAddVert_clicked()
{
    ui->btnBSF->setEnabled(false);
    ui->btnAddEdge->setChecked(false);

    int x, y;                   //координаты вершины на сцене
    int numNode = 0;
    bool flFinding = false;     //флаг нахождения, при решение с каким состоянием создавать вершину
    Node *node;
    node = new Node();
    numNode = node->GetId();
    // Определяет сколько вершин будут появлятся на одной оси У
    int nodeInRow = 6;
    x = - 2 * (2 * Node::Radius + 10) +
            ((!flFinding ? numNode : nodes.size()) % nodeInRow)
            * (2 * Node::Radius + 10);
    y = -100 + 2 * Node::Radius + 10 +
            ((!flFinding ? numNode : nodes.size()) / nodeInRow)
            * (2 * Node::Radius + 10);
    nodes.append(node);
    node->setPos(x, y);
    _source = nullptr;
    connFlag = 0;
    ui->lblAnnotation->setText("Вершина добавлена");
    if (nodes.size()==9){
        ui->btnAddEdge->setEnabled(false);
    }
    ui->graph->scene()->addItem(node);
}

void Demo::on_btnAddEdge_clicked()
{
    ui->btnBSF->setEnabled(false);
    if (ui->graph->scene()->selectedItems().size() > 0) {
        _source = dynamic_cast<Node *>(ui->graph->scene()->selectedItems().at(0));
        if (_source) {
            ui->graph->scene()->clearSelection();
            ui->lblAnnotation->setText("Нажмите на вершину, к которой будет проведена дуга.");
            connFlag = 2;
        } else {
            ui->lblAnnotation->clear();
            connFlag = 0;
        }
    }
    /* if (!_source) {
        if (connFlag == 0) {    // это условие не обязательное
            lTip->setText("Выберите вершину источник, потом получатель дуги");
            connFlag = 1;
            grafViewScene->scene()->clearSelection();
        }

    }*/
}

void Demo::on_btnDeleteVert_clicked()
{
    ui->btnBSF->setEnabled(false);
    if (ui->graph->scene()->selectedItems().size()) {
        _source = nullptr;
        connFlag = 0;
        auto i = ui->graph->scene()->selectedItems().at(0);
        if (Node* n = dynamic_cast<Node*>(i)) {
            foreach (auto e, n->edges()) {
                edges.removeAll(e);
            }
            if (n) {
                nodes.removeAll(n);
            } else {
                qDebug() << "dynamic_cast returned 0";
            }
            ui->btnAddVert->setEnabled(true);
            ui->lblAnnotation->setText("Вершина удалена");
            states.clear();
        } else if (Edge *e = dynamic_cast<Edge*>(i)) {
            if (e) {
                edges.removeAll(e);
            } else {
                qDebug() << "dynamic_cast returned 0";
            }
            ui->lblAnnotation->setText("Дуга удалена.");

        } else {
            qDebug() << tr("I don't know what it is. type == %1").arg(i->type());
        }
        ui->graph->scene()->removeItem(i);
        delete i;
    }
}

void Demo::sceneSelectionChanged()
{
    ui->btnBSF->setEnabled(false);
    QList<QGraphicsItem *> l = ui->graph->scene()->selectedItems();
    if (l.size() == 1) {
        ui->lblAnnotation->setText("Выделена вершина");
        if (Node *node = dynamic_cast<Node *>(l.at(0)))
        {
            // Выделена вершина
            ui->btnBSF->setEnabled(true);
            ui->btnAddEdge->setEnabled(true);
            if (connFlag == 1)
            {
                // Назначен "Источник"
                ui->btnBSF->setEnabled(false);
                _source = node;
                connFlag = 2;
                ui->lblAnnotation->setText("Нажмите на вершину, к которой будет проведена дуга.");
            }
            else if (connFlag == 2)
            {
                //Выделена новая вершина, с которой нужно соединить текущую вершину
                Edge *e;
                if (_source == node) {
                    ui->lblAnnotation->setText("Соединение вершины с самой собой недоступно");
                    //e = new EdgeCircle(_source);
                } else {
                    e = new Edge(_source, node);
                    ui->graph->scene()->addItem(e);
                    edges.append(e);
                    ui->btnAddEdge->setChecked(false);

                    connFlag = 0;
                    _source = nullptr;
                    ui->btnBSF->setEnabled(true);
                }
            }
            else if (connFlag == 3)
            {
                BSF();      //начинаем обход с только что выделенной вершины
            }
        }
        else
        {
            // Выделена стрелка
            ui->lblAnnotation->setText("Выделена дуга.");
            ui->btnAddEdge->setEnabled(false);
            connFlag = false;
            _source = nullptr;
        }
        ui->btnDeleteVert->setEnabled(true);

    } else if (l.size() > 1) {
        // Всегда должено быть выделено не более 1ого элемента
        qDebug() << "graph->scene()->selectedItems().size() == " << l.size();
    } else {
        // Пропало выделение (после удаления или нажатия на "Добавить вершину")
        ui->lblAnnotation->setText("");
        ui->btnDeleteVert->setEnabled(false);
        ui->btnAddEdge->setEnabled(false);
        ui->btnBSF->setEnabled(true);
    }
}

void Demo::on_btnBSF_clicked()
{
    ui->lblAnnotation->setText("Нажмите на вершину, с которой будет начинаться обход");
    connFlag = 3;   //Установка флага, чтобы выделение вершины пользователем интерпретировалось
                    //как выделение начальной вершины для алгоритма обхода графа в ширину
}

void Demo::BSF()
{
    int nodesCount = Node::GetidStatic();
    QVector<int> x(nodesCount, 0);
    QList<QGraphicsItem *> l = ui->graph->scene()->selectedItems();
    if (l.size() == 1)
    {
        if (Node *node = dynamic_cast<Node *>(l.at(0)))
        {
            BSFState state;
            state.nodesColors.fill(Qt::white, nodesCount);
            state.nodesTextColors.fill(Qt::black, nodesCount);
            states.push_back(state);
            //Выделение начальной вершина, как рассматриваемой (серый цвет)
            //node->SetNodeColor(Qt::gray, false);
            state.nodesColors[node->GetId()] = Qt::gray; state.stateText = "Выбрана начальная вершина";
            states.push_back(state);
            //Выделение начальной вершина, как пройденной
            QQueue<Node*> nodesQueue;
            nodesQueue.enqueue(node);
            x[node->GetId()] = 1;
            do
            {
                //Выделение
                Node* checkedNode = nodesQueue.dequeue();
                /*
                checkedNode->SetNodeColor(Qt::black, false);
                checkedNode->SetPenColor(Qt::white, false);
                state.nodes = nodes;*/
                state.nodesColors[checkedNode->GetId()] = Qt::black;
                state.nodesTextColors[checkedNode->GetId()] = Qt::white;
                state.stateText = "Вершина " + QString::number(checkedNode->GetId() + 1) + " пройдена\n";
                states.push_back(state);
                state.stateText = "";
                foreach(Edge *edge, checkedNode->edges())
                {
                    Node* checkingNode;
                    if(edge->GetStartNode() == checkedNode)
                    {
                        //Значит смежная вершина в endNode
                        checkingNode = edge->GetEndNode();
                    }
                    else
                    {
                        //Значит смежная вершина в startNode
                        checkingNode = edge->GetStartNode();
                    }
                    if(x[checkingNode->GetId()] == 0)
                    {
                        nodesQueue.enqueue(checkingNode);
                        x[checkingNode->GetId()] = 1;
                        /*
                        checkingNode->SetNodeColor(Qt::gray, false);
                        checkingNode->SetPenColor(Qt::white, false);
                        state.nodes = nodes;*/
                        state.nodesColors[checkingNode->GetId()] = Qt::gray;
                        state.nodesTextColors[checkingNode->GetId()] = Qt::white;
                        state.stateText += "Вершина " + QString::number(checkingNode->GetId() + 1) + " рассматривается\n";
                    }
                }
                states.push_back(state);
                state.stateText = "";
            } while(!nodesQueue.isEmpty());
        }
    }
    ui->btnNextStep->setEnabled(true);
    ui->btnPrevStep->setEnabled(true);
    currentState = 0;
    connFlag = 0;   //Установка флага в неактивное состояние
}

void Demo::loadState(int stateNum)
{
    int i = 0;
    foreach(Node* node, nodes)
    {
        node->SetNodeColor(states[stateNum].nodesColors[i]);
        node->SetPenColor(states[stateNum].nodesTextColors[i]);
        ui->tedAnnotation->setText(states[stateNum].stateText);
        i++;
    }
}

void Demo::on_btnNextStep_clicked()
{
    int statesCount = states.size();
    if((currentState + 1) < statesCount)
    {
        loadState(++currentState);
    }
}

void Demo::on_btnPrevStep_clicked()
{
    if((currentState - 1) >= 0)
    {
        loadState(--currentState);
    }
}

void Demo::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Delete:
        on_btnDeleteVert_clicked();
        break;
    case Qt::Key_N:
    case 1058:
        on_btnAddVert_clicked();
        break;
    case Qt::Key_C:
    case 1057:
        on_btnAddEdge_clicked();
        break;
    default:
        break;
    }
    QWidget::keyPressEvent(event);
}
