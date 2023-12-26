#ifndef ADDNOTEDIALOG_H
#define ADDNOTEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class AddNoteDialog : public QDialog {
    Q_OBJECT

public:
    AddNoteDialog(QWidget *parent = nullptr);

    QString getNoteTitle() const;

private:
    QLineEdit *noteTitleEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // ADDNOTEDIALOG_H
