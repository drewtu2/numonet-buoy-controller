#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <QStringList>
#include <QByteArray>
#include <QDebug>
#include <vector>

struct Entry {
    QString xbee_name_;
    QByteArray xbee_address_;
};

class AddressBook {

public:
    std::vector<Entry> book_;
    AddressBook();
    QByteArray getAddress(QString name);
    QString getName(int index);
    int getSize();
    void add(QStringList line);
    void print();
    void clear();
};
#endif // ADDRESSBOOK_H

