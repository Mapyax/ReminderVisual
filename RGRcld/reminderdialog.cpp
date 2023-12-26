#include "reminderdialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QDateTime>

ReminderDialogg::ReminderDialogg(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("Установить время"));

    QTime cTime = QTime::currentTime();
    cTime = cTime.addSecs(-cTime.second());
    dateEdit = new QDateEdit(QDate::currentDate(), this);
    timeEdit = new QTimeEdit(cTime, this);
    okButton = new QPushButton(tr("Ок"), this);
    cancelButton = new QPushButton(tr("Отмена"), this);

    connect(okButton, &QPushButton::clicked, this, &ReminderDialogg::accept);
    connect(cancelButton, &QPushButton::clicked, this, &ReminderDialogg::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(dateEdit);
    mainLayout->addWidget(timeEdit);
    mainLayout->addWidget(okButton);
    mainLayout->addWidget(cancelButton);
}

QDateTime ReminderDialogg::getReminderDateTime() const {
    QDateTime dateTime(dateEdit->date(), timeEdit->time());
    return dateTime;
}

