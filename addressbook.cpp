#include "addressbook.h"

AddressBook::AddressBook() {
    //Constructor
}

void AddressBook::Add(QStringList line) {
    AddressBook::book.push_back(Entry());
    int size = AddressBook::book.size();
    AddressBook::book[size - 1].xbee_name_ = line.at(0);
    AddressBook::book[size - 1].xbee_address_ = QByteArray::fromHex(line.at(1).toLatin1());
}

QByteArray AddressBook::Get(QString name) {
    for (std::vector<Entry>::iterator it = AddressBook::book.begin(); it != AddressBook::book.end(); ++it) {
        if (it->xbee_name_ == name) {
            return it->xbee_address_;
        }
    }
    return NULL;
}

void AddressBook::Print() {
    for (std::vector<Entry>::iterator it = AddressBook::book.begin(); it != AddressBook::book.end(); ++it) {
        qDebug() << "Name: " << it->xbee_name_;
        qDebug() << "Address: " << it->xbee_address_;
    }
}
