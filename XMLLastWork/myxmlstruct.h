#ifndef MYXMLSTRUCT_H
#define MYXMLSTRUCT_H

#include <QString>
#include <QVector>
#include <QtXml>

namespace MyXMLStruct {
    //基类
    class XMLStruct{
    public:
        virtual ~XMLStruct() = default;
        virtual void InitSelf(QDomElement& e) = 0;
        virtual void WriteSelf() = 0;
    };
    //书本类
    class Book:public XMLStruct{
    public:
        QString name;
        QString author;
        QString editor;
        QString version;
        virtual void InitSelf(QDomElement& e);
        virtual void WriteSelf();
        Book(QString bookName="新建图书");
    };
    //章节笔记类
    class ChapterNote:public XMLStruct{
    public:
        QString chapterName;
        QString chNote;
        virtual void InitSelf(QDomElement& e);
        virtual void WriteSelf();
        ChapterNote(QString chapterName="新建章节");
    };
    //章节笔记数组类
    class ChapterNotes:public XMLStruct{
    public:
        QVector<ChapterNote> noteVec;
        virtual void InitSelf(QDomElement& e);
        virtual void WriteSelf();
    };
    //笔记类
    class Note:public XMLStruct{
    public:
        Book book;
        int learnTime;
        ChapterNotes notes;
        QString myFeeling;
        virtual void InitSelf(QDomElement& e);
        virtual void WriteSelf();
        Note(QString bookName="新建图书");
    };
    //笔记系统类
    class NoteSystem:public XMLStruct{
    public:
        QVector<Note> noteVec;
        virtual void InitSelf(QDomElement& e);
        virtual void WriteSelf();
    };
}


#endif // MYXMLSTRUCT_H
