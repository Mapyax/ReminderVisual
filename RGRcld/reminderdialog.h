#ifndef REMINDERDIALOG_H
#define REMINDERDIALOG_H

#include <QDialog>
#include <QDateEdit>
#include <QTimeEdit>
#include <QPushButton>

class ReminderDialogg : public QDialog
{
    Q_OBJECT

public:
    ReminderDialogg(QWidget *parent = nullptr);
    QDateTime getReminderDateTime() const;

private:
    QDateEdit *dateEdit;
    QTimeEdit *timeEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // REMINDERDIALOG_H
