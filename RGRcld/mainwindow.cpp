#include "mainwindow.h"
#include "reminderdialog.h"
#include "notemanager.h"
#include "noteeditor.h"
#include "addnotedialog.h"
#include "searchdialog.h"
#include <QMenu>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMenuBar>
#include <QApplication>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QDateTime>
#include <QCoreApplication>
#include <QDialog>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include "aboutme.h"
#include <QCloseEvent>
#include <QSettings>
#include <QStyleFactory>
#include <QTableWidgetItem>
#include <QtMultimedia>
#include <QtWidgets>

void MainWindow::closeEvent(QCloseEvent *event) {
    QByteArray geometry = saveGeometry();
    QSettings settings("YourOrganizationName", "YourAppName");
    settings.setValue("MainWindow/Geometry", geometry);

       QMainWindow::closeEvent(event);
}

void MainWindow::readSettings() {
    QSettings settings("YourOrganizationName", "YourAppName");

    QByteArray geometry = settings.value("MainWindow/Geometry").toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
}



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
popUp = new PopUp();
setWindowTitle(tr("Заметки"));
QPalette palette = QToolTip::palette();
palette.setColor(QPalette::ToolTipBase, Qt::white);
palette.setColor(QPalette::ToolTipText, Qt::blue);
QToolTip::setPalette(palette);

database = QSqlDatabase::addDatabase("QSQLITE");
database.setDatabaseName("F:/Reminder/RGRcld/forbd/my.db");
if (!database.open()) {
qDebug() << "[FROM db] - Error opening database:" << database.lastError().text();
} else {
qDebug() << "[FROM db] - Database opened successfully!";
}
QSqlQuery query;
if (!query.exec("CREATE TABLE IF NOT EXISTS notes (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, filename TEXT, datecreate DATETIME, reminderTime DATETIME)")) {
    qDebug() << "Error creating table:" << query.lastError().text();
}

contextMenu = new QMenu(this);
notesList = new QListWidget(this);
setCentralWidget(notesList);

    createMenu();
    notesList->setContextMenuPolicy(Qt::CustomContextMenu);

notesList->setSpacing(2);
}


void MainWindow::onNoteSelected(QListWidgetItem *item) {
    if (!(QGuiApplication::mouseButtons() && Qt::RightButton)) {
        QString filename = item->data(Qt::UserRole).toString();
        NoteEditor *editor = new NoteEditor(filename, this);
        editor->show();
    }
        contextMenu->setProperty("selectedNoteTitle", item->text());
        contextMenu->setProperty("selectedNoteFilename", item->data(Qt::UserRole).toString());
}

void MainWindow::onDeleteNote() {
QListWidgetItem *currentItem = notesList->currentItem();
if (currentItem) {
QString filename = currentItem->data(Qt::UserRole).toString();
noteManager.deleteNoteFromDatabase(filename);
delete currentItem;
}
}

void MainWindow::onOpenNote() {
QListWidgetItem *currentItem = notesList->currentItem();
if (currentItem) {
QString filename = currentItem->data(Qt::UserRole).toString();
NoteEditor *editor = new NoteEditor(filename, this);
editor->show();

}

}

void MainWindow::onSetReminder() {
    QListWidgetItem *selectedItem = notesList->currentItem();

    if (!selectedItem) {
        qDebug() << "Error: Selected note not found in the list.";
        return;
    }

    QString selectedNoteTitle = selectedItem->text();
    QString selectedNoteContent = readNoteContentFromFile(selectedItem->data(Qt::UserRole).toString());
    qDebug() << selectedNoteTitle << selectedNoteContent;

    if (!selectedNoteTitle.isEmpty()) {
        ReminderDialogg dialog(this);
        dialog.resize(250, 70);
        if (dialog.exec() == QDialog::Accepted) {
            QDateTime reminderDateTime = dialog.getReminderDateTime();

            noteManager.setNoteReminderTime(selectedItem, reminderDateTime);

            selectedItem->setText(selectedNoteTitle);

            if (selectedNoteContent.isEmpty()) {
                scheduleReminder(selectedNoteTitle, "\nбез текста.", reminderDateTime);

            } else {
                scheduleReminder(selectedNoteTitle, selectedNoteContent, reminderDateTime);

            }

        }
    } else {
        qDebug() << "[FROM FILE] - Empty filename or content specified.";
    }
}

