#include "searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Поиск"));

    searchEdit = new QLineEdit(this);
    okButton = new QPushButton(tr("OK"), this);
    cancelButton = new QPushButton(tr("Cancel"), this);

    connect(okButton, &QPushButton::clicked, this, &SearchDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &SearchDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(searchEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

QString SearchDialog::getSearchTitle() const {
    return searchEdit->text();
}
