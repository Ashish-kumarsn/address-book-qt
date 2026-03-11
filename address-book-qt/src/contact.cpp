#include "contact.h"
#include <QRegularExpression>

Contact::Contact()
    : m_id(-1), m_name(""), m_mobile(""), m_email(""), m_birthday("")
{}

Contact::Contact(int id, const QString &name, const QString &mobile,
                 const QString &email, const QString &birthday)
    : m_id(id), m_name(name), m_mobile(mobile),
      m_email(email), m_birthday(birthday)
{}

// Getters
int Contact::id() const { return m_id; }
QString Contact::name() const { return m_name; }
QString Contact::mobile() const { return m_mobile; }
QString Contact::email() const { return m_email; }
QString Contact::birthday() const { return m_birthday; }

// Setters
void Contact::setId(int id) { m_id = id; }
void Contact::setName(const QString &name) { m_name = name; }
void Contact::setMobile(const QString &mobile) { m_mobile = mobile; }
void Contact::setEmail(const QString &email) { m_email = email; }
void Contact::setBirthday(const QString &birthday) { m_birthday = birthday; }

// Validation
bool Contact::isValidEmail(const QString &email) {
    QRegularExpression rx(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return rx.match(email).hasMatch();
}

bool Contact::isValidMobile(const QString &mobile) {
    QRegularExpression rx(R"(^\+?[0-9]{7,15}$)");
    return rx.match(mobile).hasMatch();
}

bool Contact::isValidBirthday(const QString &birthday) {
    QRegularExpression rx(R"(^\d{4}-\d{2}-\d{2}$)");
    return rx.match(birthday).hasMatch();
}

bool Contact::isValid() const {
    return !m_name.trimmed().isEmpty()
           && isValidMobile(m_mobile)
           && isValidEmail(m_email)
           && isValidBirthday(m_birthday);
}
