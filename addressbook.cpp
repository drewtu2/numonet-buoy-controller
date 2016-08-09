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

QByteArray AddressBook::GetAddress(QString name) {
    for (std::vector<Entry>::iterator it = AddressBook::book.begin(); it != AddressBook::book.end(); ++it) {
        if (it->xbee_name_ == name) {
            return it->xbee_address_;
        }
    }
    return NULL;
}

QString AddressBook::GetName(int index) {
    return AddressBook::book[index].xbee_name_;
}

int AddressBook::GetSize() {
    return AddressBook::book.size();
}

void AddressBook::Print() {
    for (std::vector<Entry>::iterator it = AddressBook::book.begin(); it != AddressBook::book.end(); ++it) {
        qDebug() << "Name: " << it->xbee_name_;
        qDebug() << "Address: " << it->xbee_address_;
    }
}

void AddressBook::clear() {
    book.erase(book.begin(), book.end());
}
