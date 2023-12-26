#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class SearchDialog : public QDialog {
    Q_OBJECT

public:
    SearchDialog(QWidget *parent = nullptr);

    QString getSearchTitle() const;

private:
    QLineEdit *searchEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // SEARCHDIALOG_H
