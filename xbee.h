#ifndef XBEE_H
#define XBEE_H

#include <QObject>
#include <QtCore>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include "settingsdialog.h"

class Xbee : public QSerialPort
{
    QString name;

public:
    Xbee(QObject *parent = Q_NULLPTR);
    void setRemoteRelay(QByteArray remAddr, int relay, int status);
    void setRemoteAPI(QByteArray remAddr, bool checked);
    void setSleep(QByteArray remAddr, bool sleep);
    void getRemoteRelay(QByteArray remAddr, int relay);

private:
    int checkSum(QByteArray messageBody);
    void remoteATcommand(QByteArray remAddr, QByteArray ATCommand, int parameter);
    void remoteATcommand(QByteArray remAddr, QByteArray ATCommand);

private slots:

};

#endif // XBEE_H

