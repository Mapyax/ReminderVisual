
#include "aboutme.h"

aboutme::aboutme(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Обо мне"));

    Text = new QLabel(this);
    Text->setText(tr("Разработчик:\nИП-111 Прохоров Андрей\n\nКурсовая работа."));
    okButton = new QPushButton(tr("OK"), this);

    connect(okButton, &QPushButton::clicked, this, &aboutme::accept);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(Text);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}
