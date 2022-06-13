#include <QTextStream>
#include "Exceptions/filehandleexception.h"
#include "theorypage.h"

TheoryCore::TheoryPage::TheoryPage()
{
    SetFilePath("");
}

TheoryCore::TheoryPage::TheoryPage(const QString filePath)
{
    SetFilePath(filePath);
}

void TheoryCore::TheoryPage::SetFilePath(const QString filePath)
{
    this->filePath = filePath;
}

QString TheoryCore::TheoryPage::GetFilePath() const
{
    return filePath;
}

QVector<QString> TheoryCore::TheoryPage::AllTagsInFile(const QString tagName) const
{
    QVector<QString> result;
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream strm(&file);
        strm.setCodec("UTF-8");
        QString textLine;
        QString innerHTML;
        QString frontTag = "<" + tagName + ">";
        QString backTag = "</" + tagName + ">";
        while(!strm.atEnd())
        {
            textLine = strm.readLine();
            int startIndex = textLine.indexOf(frontTag);
            startIndex += frontTag.length();
            int endIndex = textLine.indexOf(backTag);
            if((startIndex - frontTag.length()) != -1)
            {
                innerHTML = textLine.mid(startIndex, endIndex - startIndex);
                result.push_back(innerHTML);
            }
        }
    }
    else
    {
        file.close();
        std::string exText = "Theory file " + filePath.toStdString() + " wasn't opened";
        throw Exceptions::FileHandleException(exText);
    }
    return result;
}

QVector<QString> TheoryCore::TheoryPage::Definitions() const
{
    QVector<QString> result = AllTagsInFile("dfn");
    return result;
}


QVector<QString> TheoryCore::TheoryPage::Subsections() const
{
    QVector<QString> result = AllTagsInFile("h1");
    return result;
}

bool TheoryCore::TheoryPage::operator==(const TheoryPage &other)
{
    return (filePath == other.GetFilePath());
}
