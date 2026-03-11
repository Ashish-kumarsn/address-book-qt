#ifndef CONTACTDIALOG_H
#define CONTACTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QFormLayout>
#include <QDialogButtonBox>
#include "contact.h"

class ContactDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContactDialog(QWidget *parent = nullptr,
                           const Contact &contact = Contact());

    Contact getContact() const;

private slots:
    void onAccepted();
    void validateFields();

private:
    void setupUI();
    void populateFields(const Contact &contact);
    bool validate();

    // UI Elements
    QLineEdit       *m_nameEdit;
    QLineEdit       *m_mobileEdit;
    QLineEdit       *m_emailEdit;
    QDateEdit       *m_birthdayEdit;
    QLabel          *m_nameError;
    QLabel          *m_mobileError;
    QLabel          *m_emailError;
    QDialogButtonBox *m_buttonBox;

    Contact          m_contact;
};

#endif 
