#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QList>
#include <QDir>
#include "contact.h"

class DatabaseManager {
public:
    explicit DatabaseManager(const QString &dbPath = QString());
    ~DatabaseManager();

    bool open();
    void close();
    bool isOpen() const;

    bool addContact(Contact &contact);
    bool updateContact(const Contact &contact);
    bool deleteContact(int id);
    QList<Contact> getAllContacts();
    Contact getContactById(int id);

    QString lastError() const;

private:
    bool createTable();

    QSqlDatabase m_db;
    QString m_dbPath;
    QString m_lastError;
};

#endif 
