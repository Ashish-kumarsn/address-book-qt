#ifndef CONTACT_H
#define CONTACT_H

#include <QString>

class Contact {
public:
    Contact();
    Contact(int id, const QString &name, const QString &mobile,
            const QString &email, const QString &birthday);

    // Getters
    int id() const;
    QString name() const;
    QString mobile() const;
    QString email() const;
    QString birthday() const;

    // Setters
    void setId(int id);
    void setName(const QString &name);
    void setMobile(const QString &mobile);
    void setEmail(const QString &email);
    void setBirthday(const QString &birthday);

    // Validation
    static bool isValidEmail(const QString &email);
    static bool isValidMobile(const QString &mobile);
    static bool isValidBirthday(const QString &birthday);
    bool isValid() const;

private:
    int m_id;
    QString m_name;
    QString m_mobile;
    QString m_email;
    QString m_birthday;
};

#endif 
