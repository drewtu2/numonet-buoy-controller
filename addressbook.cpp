#include "addressbook.h"

AddressBook::AddressBook(QStringList line) {
    //Constructor
}

void AddressBook::Add(QStringList line) {
    AddressBook::address_book.push_back(Entry());
    int size = AddressBook::address_book.size();
    AddressBook::address_book[size].xbee_name_ = line.at(0);
    AddressBook::address_book[size].xbee_address_ = line.at(1).toLatin1();
}

QByteArray AddressBook::Get(QString name) {
    for (std::vector<Entry>::iterator it = AddressBook::address_book.begin(); it != AddressBook::address_book.end(); ++it) {
        if (it->xbee_name_ == name) {
            return it->xbee_address_;
        }
    }
    return NULL;
}

void AddressBook::Print(){
    for (std::vector<Entry>::iterator it = AddressBook::address_book.begin(); it != AddressBook::address_book.end(); ++it) {
        qDebug() << "Name: " << it->xbee_name_;
        qDebug() << "Address: " << it->xbee_address_;
    }
}
