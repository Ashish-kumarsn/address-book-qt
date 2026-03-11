#include "contactdialog.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFont>

ContactDialog::ContactDialog(QWidget *parent, const Contact &contact)
    : QDialog(parent), m_contact(contact)
{
    setupUI();
    populateFields(contact);
    validateFields();
}

void ContactDialog::setupUI()
{
    bool isEdit = (m_contact.id() != -1);
    setWindowTitle(isEdit ? "Edit Contact" : "Add Contact");
    setMinimumWidth(400);
    setModal(true);

    m_nameEdit    = new QLineEdit(this);
    m_mobileEdit  = new QLineEdit(this);
    m_emailEdit   = new QLineEdit(this);
    m_birthdayEdit = new QDateEdit(this);

    m_nameEdit->setPlaceholderText("e.g. John Doe");
    m_mobileEdit->setPlaceholderText("e.g. +911234567890");
    m_emailEdit->setPlaceholderText("e.g. john@example.com");

    m_birthdayEdit->setDisplayFormat("yyyy-MM-dd");
    m_birthdayEdit->setCalendarPopup(true);
    m_birthdayEdit->setDate(QDate(1990, 1, 1));

    m_nameError   = new QLabel(this);
    m_mobileError = new QLabel(this);
    m_emailError  = new QLabel(this);

    QString errStyle = "color: red; font-size: 11px;";
    m_nameError->setStyleSheet(errStyle);
    m_mobileError->setStyleSheet(errStyle);
    m_emailError->setStyleSheet(errStyle);

    m_nameError->setVisible(false);
    m_mobileError->setVisible(false);
    m_emailError->setVisible(false);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(6);
    formLayout->addRow("Name *",     m_nameEdit);
    formLayout->addRow("",           m_nameError);
    formLayout->addRow("Mobile *",   m_mobileEdit);
    formLayout->addRow("",           m_mobileError);
    formLayout->addRow("Email *",    m_emailEdit);
    formLayout->addRow("",           m_emailError);
    formLayout->addRow("Birthday *", m_birthdayEdit);

    m_buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    m_buttonBox->button(QDialogButtonBox::Ok)->setText(
        isEdit ? "Save Changes" : "Add Contact");
    m_buttonBox->button(QDialogButtonBox::Ok)->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; "
        "padding: 6px 16px; border-radius: 4px; font-weight: bold; }"
        "QPushButton:hover { background-color: #45a049; }"
        "QPushButton:disabled { background-color: #aaaaaa; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted,
            this, &ContactDialog::onAccepted);
    connect(m_buttonBox, &QDialogButtonBox::rejected,
            this, &QDialog::reject);
    connect(m_nameEdit,   &QLineEdit::textChanged,
            this, &ContactDialog::validateFields);
    connect(m_mobileEdit, &QLineEdit::textChanged,
            this, &ContactDialog::validateFields);
    connect(m_emailEdit,  &QLineEdit::textChanged,
            this, &ContactDialog::validateFields);
}

void ContactDialog::populateFields(const Contact &contact)
{
    if (contact.id() == -1) return;
    m_nameEdit->setText(contact.name());
    m_mobileEdit->setText(contact.mobile());
    m_emailEdit->setText(contact.email());
    QDate date = QDate::fromString(contact.birthday(), "yyyy-MM-dd");
    if (date.isValid())
        m_birthdayEdit->setDate(date);
}

bool ContactDialog::validate()
{
    bool valid = true;

    if (m_nameEdit->text().trimmed().isEmpty()) {
        m_nameError->setText("Name cannot be empty.");
        m_nameError->setVisible(true);
        valid = false;
    } else {
        m_nameError->setVisible(false);
    }

    if (!Contact::isValidMobile(m_mobileEdit->text().trimmed())) {
        m_mobileError->setText("Enter a valid mobile number (7-15 digits).");
        m_mobileError->setVisible(true);
        valid = false;
    } else {
        m_mobileError->setVisible(false);
    }

    if (!Contact::isValidEmail(m_emailEdit->text().trimmed())) {
        m_emailError->setText("Enter a valid email address.");
        m_emailError->setVisible(true);
        valid = false;
    } else {
        m_emailError->setVisible(false);
    }

    return valid;
}

void ContactDialog::validateFields()
{
    bool valid = true;
    if (m_nameEdit->text().trimmed().isEmpty())                  valid = false;
    if (!Contact::isValidMobile(m_mobileEdit->text().trimmed())) valid = false;
    if (!Contact::isValidEmail(m_emailEdit->text().trimmed()))   valid = false;
    m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}

void ContactDialog::onAccepted()
{
    if (!validate()) return;
    accept();
}

Contact ContactDialog::getContact() const
{
    Contact c;
    c.setName(m_nameEdit->text().trimmed());
    c.setMobile(m_mobileEdit->text().trimmed());
    c.setEmail(m_emailEdit->text().trimmed());
    c.setBirthday(m_birthdayEdit->date().toString("yyyy-MM-dd"));
    return c;
}
