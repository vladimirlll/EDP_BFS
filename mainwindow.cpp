#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ptheory = new TheoryWidget();
    pdemo = new Demo();
    ptesting = new Testing();
    ui->tabWidget->addTab(ptheory, "Теория");
    ui->tabWidget->addTab(pdemo, "Демонстрация");
    ui->tabWidget->addTab(ptesting, "Тестирование");
    setWindowTitle("Курсовой проект на тему \"Алгоритм обхода графа в ширину\". Автор: Лиджигоряев Владимир, ДИПРБ-21");
}

MainWindow::~MainWindow()
{
    delete ui;
}

