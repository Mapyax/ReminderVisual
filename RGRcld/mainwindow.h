// В файле mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QSqlDatabase>
#include <QSystemTrayIcon>
#include "notemanager.h"
#include "popup.h"
#include <QTableWidget>
#include <QtMultimedia>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void AboutMe();
    void scheduleReminder(const QString &title, const QString &content, const QDateTime &reminderDateTime);
    void readSettings();

private:
    QMenu *contextMenu;  // Добавим переменную для контекстного меню

    QListWidget *notesList;
    QSqlDatabase database;  // Объект базы данных как член класса
    NoteManager noteManager;
    QAction *deleteNoteAction;
    QAction *openNoteAction;
    QAction *setReminderAction;
    QMediaPlayer *AlarmSound;
    void saveSettings();
    QString loadSettings();
    PopUp *popUp;       // Объявляем объект всплывающего сообщения

    // Добавляем члены для сохранения размера и положения окна
    QByteArray windowGeometry;
    QByteArray windowState;

    void createMenu();
    void updateMenu();
    void loadNotesFromManager();
    void onDeleteNote();
    void onSetReminder();
public slots:

private slots:
    QString readNoteContentFromFile(const QString &filename);
    void onNoteSelected(QListWidgetItem *item);
    void onAddNote();
    void onSearchDialog();
    void onOpenNote();

protected:
    // Добавляем методы для сохранения и восстановления настроек окна
    void closeEvent(QCloseEvent *event) override;

};

#endif // MAINWINDOW_H
