#ifndef XBEE_H
#define XBEE_H

#include <QObject>
#include <QtCore>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include "settingsdialog.h"

/*
struct Frame {
    char sentinel = 0x7e;
    char length[2];
    char checkSum;

    virtual QByteArray toByteArray();
};

struct AtCmdFrame : Frame {
    char frameType = 0x17;
    char frameId = 0x01;
    char remoteAddr[8];
    const char reserved[2] = {0xff, 0xfe};
    char cmdOptions;
    char atCmd[2];
    char parameter = char(NULL);
    char checksum;

    QByteArray toByteArray(){
        QByteArray cmdString;
        cmdString.append(sentinel);
        cmdString.append(length, 2);
        cmdString.append(frameType);
        cmdString.append(remoteAddr, 8);
        cmdString.append(*reserved, 2);
        cmdString.append()
        cmdString.append(cmdOptions);
        cmdString.append(atCmd, 2);
        cmdString.append(parameter);
        cmdString.append(checksum);

        return cmdString;
    }
};*/

class Xbee : public QSerialPort
{
    QString name;
    static const char k_sentinel = 0x7e;
    static const char k_typeRemoteAT = 0x17;
    //static const char k_typeRemoteAT_reserved[2] = {0xff, 0xfe};
    static const char k_typeRemoteAT_saveChanges= 0x02;
    static const char k_typeAT = 0x08;

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

