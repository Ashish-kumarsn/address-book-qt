#include <QtTest>
#include "../src/contact.h"
#include "../src/databasemanager.h"

class TestContact : public QObject
{
    Q_OBJECT

private slots:
    void testValidEmail_data();
    void testValidEmail();
    void testValidMobile_data();
    void testValidMobile();
    void testValidBirthday_data();
    void testValidBirthday();
    void testContactCreation();
    void testDatabaseAddAndGet();
    void testDatabaseUpdate();
    void testDatabaseDelete();
};

// ── Email tests ───────────────────────────────────────────────
void TestContact::testValidEmail_data()
{
    QTest::addColumn<QString>("email");
    QTest::addColumn<bool>("expected");

    QTest::newRow("valid1")        << "test@example.com"   << true;
    QTest::newRow("valid2")        << "user.name@domain.org" << true;
    QTest::newRow("missing_at")    << "invalidemail.com"   << false;
    QTest::newRow("missing_dot")   << "user@domain"        << false;
    QTest::newRow("empty")         << ""                   << false;
}

void TestContact::testValidEmail()
{
    QFETCH(QString, email);
    QFETCH(bool, expected);
    QCOMPARE(Contact::isValidEmail(email), expected);
}

// ── Mobile tests ──────────────────────────────────────────────
void TestContact::testValidMobile_data()
{
    QTest::addColumn<QString>("mobile");
    QTest::addColumn<bool>("expected");

    QTest::newRow("valid_local")   << "9876543210"    << true;
    QTest::newRow("valid_intl")    << "+911234567890" << true;
    QTest::newRow("too_short")     << "123"           << false;
    QTest::newRow("letters")       << "abcdefgh"      << false;
    QTest::newRow("empty")         << ""              << false;
}

void TestContact::testValidMobile()
{
    QFETCH(QString, mobile);
    QFETCH(bool, expected);
    QCOMPARE(Contact::isValidMobile(mobile), expected);
}

// ── Birthday tests ────────────────────────────────────────────
void TestContact::testValidBirthday_data()
{
    QTest::addColumn<QString>("birthday");
    QTest::addColumn<bool>("expected");

    QTest::newRow("valid")         << "1990-05-21"  << true;
    QTest::newRow("wrong_format")  << "21/05/1990"  << false;
    QTest::newRow("missing_dashes")<< "19900521"    << false;
    QTest::newRow("empty")         << ""            << false;
}

void TestContact::testValidBirthday()
{
    QFETCH(QString, birthday);
    QFETCH(bool, expected);
    QCOMPARE(Contact::isValidBirthday(birthday), expected);
}

// ── Contact creation test ─────────────────────────────────────
void TestContact::testContactCreation()
{
    Contact c(1, "John Doe", "+911234567890",
              "john@example.com", "1990-01-15");

    QCOMPARE(c.id(),       1);
    QCOMPARE(c.name(),     QString("John Doe"));
    QCOMPARE(c.mobile(),   QString("+911234567890"));
    QCOMPARE(c.email(),    QString("john@example.com"));
    QCOMPARE(c.birthday(), QString("1990-01-15"));
    QVERIFY(c.isValid());
}

// ── Database tests ────────────────────────────────────────────
void TestContact::testDatabaseAddAndGet()
{
    DatabaseManager db(":memory:");
    QVERIFY(db.open());

    Contact c(-1, "Alice", "9876543210",
              "alice@example.com", "1995-03-10");
    QVERIFY(db.addContact(c));
    QVERIFY(c.id() > 0);

    QList<Contact> all = db.getAllContacts();
    QCOMPARE(all.size(), 1);
    QCOMPARE(all[0].name(), QString("Alice"));
}

void TestContact::testDatabaseUpdate()
{
    DatabaseManager db(":memory:");
    QVERIFY(db.open());

    Contact c(-1, "Bob", "1234567890",
              "bob@example.com", "1988-07-22");
    QVERIFY(db.addContact(c));

    c.setName("Bob Updated");
    QVERIFY(db.updateContact(c));

    Contact updated = db.getContactById(c.id());
    QCOMPARE(updated.name(), QString("Bob Updated"));
}

void TestContact::testDatabaseDelete()
{
    DatabaseManager db(":memory:");
    QVERIFY(db.open());

    Contact c(-1, "Charlie", "9999999999",
              "charlie@example.com", "2000-12-01");
    QVERIFY(db.addContact(c));
    QVERIFY(db.deleteContact(c.id()));

    QList<Contact> all = db.getAllContacts();
    QCOMPARE(all.size(), 0);
}

QTEST_MAIN(TestContact)
#include "tst_contact.moc"
