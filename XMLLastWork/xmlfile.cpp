#include "xmlfile.h"
#include <QFile>
#include <QXmlStreamWriter>
#include <windows.h>

using namespace MyXMLStruct;

XMLFile::XMLFile(QString fileName)
    :fileName(fileName){}

void XMLFile::CreateNewFile()
{
    QFile file(fileName);
    //如果文件不能正常打开
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        MessageBoxA(NULL,"cannot create xml!","ERROR",MB_ICONERROR);
        exit(1);
    }
    QDomDocument doc;
    //添加处理命令
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    //创建根节点
    QDomElement root = doc.createElement("NoteSystem");
    doc.appendChild(root);

    //写入文件中
    QTextStream out_stream(&file);
    doc.save(out_stream,4);
    file.close();
}

void XMLFile::WriteNoteSystemToXML(MyXMLStruct::NoteSystem &noteSys)
{
    //如果之前已经有了这个文件，那么就移除掉
    QFileInfo fileInfo(fileName);
    if(fileInfo.exists()){
        QFile file(fileName);
        file.remove();
    }
    //创建新的文件
    CreateNewFile();

    for(auto& i:noteSys.noteVec){
        WriteNoteToXml(i);
    }
}

void XMLFile::WriteNoteToXml(MyXMLStruct::Note &note)
{
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly)) return;

    QDomDocument doc;
    if(!doc.setContent(&file)){
        file.close();return;
    }
    file.close();

    //NoteSystem节点
    QDomElement root = doc.documentElement();
    //创建Note节点
    QDomElement elemNote = doc.createElement("Note");
    //创建Book部分
    QDomElement elemBook = doc.createElement("Book");

    QDomElement elemBookName = doc.createElement("BookName");
    QDomText bookNameText = doc.createTextNode(note.book.name);
    elemBookName.appendChild(bookNameText);

    QDomElement elemBookAuthor = doc.createElement("BookAuthor");
    QDomText bookAuthorText = doc.createTextNode(note.book.author);
    elemBookAuthor.appendChild(bookAuthorText);

    QDomElement elemBookEditor = doc.createElement("BookEditor");
    QDomText bookEditorText = doc.createTextNode(note.book.editor);
    elemBookEditor.appendChild(bookEditorText);

    QDomElement elemBookVersion = doc.createElement("BookVersion");
    QDomText bookVersionText = doc.createTextNode(note.book.version);
    elemBookVersion.appendChild(bookVersionText);

    elemBook.appendChild(elemBookName);
    elemBook.appendChild(elemBookAuthor);
    elemBook.appendChild(elemBookEditor);
    elemBook.appendChild(elemBookVersion);

    //创建LearnTime部分
    QDomElement elemLearnTime = doc.createElement("LearnTime");
    QDomText learnTimeText = doc.createTextNode(QString(note.learnTime));
    elemLearnTime.appendChild(learnTimeText);

    //创建ChapterNotes部分
    QDomElement elemChapterNotes = doc.createElement("ChapterNotes");
    //循环遍历来增加ChapterNote
    for(auto& i:note.notes.noteVec){
        QDomElement elemTempChapterNote = doc.createElement("ChapterNote");

        QDomElement elemTempChapterNameNode = doc.createElement("ChapterName");
        QDomText tempChapterNameText = doc.createTextNode(i.chapterName);
        elemTempChapterNameNode.appendChild(tempChapterNameText);

        QDomElement elemTempChNoteNode = doc.createElement("ChNote");
        QDomText tempChNoteText = doc.createTextNode(i.chNote);
        elemTempChNoteNode.appendChild(tempChNoteText);

        elemTempChapterNote.appendChild(elemTempChapterNameNode);
        elemTempChapterNote.appendChild(elemTempChNoteNode);

        elemChapterNotes.appendChild(elemTempChapterNote);
    }

    //创建MyFeeling部分
    QDomElement elemMyFeeling = doc.createElement("MyFeeling");
    QDomText myFeelingText = doc.createTextNode(note.myFeeling);
    elemMyFeeling.appendChild(myFeelingText);

    //整合到Note节点中
    elemNote.appendChild(elemBook);
    elemNote.appendChild(elemLearnTime);
    elemNote.appendChild(elemChapterNotes);
    elemNote.appendChild(elemMyFeeling);

    //向root中添加
    root.appendChild(elemNote);

    //输出到文件
    if(!file.open(QFile::WriteOnly|QFile::Truncate)) return;
    QTextStream out_stream(&file);
    doc.save(out_stream,4);
    file.close();
}

void XMLFile::ReadNoteSystemFromXML(MyXMLStruct::NoteSystem &noteSys)
{
    QFileInfo fileInfo(fileName);
    if(!fileInfo.exists()) return;

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly)) return;

    QDomDocument doc;
    if(!doc.setContent(&file)){
        file.close();return;
    }
    file.close();

    QDomElement root = doc.documentElement();

    QDomNodeList noteList = root.childNodes();
    for(int i=0;i<noteList.count();++i){
        Note tempNote;
        ReadNoteFromXML(noteList.at(i),tempNote);
        noteSys.noteVec.push_back(tempNote);
    }
}

void XMLFile::ReadNoteFromXML(const QDomNode& noteNode, Note &note)
{
    //针对book赋值
    QDomNode bookNode = noteNode.childNodes().at(0);
    note.book.name = bookNode.childNodes().at(0).toElement().text();
    note.book.author = bookNode.childNodes().at(1).toElement().text();
    note.book.editor = bookNode.childNodes().at(2).toElement().text();
    note.book.version = bookNode.childNodes().at(3).toElement().text();

    //针对learningTime赋值
    QDomNode learnTimeNode = noteNode.childNodes().at(1);
    note.learnTime = learnTimeNode.toElement().text().toInt();

    //针对notes赋初值
    QDomNode notesNode = noteNode.childNodes().at(2);
    QDomNodeList noteList = notesNode.childNodes();
    for(int i=0;i<noteList.count();++i){
        ChapterNote tempNote;
        tempNote.chapterName = noteList.at(i).childNodes().at(0).toElement().text();
        tempNote.chNote = noteList.at(i).childNodes().at(1).toElement().text();

        note.notes.noteVec.push_back(tempNote);
    }

    //针对myFeeling赋值
    QDomNode myFeelingNode = noteNode.childNodes().at(3);
    note.myFeeling = myFeelingNode.toElement().text();
}
