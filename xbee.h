#ifndef XBEE_H
#define XBEE_H

#include <QObject>
#include <QtCore>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include "settingsdialog.h"

class Xbee
{
    QSerialPort *XbeeSerial;
    QString name;

public:
    Xbee(QSerialPort *portInput);
    void setRemoteRelay(QByteArray remAddr, int relay, int status);
    void setRemoteAPI(QByteArray remAddr, bool checked);
    void setSleep(QByteArray remAddr, bool sleep);
    void getRemoteRelay(QByteArray remAddr, int relay);

private:
    int checkSum(QByteArray messageBody);
    void remoteATcommand(QByteArray remAddr, QByteArray ATCommand, int parameter);
    void remoteATcommand(QByteArray remAddr, QByteArray ATCommand);
private slots:
    //void openSerialPort();
    //void closeSerialPort();

};

#endif // XBEE_H

