#include "addnotedialog.h"

AddNoteDialog::AddNoteDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Добавить запись"));

    noteTitleEdit = new QLineEdit(this);
    okButton = new QPushButton(tr("OK"), this);
    cancelButton = new QPushButton(tr("Cancel"), this);

    connect(okButton, &QPushButton::clicked, this, &AddNoteDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &AddNoteDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(noteTitleEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

QString AddNoteDialog::getNoteTitle() const {
    return noteTitleEdit->text();
}
