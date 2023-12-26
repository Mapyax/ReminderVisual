// aboutme.h

#ifndef ABOUTME_H
#define ABOUTME_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class aboutme : public QDialog {
    Q_OBJECT

public:
    aboutme(QWidget *parent = nullptr); // Добавьте параметр parent

private:
    QLabel *Text;
    QPushButton *okButton;
};

#endif // ABOUTME_H
