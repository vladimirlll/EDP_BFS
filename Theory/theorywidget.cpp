#include <QtWebEngineWidgets>
#include "theorywidget.h"

TheoryWidget::TheoryWidget(QWidget *parent) : QWidget(parent), theory{QApplication::applicationDirPath() + "/Theory"}
{
    InitializeUI();
    Go();
}

TheoryWidget::~TheoryWidget()
{
    QDir tempDir = QDir(QApplication::applicationDirPath() + "/Theory/Temp");
    tempDir.removeRecursively();
}


void TheoryWidget::InitializeUI()
{
    webView = new QWebEngineView;
    connect(webView, SIGNAL(urlChanged(QUrl)), SLOT(on_urlChanged(QUrl)));
    btnToNextPage = new QPushButton("Вперед");
    btnToPrevPage = new QPushButton("Назад");
    btnToContent = new QPushButton("К содержанию");
    btnToSubsectionStart = new QPushButton("К началу подраздела");
    btnOpenDictionary = new QPushButton("Открыть словарь");
    connect(btnToNextPage, SIGNAL(clicked()), &theory, SLOT(NextPage()));
    connect(btnToPrevPage, SIGNAL(clicked()), &theory, SLOT(PrevPage()));
    connect(btnToContent, SIGNAL(clicked()), SLOT(on_btnToContent_clicked()));
    connect(btnToSubsectionStart, SIGNAL(clicked()), &theory, SLOT(ToCurrentSubsectionStart()));
    connect(btnOpenDictionary, SIGNAL(clicked()), SLOT(on_btnOpenDictionary_clicked()));
    QHBoxLayout *navBtnsLayout = new QHBoxLayout;
    navBtnsLayout->addWidget(btnToNextPage);
    navBtnsLayout->addWidget(btnToPrevPage);
    navBtnsLayout->addWidget(btnToContent);
    navBtnsLayout->addWidget(btnToSubsectionStart);
    navBtnsLayout->addWidget(btnOpenDictionary);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(webView);
    mainLayout->addLayout(navBtnsLayout);
    setLayout(mainLayout);
    connect(&theory, SIGNAL(CurentPageChanged()), SLOT(on_pageChanged()));
}

void TheoryWidget::on_btnToNextPage_clicked()
{
    theory.NextPage();
}

void TheoryWidget::on_btnToPrevPage_clicked()
{
    theory.PrevPage();
}

void TheoryWidget::on_btnToSubsectionStart_clicked()
{
    theory.ToCurrentSubsectionStart();
}

void TheoryWidget::on_btnToContent_clicked()
{
    QDir tempDir = QDir(QApplication::applicationDirPath() + "/Theory/Temp");
    if(!tempDir.exists()) tempDir.mkpath(".");

    theory.SaveContentOnDisc(tempDir.absolutePath());
    webView->setUrl(QUrl(tempDir.absolutePath() + "/content.html"));
}

void TheoryWidget::on_btnOpenDictionary_clicked()
{
    QDir tempDir = QDir(QApplication::applicationDirPath() + "/Theory/Temp");
    if(!tempDir.exists()) tempDir.mkpath(".");

    theory.SaveDictionaryOnDisc(tempDir.absolutePath());
    webView->setUrl(QUrl(tempDir.absolutePath() + "/dictionary.html"));
}

void TheoryWidget::on_urlChanged(const QUrl &url)
{

}

void TheoryWidget::on_pageChanged()
{
    Go();
}

void TheoryWidget::Go()
{
    QFile file(theory.GetCurrentPage().GetFilePath());
    qDebug() << theory.GetCurrentPage().GetFilePath();
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            webView->setUrl(QUrl(theory.GetCurrentPage().GetFilePath()));
        }
        else
        {
            file.close();
        }
    }
    else
    {
        file.close();
    }
}
