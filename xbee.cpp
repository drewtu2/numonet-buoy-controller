#include "xbee.h"
#include <iostream>
#include <string>

/**
 * @brief Xbee::Xbee * Initialize the Xbee with the proper serial port connection opened through the console application
 * @param portInput
 */
Xbee::Xbee(QObject *parent) : QSerialPort(parent)
{

}

/**
 * @brief Xbee::setRemoteRelay: Turns the given relay on or off based on a toggle from the GUI
 * @param remAddr: The address of who we are communicating with
 * @param relay: Which relay we are operating
 * @param status: What we are setting the relay to
 */
void Xbee::setRemoteRelay(QByteArray remAddr, int relay, int status)
{
    //Command Arguments for each relay
        //Relay 1 -> D0
        //Relay 2 -> D1
        //Relay 3 -> D2
        //Relay 4 -> D3
    char commandArr[] = "D";
    QByteArray command = QByteArray::fromRawData(commandArr, 1);
    command.append(QByteArray::number(relay-1));

    //Status is 0/off 1/on from GUI
    //Parameter for Xbee is 4/off 5/on
    int parameter = 4 + status;

    remoteATcommand(remAddr, command ,parameter);
}
void Xbee::getRemoteRelay(QByteArray remAddr, int relay)
{
    //Command Arguments for each relay
        //Relay 1 -> D0
        //Relay 2 -> D1
        //Relay 3 -> D2
        //Relay 4 -> D3

    char commandArr[] = "D";
    QByteArray command = QByteArray::fromRawData(commandArr, 1);
    command.append(QByteArray::number(relay-1));
    remoteATcommand(remAddr, command);
}

/**
 * @brief Xbee::setSleep: sends the "sleep" or "wake" commands to the remote Xbee
 * @param remAddr
 * @param sleep
 */
void Xbee::setSleep(QByteArray remAddr, bool sleep)
{
    char commandArr[] = "SM";
    QByteArray command = QByteArray::fromRawData(commandArr, 2);

    //Status is 0/wake 1/sleep from GUI
    //Parameter for Xbee is 0x00/wake 0x04/sleep
    int parameter = 4*sleep;

    remoteATcommand(remAddr, command ,parameter);
}

void Xbee::setRemoteAPI(QByteArray remAddr, bool checked)
{
    char commandArr[] = "AP";
    QByteArray command = QByteArray::fromRawData(commandArr, 2);

    //Status is 0/Transparent 1/API from GUI
    //Parameter for Xbee is 0x00/Transparent 0x01/API
    int parameter = 1*checked;

    remoteATcommand(remAddr, command ,parameter);
}

void Xbee::remoteATcommand(QByteArray remAddr, QByteArray ATCommand, int parameter)
{
    QByteArray num;
    num[0] = 0x7E; //Sentinel
    num[1] = 0x00; //LengthMSB
    num[2] = 0x10; //LengthLSB

    num[3] = 0x17; //Message is AT Command
    num[4] = 0x01;  //FrameID
    for (int i = 0; i < 8; i++) //64Bit Address
    {
        num[5+i] = remAddr[i];
    }                           //End 64Bit Address
    num[13] = 0xFF; //Reserved
    num[14] = 0xFE; //Reserved
    num[15] = 0x02; //Remote Command Options 0x02 saves changes on exit
    num[16] = ATCommand[0]; //AT Command Byte 1
    num[17] = ATCommand[1]; //AT Command Byte 2
    num[18] = parameter; //Command Parameter
    num[19] = checkSum(num.mid(3));

    //Write the command
    write(num);
    qDebug() << "Hex Cmd Sent: " << num << "\n";
}

void Xbee::remoteATcommand(QByteArray remAddr, QByteArray ATCommand)
{
    QByteArray num;
    num[0] = k_sentinel; //Sentinel
    num[1] = 0x00; //LengthMSB
    num[2] = 0x0F; //LengthLSB
    num[3] = k_typeRemoteAT; //Message is remote AT Command
    num[4] = 0x01;  //FrameID
    num.append(remAddr);
    num.append(0xff); // reserved
    num.append(0xfe); // reserved
    num.append(k_typeRemoteAT_saveChanges); //Remote Command Options 0x02 saves changes on exit
    num.append(ATCommand);
    num.append(checkSum(num.mid(3)));

    //Write the command
    write(num);
    qDebug() << "Hex Cmd Sent: " << num.toHex() << "\n";
}

int Xbee::checkSum(QByteArray messageBody)
{
    uchar sum = 0;
    for(int i = 0; i < messageBody.length(); i++)
    {
        sum += uchar(messageBody[i]);

    }
    int checkSum = 0xFF-(sum%255);
    return checkSum;
}
