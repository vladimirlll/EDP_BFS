#include <QDir>
#include <QException>
#include <QTextStream>
#include "theory.h"

TheoryCore::Theory::Theory(const QString &filesDirPath) : QObject(nullptr)
{
    pages = QVector<TheoryPage>(0);
    LoadPages(filesDirPath);
    if(!pages.isEmpty()) currentPage = TheoryPage(pages[0].GetFilePath());
    else currentPage = TheoryPage();
    currentPageNum = 0;
}

void TheoryCore::Theory::LoadPages(const QString &filesDirPath)
{
    QDir dir = QDir(filesDirPath);
    QStringList filesInDir = dir.entryList({"*.html"});
    uint filesCount = filesInDir.length();
    for(uint i = 0; i < filesCount; ++i)
    {
        TheoryPage page = TheoryPage(filesDirPath + "/" + filesInDir[i]);
        pages.push_back(page);
    }
}

void TheoryCore::Theory::RemovePage(const TheoryPage &page)
{
    uint i = 0;
    uint pagesCount = pages.length();
    QString pageFilePath = page.GetFilePath();
    while (i < pagesCount && pageFilePath != pages[i].GetFilePath()) ++i;


    if(i < pagesCount)
    {
        pages.remove(i);
        PrevPage();
    }
}

void TheoryCore::Theory::RemoveAllPages()
{
    pages.clear();
    emit CurentPageChanged();
}

TheoryCore::TheoryPage TheoryCore::Theory::GetCurrentPage() { return currentPage; }

void TheoryCore::Theory::NextPage()
{
    ++currentPageNum;
    if(currentPageNum >= 0 && currentPageNum < pages.length())
    {
        currentPage = pages[currentPageNum];
        emit CurentPageChanged();
    }
    else
        --currentPageNum;
}

void TheoryCore::Theory::PrevPage()
{
    --currentPageNum;
    if(currentPageNum >= 0 && currentPageNum < pages.length())
    {
        currentPage = pages[currentPageNum];
        emit CurentPageChanged();
    }
    else
        ++currentPageNum;
}

/*void TheoryCore::Theory::CurentPageChanged() {}
void TheoryCore::Theory::PagesChanged() {}*/

void TheoryCore::Theory::SaveContentOnDisc(const QString& dirPath)
{
    QString content = "<link rel=\"stylesheet\" href=\"main.css\">";
    uint pagesCount = pages.length();

    for(uint i = 0; i < pagesCount; ++i)
    {
        QVector<QString> subsectionsInPage = pages[i].Subsections();
        uint subsectionsInPageCount = subsectionsInPage.length();
        for(uint j = 0; j < subsectionsInPageCount; ++j)
        {
            content += "<p><a href = \"" + pages[i].GetFilePath() + "\">" + "стр. " + QString::number(i+1) + " --- " + subsectionsInPage[j] + "</a></p> ";
        }
    }

    QFile tempFile(dirPath + "/content.html");
    if(tempFile.open(QIODevice::WriteOnly))
    {
        QTextStream strm(&tempFile);
        strm.setCodec("UTF-8");
        strm << content;
    }
    tempFile.close();
}

void TheoryCore::Theory::SaveDictionaryOnDisc(const QString &dirPath)
{
    QString content = "<link rel=\"stylesheet\" href=\"main.css\">";
    uint pagesCount = pages.length();

    for(uint i = 0; i < pagesCount; ++i)
    {
        QVector<QString> definitionsInPage = pages[i].Definitions();
        uint definitionsInPageCount = definitionsInPage.length();
        for(uint j = 0; j < definitionsInPageCount; ++j)
        {
            content += "<p><a href = \"" + pages[i].GetFilePath() + "\">" + QString::number(i+1) + ". " + definitionsInPage[j] + "</a></p> ";
        }
    }

    QFile tempFile(dirPath + "/dictionary.html");
    if(tempFile.open(QIODevice::WriteOnly))
    {
        QTextStream strm(&tempFile);
        strm.setCodec("UTF-8");
        strm << content;
    }
    tempFile.close();
}

QVector<TheoryCore::TheoryPage> TheoryCore::Theory::GetPages()
{
    return pages;
}

void TheoryCore::Theory::ToCurrentSubsectionStart()
{
    uint i = currentPageNum;
    while(i >= 0 && pages[i].Subsections().isEmpty())
    {
        --i;
    }
    if(i >= 0)
    {
        currentPage = pages[i];
        currentPageNum = i;
        emit CurentPageChanged();
    }
}
