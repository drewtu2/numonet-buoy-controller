#include "addressbook.h"

AddressBook::AddressBook()
{
    //Constructor
}

void AddressBook::add(QStringList line)
{
    AddressBook::book_.push_back(Entry());
    int size = AddressBook::book_.size();
    AddressBook::book_[size - 1].xbee_name_ = line.at(0);
    AddressBook::book_[size - 1].xbee_address_ = QByteArray::fromHex(line.at(1).toLatin1());
}

QByteArray AddressBook::getAddress(QString name)
{
    for (std::vector<Entry>::iterator it = AddressBook::book_.begin(); it != AddressBook::book_.end(); ++it) {
        if (it->xbee_name_ == name) {
            return it->xbee_address_;
        }
    }
    return NULL;
}

QString AddressBook::getName(int index)
{
    return AddressBook::book_[index].xbee_name_;
}

int AddressBook::getSize()
{
    return AddressBook::book_.size();
}

void AddressBook::print()
{
    for (std::vector<Entry>::iterator it = AddressBook::book_.begin(); it != AddressBook::book_.end(); ++it) {
        qDebug() << "Name: " << it->xbee_name_;
        qDebug() << "Address: " << it->xbee_address_;
    }
}

void AddressBook::clear() {
    book_.erase(book_.begin(), book_.end());
}