void MainWindow::onAddNote() {
    AddNoteDialog dialog(this);
    dialog.resize(300, 70);
    dialog.show();

    if (dialog.exec() == QDialog::Accepted) {
        QString title = dialog.getNoteTitle();
        if (title.isEmpty() || (title.length() >= 50)){
            qDebug() << "Ошибка пустого ввода";
            return;
        } else {
            QString filename = "F:/Reminder/" + title + ".txt"; // Путь к файлу заметки

            QFile file(filename);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                file.close();
            } else {
                qDebug() << "[FROM FILE] - Error opening file for writing:" << file.errorString();
                return;
            }
            QDateTime datecreate = QDateTime::currentDateTime();
            QDateTime reminderTime = QDateTime(); // Здесь нужно установить время уведомления, если оно доступно
            QString noteInfo = QString("Дата создания:\n %1\nВремя уведомления:\n %2")
                                   .arg(datecreate.toString())
                                   .arg(reminderTime.isValid() ? reminderTime.toString() : "Не установленно");


               QListWidgetItem *newItem = new QListWidgetItem(title);
               newItem->setData(Qt::UserRole, filename);
               newItem->setData(Qt::UserRole + 1, datecreate);
               newItem->setData(Qt::UserRole + 2, reminderTime);
               noteManager.addNoteToDatabase(title, filename, datecreate, reminderTime);
               newItem->setToolTip(noteInfo);
               notesList->addItem(newItem);

           }
    }
}





void MainWindow::scheduleReminder(const QString &title, const QString &content, const QDateTime &reminderDateTime) {
    qDebug() << "Schedule Reminder for" << title << "at" << reminderDateTime;
    QListWidgetItem *currentItemS = notesList->currentItem();

    QTimer *timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [=]() {

        AlarmSound = new QMediaPlayer();
        AlarmSound->setMedia(QUrl("F:/Reminder/RGRcld/alarm.mp3"));
        AlarmSound->play();
        popUp->setPopupText("Напоминалка:\n\n" + title + "\n" + content);
        popUp->show();
        timer->stop();
        QDateTime createdTime = currentItemS->data(Qt::UserRole + 1).toDateTime();

        QString noteInfoend = QString("Дата создания:\n %1\nВремя уведомления:\n %2")
                               .arg(createdTime.toString())
                               .arg(reminderDateTime.isValid() ? reminderDateTime.toString() : "Не установленно");
        timer->deleteLater();
        currentItemS->setToolTip(noteInfoend);
    });



    QDateTime currentDateTime = QDateTime::currentDateTime();
    int delay = currentDateTime.msecsTo(reminderDateTime);
    QDateTime createdTime = currentItemS->data(Qt::UserRole + 1).toDateTime();

    QString noteInfostart = QString("Дата создания1:\n %1\nВремя уведомления:\n %2")
                           .arg(createdTime.toString())
                           .arg(reminderDateTime.toString());
    currentItemS->setToolTip(noteInfostart);

    timer->start(delay);
}


void MainWindow::AboutMe()
{
    aboutme aboutMeDialog;
    aboutMeDialog.resize(200, 70);
    aboutMeDialog.exec();
}


QString MainWindow::readNoteContentFromFile(const QString &filename) {
QString content;
    if (!filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            content = in.readAll();
            file.close();
        } else {
            qDebug() << "Error opening file for reading:" << file.errorString();
        }
   } else {
        qDebug() << "Empty filename specified.";
    }
    return content;
}

void MainWindow::onSearchDialog() {
        SearchDialog dialog(this);
        dialog.resize(300, 70);
        dialog.show();

        if (dialog.exec() == QDialog::Accepted) {
            QString title = dialog.getSearchTitle();
            if (title.isEmpty() || (title.length() >= 50)){
                qDebug() << "Ошибка пустого ввода";
                return;
            } else {
                notesList->clear();
                for (QListWidgetItem *item : noteManager.getNoteItems()) {

                    QDateTime createdTime = item->data(Qt::UserRole + 1).toDateTime();
                    QDateTime reminderTime = item->data(Qt::UserRole + 2).toDateTime();

                    QString noteText = item->text();
                    if (title == noteText) {
                    QString noteInfo = QString("Дата создания:\n %1\nВремя уведомления:\n %2")
                                           .arg(createdTime.toString())
                                           .arg(reminderTime.isValid() ? reminderTime.toString() : "Не установленно");

                    QListWidgetItem *newItem = new QListWidgetItem(noteText);
                    newItem->setData(Qt::UserRole, item->data(Qt::UserRole));
                    newItem->setData(Qt::UserRole + 1, item->data(Qt::UserRole + 1));
                    newItem->setData(Qt::UserRole + 2, item->data(Qt::UserRole + 2));
                    newItem->setToolTip(noteInfo);
                    notesList->addItem(newItem);
                    }
                }
               }
        }
}

