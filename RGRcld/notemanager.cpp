#include "notemanager.h"
#include <QDebug>
#include <QDateTime>

NoteManager::NoteManager(QObject *parent): QObject(parent){
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("F:/Reminder/RGRcld/forbd/my.db");
    if (!database.open()) {
        qDebug() << "Error opening database in NoteManager:" << database.lastError().text();
    } else {
        qDebug() << "Database opened successfully in NoteManager!";
    }
    loadNotesFromDatabase();
}
void NoteManager::setNoteReminderTime(QListWidgetItem *item, const QDateTime &reminderTime) {
    QSqlDatabase::database().transaction();
    if (reminderTime.isValid()) {
        QSqlQuery query;
        query.prepare("UPDATE notes SET reminderTime = :reminderTime WHERE title = :title");
        query.bindValue(":title", item->text());
        query.bindValue(":reminderTime", reminderTime);

        if (query.exec()) {
            qDebug() << "Note updated to the database.";
            QSqlDatabase::database().commit();
            emit noteUpdated();  // Отправляем сигнал об обновлении
        } else {
            qDebug() << "[FROM db]Error updating note in the database:" << query.lastError().text();
            QSqlDatabase::database().rollback();
        }


        qDebug() << "Note reminder time set for item: " << item->text();
    }
}
void NoteManager::loadNotesFromDatabase() {
    QSqlQuery query("SELECT title, filename, datecreate, reminderTime FROM notes");
    if (!query.isActive()) {
        qDebug() << "Error loading notes from the database:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString title = query.value(0).toString();
        QString filename = query.value(1).toString();
        QDateTime datecreate = query.value(2).toDateTime();
        QDateTime reminderTime = query.value(3).toDateTime();
        addNoteFromDatabase(title, filename, datecreate, reminderTime);
    }
}

void NoteManager::deleteNoteFromDatabase(const QString &filename) {
    QSqlDatabase::database().transaction();

    QSqlQuery query;
    query.prepare("DELETE FROM notes WHERE filename = :filename");
    query.bindValue(":filename", filename);

    if (query.exec()) {
        qDebug() << "[FROM db] - Note deleted from the database.";
        QFile file(filename);
        if (file.remove()) {
            qDebug() << "[FROM FILE] - File deleted successfully.";
        } else {
            qDebug() << "[FROM FILE] - Error deleting file:" << file.errorString();
        }
        QSqlDatabase::database().commit();
        emit noteUpdated();  // Отправляем сигнал об обновлении
    } else {
        qDebug() << "[FROM db] - Error deleting note from the database:" << query.lastError().text();
        QSqlDatabase::database().rollback();
    }
}

void NoteManager::addNoteFromDatabase(const QString &title, const QString &filename, const QDateTime &datecreate, const QDateTime &reminderTime) {
    QListWidgetItem *item = new QListWidgetItem(title);
    item->setData(Qt::UserRole, filename);
    item->setData(Qt::UserRole + 1, datecreate);
    item->setData(Qt::UserRole + 2, reminderTime);  // Добавляем данные о времени уведомления
    noteItems.append(item);
}

void NoteManager::addNoteFromFileAndDatabase(const QString &title, const QString &filename, QDateTime &datecreate, const QString &noteContent, const QDateTime &reminderTime) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << noteContent;

        addNoteToDatabase(title, filename, datecreate, reminderTime);
        addNoteFromDatabase(title, filename, datecreate, reminderTime);
        file.close();
    } else {
        qDebug() << "[FROM FILE] - Error opening file for writing:" << file.errorString();
    }
}

void NoteManager::addNoteToDatabase(const QString &title, const QString &filename, const QDateTime &datecreate, const QDateTime &reminderTime) {
    QSqlDatabase::database().transaction();

    QSqlQuery query;
    query.prepare("INSERT INTO notes (title, filename, datecreate, reminderTime) VALUES (:title, :filename, :datecreate, :reminderTime)");
    query.bindValue(":title", title);
    query.bindValue(":filename", filename);
    query.bindValue(":datecreate", datecreate);
    query.bindValue(":reminderTime", reminderTime);

    if (query.exec()) {
        qDebug() << "Note added to the database.";
        QSqlDatabase::database().commit();
        emit noteUpdated();  // Отправляем сигнал об обновлении
    } else {
        qDebug() << "[FROM db]Error adding note to the database:" << query.lastError().text();
        QSqlDatabase::database().rollback();
    }
}




QList<QListWidgetItem *> NoteManager::getNoteItems() const {
    return noteItems;
}
