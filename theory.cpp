#include <QtWidgets>
#include <QtWebEngineWidgets>
#include "theory.h"

int Theory::GetCurrentSubsectionPageNum()
{
    int result = -1;
    int subsectionsCount = subsectionsPages[0].size();
    if(subsectionsCount != 0)
    {
        int i = 0;
        while(i < subsectionsCount && currentPageNum >= subsectionsPages[1][i].toInt()) ++i;
        if(i < subsectionsCount) result = subsectionsPages[1][i-1].toInt();
        else result = subsectionsPages[1][subsectionsCount-1].toInt();
    }
    return result;
}

QString Theory::FromUrlToFilePath(QUrl url)
{
    QString result = url.toString();
    result = result.right(result.size() - 8);
    return result;
}

int Theory::GetPageNumFromFilePath(QString filePath)
{
    int result;
    QString temp = filePath.right(6);
    temp = temp[0];
    bool ok;
    result = temp.toInt(&ok);
    if(!ok) result = -1;
    return result;
}

QString Theory::GetPagePathFromPageNum(int pageNum)
{
    QString result = QApplication::applicationDirPath() + "/theory_" + QString::number(pageNum) + ".html";
    return result;
}

void Theory::GetDefinitionsPages()
{
    definitionsPages.resize(2);
    currentPageNum = 0;
    filePath = GetPagePathFromPageNum(currentPageNum);
    bool allChecked = false;
    do
    {
        QFile file(filePath);
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream strm(&file);
            strm.setCodec("UTF-8");
            QString theoryText;
            QString defintion;
            while(!strm.atEnd())
            {
                theoryText = strm.readLine();
                int startIndex = theoryText.indexOf("<dfn>");
                startIndex += 5;
                int endIndex = theoryText.indexOf("</dfn>");
                if((startIndex-5) != -1)
                {
                    defintion = theoryText.mid(startIndex, endIndex - startIndex);
                    definitionsPages[0].push_back(defintion);
                    definitionsPages[1].push_back(QString::number(currentPageNum));
                }
            }
            currentPageNum++; filePath = GetPagePathFromPageNum(currentPageNum);
        }
        else
        {
            allChecked = true;
            file.close();
        }
    } while(!allChecked);
}

void Theory::GetSubsectionsPages()
{
    subsectionsPages.resize(2);
    currentPageNum = 0;
    filePath = GetPagePathFromPageNum(currentPageNum);
    bool allChecked = false;
    do
    {
        QFile file(filePath);
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream strm(&file);
            strm.setCodec("UTF-8");
            QString theoryText;
            QString subsection;
            while(!(theoryText = strm.readLine()).isEmpty())
            {
                int startIndex = theoryText.indexOf("<h1>");
                startIndex += 4;
                int endIndex = theoryText.indexOf("</h1>");
                if((startIndex-4) != -1)
                {
                    subsection = theoryText.mid(startIndex, endIndex - startIndex);
                    subsectionsPages[0].push_back(subsection);
                    subsectionsPages[1].push_back(QString::number(currentPageNum));
                }
            }
            currentPageNum++; filePath = GetPagePathFromPageNum(currentPageNum);
        }
        else
        {
            allChecked = true;
            file.close();
        }
    } while(!allChecked);
}

Theory::Theory(QWidget *parent) :
    QWidget(parent)
{
    webView = new QWebEngineView;
    connect(webView, SIGNAL(urlChanged(QUrl)), SLOT(on_urlChanged(QUrl)));
    btnToNextPage = new QPushButton("Вперед");
    btnToPrevPage = new QPushButton("Назад");
    btnToContent = new QPushButton("К содержанию");
    btnToSubsectionStart = new QPushButton("К началу подраздела");
    btnOpenDictionary = new QPushButton("Открыть словарь");
    connect(btnToNextPage, SIGNAL(clicked()), SLOT(on_btnToNextPage_clicked()));
    connect(btnToPrevPage, SIGNAL(clicked()), SLOT(on_btnToPrevPage_clicked()));
    connect(btnToContent, SIGNAL(clicked()), SLOT(on_btnToContent_clicked()));
    connect(btnToSubsectionStart, SIGNAL(clicked()), SLOT(on_btnToSubsectionStart_clicked()));
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
    GetSubsectionsPages();
    GetDefinitionsPages();
    currentPageNum = 0;
    filePath = GetPagePathFromPageNum(currentPageNum);
    Go();
}

