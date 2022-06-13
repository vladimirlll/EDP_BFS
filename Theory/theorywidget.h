#ifndef THEORYWIDGET_H
#define THEORYWIDGET_H

#include <QWidget>
#include <QFile>
#include <QDebug>
#include "theory.h"

class QWebEngineView;
class QPushButton;

class TheoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TheoryWidget(QWidget *parent = nullptr);
    ~TheoryWidget();

private slots:
    void on_btnToNextPage_clicked();
    void on_btnToPrevPage_clicked();
    void on_btnToSubsectionStart_clicked();
    void on_btnToContent_clicked();
    void on_btnOpenDictionary_clicked();
    void on_urlChanged(const QUrl &url);
    void on_pageChanged();

private:
    QPushButton* btnToNextPage;
    QPushButton* btnToPrevPage;
    QPushButton* btnToContent;
    QPushButton* btnToSubsectionStart;
    QPushButton* btnOpenDictionary;
    QWebEngineView* webView;
    TheoryCore::Theory theory;

    void Go();
    void InitializeUI();
};

#endif // THEORYWIDGET_H
