#ifndef THEORY_H
#define THEORY_H

#include "theorypage.h"

namespace TheoryCore
{
    class Theory : public QObject
    {
        Q_OBJECT
    public:
        Theory(const QString &filesDirPath);
        void LoadPages(const QString &filesDirPath);                    // - Загружает страницы теории
        void RemovePage(const TheoryCore::TheoryPage &page);            // - Удаляет страницу теории из коллекции страниц
        void RemoveAllPages();                                          // - Удаляет все страницы теории из коллекции страниц
        TheoryCore::TheoryPage GetCurrentPage();                        // - Возвращает текущую страницу
        QVector<TheoryPage> GetPages();                                 // - Возвращает все страницы теории
        uint GetCurrentPageNum();                                       // - Возвращает номер
        void SaveContentOnDisc(const QString &dirPath);                 // - Сохраняет содержание теории в папке dirPath
        void SaveDictionaryOnDisc(const QString &dirPath);              // - Сохраняет содержание теории в папке dirPath

    signals:
        void CurentPageChanged();

    public slots:
        void NextPage();                                                // - Переход на следующую страницу
        void PrevPage();                                                // - Переход на предыдущую страницу
        void ToCurrentSubsectionStart();                                // - Переход к началу текущего подраздела

    private:
        QVector<TheoryCore::TheoryPage> pages;                          // - Коллекция страниц
        TheoryCore::TheoryPage          currentPage;                    // - Текущая страница
        uint                            currentPageNum;                 // - Номер страницы в векторе страниц
    };
}

#endif // THEORY_H
