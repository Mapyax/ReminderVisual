#ifndef NOTEMANAGER_H
#define NOTEMANAGER_H

#include <QObject>
#include <QListWidgetItem>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>
#include <QFile>
#include <QDateTime>

class NoteManager : public QObject {
    Q_OBJECT
private:
    QList<QListWidgetItem *> noteItems;

public slots:
    void setNoteReminderTime(QListWidgetItem *item, const QDateTime &reminderTime);

public:

    // Получить время создания заметки
        QDateTime getNoteCreatedTime(QListWidgetItem *item) const {
            // Верните время создания, сохраненное в данных элемента списка
            return item->data(Qt::UserRole + 1).toDateTime();
        }

        // Получить время уведомления для заметкиdx
        QDateTime getNoteReminderTime(QListWidgetItem *item) const {
            // Верните время уведомления, сохраненное в данных элемента списка
            return item->data(Qt::UserRole + 2).toDateTime();
        }


    NoteManager(QObject *parent = nullptr);
    QList<QListWidgetItem *> getNoteItems() const;

    void loadNotesFromDatabase();
    void onDeleteNote();
    void deleteNoteFromDatabase(const QString &filename);


    void addNoteFromDatabase(const QString &title, const QString &filename, const QDateTime &datecreate, const QDateTime &reminderTime);
    void addNoteFromFileAndDatabase(const QString &title, const QString &filename, QDateTime &datecreate, const QString &noteContent, const QDateTime &reminderTime);
    void addNoteToDatabase(const QString &title, const QString &filename, const QDateTime &datecreate, const QDateTime &reminderTime);

    void scheduleReminder(const  QString          &title,
                          const  QString          &content,
                          const  QDateTime        &reminderDateTime);
signals:
    void noteUpdated();
    void noteAdded();
    void noteDeleted();
    void reminderTimeUpdated();
};

#endif // NOTEMANAGER