void MainWindow::updateMenu() {
    notesList->clear();
    for (QListWidgetItem *item : noteManager.getNoteItems()) {

        QDateTime createdTime = item->data(Qt::UserRole + 1).toDateTime();
        QDateTime reminderTime = item->data(Qt::UserRole + 2).toDateTime();

        QString noteText = item->text();
        QString noteInfo = QString("Дата создания:\n %1\nВремя уведомления:\n %2")
                               .arg(createdTime.toString())
                               .arg(reminderTime.isValid() ? reminderTime.toString() : "Не установленно");

        QListWidgetItem *newItem = new QListWidgetItem(noteText);
        newItem->setData(Qt::UserRole, item->data(Qt::UserRole));
        newItem->setData(Qt::UserRole + 1, item->data(Qt::UserRole + 1));
        newItem->setData(Qt::UserRole + 2, item->data(Qt::UserRole + 2));
        newItem->setToolTip(noteInfo);
        notesList->addItem(newItem);
    }
}

void MainWindow::createMenu() {
    QMenu *fileMenu = menuBar()->addMenu(tr("File"));
    QMenu *Aabout = menuBar()->addMenu(tr("Info"));

    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Database is not open!";
        return;
    }

    for (QListWidgetItem *item : noteManager.getNoteItems()) {

        QDateTime createdTime = item->data(Qt::UserRole + 1).toDateTime();
        QDateTime reminderTime = item->data(Qt::UserRole + 2).toDateTime();

        QString noteText = item->text();
        QString noteInfo = QString("Дата создания:\n %1\nВремя уведомления:\n %2")
                               .arg(createdTime.toString())
                               .arg(reminderTime.isValid() ? reminderTime.toString() : "Не установленно");

        QListWidgetItem *newItem = new QListWidgetItem(noteText);
        newItem->setData(Qt::UserRole, item->data(Qt::UserRole));
        newItem->setData(Qt::UserRole + 1, item->data(Qt::UserRole + 1));
        newItem->setData(Qt::UserRole + 2, item->data(Qt::UserRole + 2));
        newItem->setToolTip(noteInfo);
        notesList->addItem(newItem);
    }





//*****************************************
    QAction *addNoteAction = fileMenu->addAction(tr("Добавить запись"));
    connect(addNoteAction, &QAction::triggered, this, &MainWindow::onAddNote);

    QAction *searchAction = fileMenu->addAction(tr("Поиск"));
    connect(searchAction, &QAction::triggered, this, &MainWindow::onSearchDialog);

    QAction *updateAction = fileMenu->addAction(tr("Обновить"));
    connect(updateAction, &QAction::triggered, this, &MainWindow::updateMenu);

    QAction *about = Aabout->addAction(tr("О программе"));
    connect(about, &QAction::triggered, this, &MainWindow::AboutMe);

//*****************************************


    deleteNoteAction = new QAction(tr("Удалить"), this);
    connect(deleteNoteAction, &QAction::triggered, this, &MainWindow::onDeleteNote);

    setReminderAction = new QAction(tr("Поставить уведомление"), this);
    connect(setReminderAction, &QAction::triggered, this, &MainWindow::onSetReminder);

    openNoteAction = new QAction(tr("Редактировать"), this);
    connect(openNoteAction, &QAction::triggered, this, &MainWindow::onOpenNote);

    connect(notesList, &QListWidget::itemClicked, this, &MainWindow::onNoteSelected);


    notesList->addAction(deleteNoteAction);
    notesList->addAction(openNoteAction);
    notesList->addAction(setReminderAction);


    QMenu *contextMenu = new QMenu(this);
    contextMenu->addAction(deleteNoteAction);
    contextMenu->addAction(openNoteAction);
    contextMenu->addAction(setReminderAction);

    notesList->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(notesList, &QListWidget::customContextMenuRequested, [=](const QPoint &pos){
            contextMenu->setProperty("selectedNoteTitle", notesList->itemAt(pos)->text());
            contextMenu->setProperty("selectedNoteFilename", notesList->itemAt(pos)->data(Qt::UserRole).toString());
            contextMenu->exec(notesList->mapToGlobal(pos));
        });
}
