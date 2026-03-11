#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QStandardPaths>
#include <QDir>

DatabaseManager::DatabaseManager(const QString &dbPath)
{
    if (dbPath.isEmpty()) {
        QString dataDir = QStandardPaths::writableLocation(
            QStandardPaths::AppDataLocation);
        QDir().mkpath(dataDir);
        m_dbPath = dataDir + "/addressbook.db";
    } else {
        m_dbPath = dbPath;
    }
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_dbPath);
}

DatabaseManager::~DatabaseManager()
{
    close();
}

bool DatabaseManager::open()
{
    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        return false;
    }
    return createTable();
}

void DatabaseManager::close()
{
    if (m_db.isOpen())
        m_db.close();
}

bool DatabaseManager::isOpen() const
{
    return m_db.isOpen();
}

bool DatabaseManager::createTable()
{
    QSqlQuery query;
    bool ok = query.exec(
        "CREATE TABLE IF NOT EXISTS contacts ("
        "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name     TEXT    NOT NULL,"
        "mobile   TEXT    NOT NULL,"
        "email    TEXT    NOT NULL,"
        "birthday TEXT    NOT NULL"
        ");"
    );
    if (!ok)
        m_lastError = query.lastError().text();
    return ok;
}

bool DatabaseManager::addContact(Contact &contact)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO contacts (name, mobile, email, birthday) "
        "VALUES (:name, :mobile, :email, :birthday);"
    );
    query.bindValue(":name",     contact.name());
    query.bindValue(":mobile",   contact.mobile());
    query.bindValue(":email",    contact.email());
    query.bindValue(":birthday", contact.birthday());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    contact.setId(query.lastInsertId().toInt());
    return true;
}

bool DatabaseManager::updateContact(const Contact &contact)
{
    QSqlQuery query;
    query.prepare(
        "UPDATE contacts SET name=:name, mobile=:mobile, "
        "email=:email, birthday=:birthday WHERE id=:id;"
    );
    query.bindValue(":name",     contact.name());
    query.bindValue(":mobile",   contact.mobile());
    query.bindValue(":email",    contact.email());
    query.bindValue(":birthday", contact.birthday());
    query.bindValue(":id",       contact.id());

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::deleteContact(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM contacts WHERE id=:id;");
    query.bindValue(":id", id);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return false;
    }
    return true;
}

QList<Contact> DatabaseManager::getAllContacts()
{
    QList<Contact> contacts;
    QSqlQuery query("SELECT id, name, mobile, email, birthday "
                    "FROM contacts ORDER BY name;");

    while (query.next()) {
        Contact c(
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString()
        );
        contacts.append(c);
    }
    return contacts;
}

Contact DatabaseManager::getContactById(int id)
{
    QSqlQuery query;
    query.prepare("SELECT id, name, mobile, email, birthday "
                  "FROM contacts WHERE id=:id;");
    query.bindValue(":id", id);
    query.exec();

    if (query.next()) {
        return Contact(
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString()
        );
    }
    return Contact();
}

QString DatabaseManager::lastError() const
{
    return m_lastError;
}
