#include "mainwindow.h"
#include "contactdialog.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QStatusBar>
#include <QLabel>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_db = new DatabaseManager();
    if (!m_db->open()) {
        QMessageBox::critical(this, "Database Error",
            "Could not open database:\n" + m_db->lastError());
    }
    setupUI();
    loadContacts();
}

MainWindow::~MainWindow()
{
    m_db->close();
    delete m_db;
}

void MainWindow::setupUI()
{
    setWindowTitle("Address Book");
    setMinimumSize(750, 500);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    QLabel *searchLabel = new QLabel("Search:", central);
    m_searchBox = new QLineEdit(central);
    m_searchBox->setPlaceholderText("Search by name, email or mobile...");
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(m_searchBox);

    m_table = new QTableWidget(central);
    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels({"Name", "Mobile", "Email", "Birthday"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setSortingEnabled(true);

    m_addBtn    = new QPushButton("Add Contact",    central);
    m_editBtn   = new QPushButton("Edit Contact",   central);
    m_deleteBtn = new QPushButton("Delete Contact", central);

    m_addBtn->setMinimumHeight(35);
    m_editBtn->setMinimumHeight(35);
    m_deleteBtn->setMinimumHeight(35);

    m_addBtn->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; "
        "border-radius: 4px; font-weight: bold; }"
        "QPushButton:hover { background-color: #45a049; }");
    m_editBtn->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; "
        "border-radius: 4px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1976D2; }");
    m_deleteBtn->setStyleSheet(
        "QPushButton { background-color: #f44336; color: white; "
        "border-radius: 4px; font-weight: bold; }"
        "QPushButton:hover { background-color: #d32f2f; }");

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_addBtn);
    btnLayout->addWidget(m_editBtn);
    btnLayout->addWidget(m_deleteBtn);
    btnLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(8);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(m_table);
    mainLayout->addLayout(btnLayout);

    statusBar()->showMessage("Ready");

    connect(m_addBtn,    &QPushButton::clicked,
            this, &MainWindow::onAddContact);
    connect(m_editBtn,   &QPushButton::clicked,
            this, &MainWindow::onEditContact);
    connect(m_deleteBtn, &QPushButton::clicked,
            this, &MainWindow::onDeleteContact);
    connect(m_searchBox, &QLineEdit::textChanged,
            this, &MainWindow::onSearchChanged);
    connect(m_table,     &QTableWidget::itemSelectionChanged,
            this, &MainWindow::onTableSelectionChanged);
    connect(m_table,     &QTableWidget::doubleClicked,
            this, &MainWindow::onEditContact);

    updateButtonStates();
}

void MainWindow::loadContacts()
{
    loadContacts(m_searchBox->text());
}

void MainWindow::loadContacts(const QString &filter)
{
    m_contacts = m_db->getAllContacts();
    m_table->setRowCount(0);

    for (const Contact &c : m_contacts) {
        if (!filter.isEmpty()) {
            bool match = c.name().contains(filter, Qt::CaseInsensitive)
                      || c.mobile().contains(filter, Qt::CaseInsensitive)
                      || c.email().contains(filter, Qt::CaseInsensitive);
            if (!match) continue;
        }

        int row = m_table->rowCount();
        m_table->insertRow(row);
        m_table->setItem(row, 0, new QTableWidgetItem(c.name()));
        m_table->setItem(row, 1, new QTableWidgetItem(c.mobile()));
        m_table->setItem(row, 2, new QTableWidgetItem(c.email()));
        m_table->setItem(row, 3, new QTableWidgetItem(c.birthday()));
        m_table->item(row, 0)->setData(Qt::UserRole, c.id());
    }

    statusBar()->showMessage(
        QString("%1 contact(s) found").arg(m_table->rowCount()));
    updateButtonStates();
}

void MainWindow::onAddContact()
{
    ContactDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Contact c = dialog.getContact();
        if (m_db->addContact(c)) {
            loadContacts();
            statusBar()->showMessage("Contact added successfully.");
        } else {
            QMessageBox::warning(this, "Error",
                "Failed to add contact:\n" + m_db->lastError());
        }
    }
}

void MainWindow::onEditContact()
{
    int row = m_table->currentRow();
    if (row < 0) return;

    int id = m_table->item(row, 0)->data(Qt::UserRole).toInt();
    Contact c = m_db->getContactById(id);

    ContactDialog dialog(this, c);
    if (dialog.exec() == QDialog::Accepted) {
        Contact updated = dialog.getContact();
        updated.setId(id);
        if (m_db->updateContact(updated)) {
            loadContacts();
            statusBar()->showMessage("Contact updated successfully.");
        } else {
            QMessageBox::warning(this, "Error",
                "Failed to update contact:\n" + m_db->lastError());
        }
    }
}

void MainWindow::onDeleteContact()
{
    int row = m_table->currentRow();
    if (row < 0) return;

    QString name = m_table->item(row, 0)->text();
    int id = m_table->item(row, 0)->data(Qt::UserRole).toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Delete Contact",
        "Are you sure you want to delete \"" + name + "\"?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (m_db->deleteContact(id)) {
            loadContacts();
            statusBar()->showMessage("Contact deleted.");
        } else {
            QMessageBox::warning(this, "Error",
                "Failed to delete contact:\n" + m_db->lastError());
        }
    }
}

void MainWindow::onSearchChanged(const QString &text)
{
    loadContacts(text);
}

void MainWindow::onTableSelectionChanged()
{
    updateButtonStates();
}

void MainWindow::updateButtonStates()
{
    bool selected = m_table->currentRow() >= 0;
    m_editBtn->setEnabled(selected);
    m_deleteBtn->setEnabled(selected);
}
