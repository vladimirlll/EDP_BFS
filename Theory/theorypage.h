#ifndef THEORYPAGE_H
#define THEORYPAGE_H

#include <QObject>
#include <QFile>
#include <qvector.h>

namespace TheoryCore
{
    class TheoryPage
    {
    public:
        TheoryPage();
        TheoryPage(const QString filePath);
        void SetFilePath(const QString filePath);                                   // - Устанавливает путь к файлу со страницей теории
        QString GetFilePath()           const;                                      // - Возвращает путь к файлу со страницей теории
        QVector<QString> Definitions()  const;                                      // - Возвращает определения, обраруженные на странице
        QVector<QString> Subsections()  const;                                      // - Возвращает подзаголовки, обнаруженные на странице

        bool operator==(const TheoryPage &other);

    private:
        QString filePath;

        QVector<QString> AllTagsInFile(const QString tagName) const;               // - Возвращает содержимое всех тегов с именем tagName в файле filePath
    };
}

#endif // THEORYPAGE_H
