#include "noteeditor.h"
#include <QDebug>

NoteEditor::NoteEditor(const QString &filename, QWidget *parent): QMainWindow(parent), currentFilename(filename)
{
    setWindowTitle(tr("Редактор"));

    noteTextEdit = new QTextEdit(this);
    saveButton = new QPushButton(tr("&Сохранить"), this);

    QFile file(currentFilename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        // Установка кодировки для QTextStream (в данном случае, UTF-8)
        in.setCodec("CRLF");
        in.setGenerateByteOrderMark(true);
        QString content = in.readAll();

        noteTextEdit->setPlainText(content);

        file.close();
    }

    connect(saveButton, &QPushButton::clicked, this, &NoteEditor::onSave);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(noteTextEdit);
    mainLayout->addWidget(saveButton);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void NoteEditor::onSave() {
    qDebug() << "Save button clicked!";

    QFile file(currentFilename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        //out.setCodec("UTF-8");

        out << noteTextEdit->toPlainText();
        file.close();

        qDebug() << "saved!";
        close();
    } else {
        qDebug() << "Error saving note!";
    }
}