Theory::~Theory()
{
    QFile tempFile(QApplication::applicationDirPath() + "/contentTemp.html");
    if(tempFile.exists()) tempFile.remove();
    tempFile.setFileName(QApplication::applicationDirPath() + "/dictionaryTemp.html");
    if(tempFile.exists()) tempFile.remove();
    tempFile.close();
}

void Theory::on_urlChanged(const QUrl &url)
{
    filePath = FromUrlToFilePath(url);
    currentPageNum = GetPageNumFromFilePath(filePath);
}

void Theory::on_btnToNextPage_clicked()
{
    currentPageNum++;
    filePath = GetPagePathFromPageNum(currentPageNum);
    QFile file(filePath);
    if(!file.exists())
    {
        currentPageNum--;
        filePath = GetPagePathFromPageNum(currentPageNum);
    }
    else Go();
}


void Theory::on_btnToPrevPage_clicked()
{
    currentPageNum--;
    filePath = GetPagePathFromPageNum(currentPageNum);
    QFile file(filePath);
    if(!file.exists())
    {
        currentPageNum++;
        filePath = GetPagePathFromPageNum(currentPageNum);
    }
    else Go();
}


void Theory::on_btnToSubsectionStart_clicked()
{
    currentPageNum = GetCurrentSubsectionPageNum();
    filePath = GetPagePathFromPageNum(currentPageNum);
    Go();
}


void Theory::on_btnToContent_clicked()
{
    QString content = "<link rel=\"stylesheet\" href=\"main.css\"";
    int subsectionsCount = subsectionsPages[0].size();
    for(int i = 0; i < subsectionsCount; ++i)
    {
        QString pageName = GetPagePathFromPageNum(subsectionsPages[1][i].toInt());
        content += "<p><a href = \"" + pageName + "\">" + QString::number(i+1) + ". " + subsectionsPages[0][i] + "</a></p> ";
    }
    QFile tempFile(QApplication::applicationDirPath() + "/contentTemp.html");
    if(tempFile.open(QIODevice::WriteOnly))
    {
        QTextStream strm(&tempFile);
        strm.setCodec("UTF-8");
        strm << content;
    }
    tempFile.close();
    webView->setUrl(QUrl(QApplication::applicationDirPath() + "/contentTemp.html"));
    currentPageNum = -1;
}

void Theory::on_btnOpenDictionary_clicked()
{
    QString content = "<link rel=\"stylesheet\" href=\"main.css\"";
    int definitionsCount = definitionsPages[0].size();
    for(int i = 0; i < definitionsCount; ++i)
    {
        QString pageName = GetPagePathFromPageNum(definitionsPages[1][i].toInt());
        content += "<p><a href = \"" + pageName + "\">" + QString::number(i+1) + ". " + definitionsPages[0][i] + "</a></p> ";
    }
    QFile tempFile(QApplication::applicationDirPath() + "/dictionaryTemp.html");
    if(tempFile.open(QIODevice::WriteOnly))
    {
        QTextStream strm(&tempFile);
        strm.setCodec("UTF-8");
        strm << content;
    }
    tempFile.close();
    webView->setUrl(QUrl(QApplication::applicationDirPath() + "/dictionaryTemp.html"));
    currentPageNum = -1;
}

void Theory::Go()
{
    QFile file(filePath);
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly))
        {
            webView->setUrl(QUrl(filePath));
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

