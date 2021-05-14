#ifndef XMLFILE_H
#define XMLFILE_H

#include <QtXml>
#include <myxmlstruct.h>

class XMLFile
{
private:
    const QString fileName;
public:
    XMLFile(QString fileName = "NoteBook.xml");

    void CreateNewFile();
    void WriteNoteSystemToXML(MyXMLStruct::NoteSystem& noteSys);
    void WriteNoteToXml(MyXMLStruct::Note& note);
    void ReadNoteSystemFromXML(MyXMLStruct::NoteSystem& noteSys);
    void ReadNoteFromXML(const QDomNode& noteNode,MyXMLStruct::Note& note);
};

#endif // XMLFILE_H
