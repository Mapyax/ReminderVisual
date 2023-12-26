#ifndef NOTEEDITOR_H
#define NOTEEDITOR_H

#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QTextStream>

class NoteEditor : public QMainWindow {
    Q_OBJECT
private:
    QString      currentFilename;
    QTextEdit   *noteTextEdit;
    QPushButton *saveButton;

private slots:
    void onSave();

public:
    NoteEditor(const QString &filename, QWidget *parent = nullptr);


};

#endif // NOTEEDITOR_H
