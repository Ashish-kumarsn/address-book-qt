#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "databasemanager.h"
#include "contact.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddContact();
    void onEditContact();
    void onDeleteContact();
    void onSearchChanged(const QString &text);
    void onTableSelectionChanged();

private:
    void setupUI();
    void loadContacts();
    void loadContacts(const QString &filter);
    void updateButtonStates();

    // UI Elements
    QTableWidget    *m_table;
    QPushButton     *m_addBtn;
    QPushButton     *m_editBtn;
    QPushButton     *m_deleteBtn;
    QLineEdit       *m_searchBox;

    // Database
    DatabaseManager *m_db;

    // Data
    QList<Contact>   m_contacts;
};

#endif 
