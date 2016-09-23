/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "mainwindow.h"

#include <string>
#include <QMessageBox>
#include <QLabel>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    QPixmap logo(":/Resources/Images/NUMONET_Logo.jpg");
    ui->Logo->setPixmap(logo);

    // Set Central Widget to the Main Widget (Our Controller)
    setCentralWidget(ui->MainWidget);

    // Create Xbee and Serial Settings
    settings = new SettingsDialog;
    //localXbee(this);

    //address_book();
    updateAddressBook();
    ui->RemoteXbeeSelector->setCurrentText("Venus"); // Default to Venus for now

    // Create Console Widget and attach to dock
    console = new Console;
    console->setEnabled(false);
    ui->ConsoleWidget->addWidget(console);

    // Set Menu Bar Actions
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    // Create Label
    status = new QLabel;
    ui->statusBar->addWidget(status);

    initActionsConnections();
    console->putData("GUI", 1);
}
/**
 * @brief MainWindow::~MainWindow
 * Deconstructor for the MainWindow object
 */
MainWindow::~MainWindow()
{
    //delete settings;
    delete ui;
}
/**
 * @brief MainWindow::about
 * Displays a message box with a basic about message when run
 */
void MainWindow::about()
{
    QMessageBox::about(this, tr("About NU MONET Buoy Controller"),
                       tr("The <b>NU MONET Buoy Controller</b> was developed by "
                          "Andrew Tu to control the smart buoys in use by the "
                          "NU MONET project. Please contact Andrew at tu.a@husky.neu.edu"
                          "with any questions"));
}
/**
 * @brief MainWindow::openSerialPort
 * Open a conneciton to the port designated by the current settings.
 */
void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    localXbee.setPortName(p.name);
    localXbee.setBaudRate(p.baudRate);
    localXbee.setDataBits(p.dataBits);
    localXbee.setParity(p.parity);
    localXbee.setStopBits(p.stopBits);
    localXbee.setFlowControl(p.flowControl);

    if (localXbee.open(QIODevice::ReadWrite)) {
        console->setEnabled(true);
        console->setLocalEchoEnabled(p.localEchoEnabled);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        console->putData("SerialPort", 1);
        qDebug() << "Connected to Serial Port";
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    }
    else {
        QMessageBox::critical(this, tr("Error"), localXbee.errorString());
        showStatusMessage(tr("Open error"));
    }
}
/**
 * @brief MainWindow::closeSerialPort
 * Closes the active serial port connection (if exists).
 */
void MainWindow::closeSerialPort()
{
    if (localXbee.isOpen())
        localXbee.close();
    console->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    console->putData("SerialPort", 0);
    showStatusMessage(tr("Disconnected"));
}

/**
 * @brief MainWindow::writeData
 * @param data
 * Takes in a QByteArray and transmits the information DIRECTLY to the serial port
 */
void MainWindow::writeData(const QByteArray &data)
{
    localXbee.write(data);
}

/**
 * @brief MainWindow::readData
 * Reads incoming data from the serial port and displays it on the console
 */
QByteArray MainWindow::readData()
{
    if (ui->LocalTransparent->isChecked()) {
        QByteArray data = localXbee.readAll();
        qDebug() <<"Incoming Hex Data: " << data.toHex() << endl;
        qDebug() <<"Data (Raw): " << data << endl;
        qDebug() <<"Data Length: " << data.length() << endl;
        console->putData(data.toHex() + "\n");
        return data;
    } else if (ui->LocalAPI->isChecked()) {
        console->putData("API Command Response\n");
        return NULL;
    } else {
        return NULL;
    }
}


/**
 * @brief MainWindow::handleError
 * @param error
 *
 */
void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), localXbee.errorString());
        closeSerialPort();
    }
}
/**
 * @brief MainWindow::initActionsConnections
 * Connects GUI Buttons to their respective functions
 */
void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionConfigure, &QAction::triggered, settings, &MainWindow::show);
    connect(ui->actionClear, &QAction::triggered, console, &Console::clear);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(settings, &SettingsDialog::settingsApplied, this, &MainWindow::updateAddressBook);
    connect(&localXbee, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MainWindow::handleError);
    // As data come FROM serial port, DISPLAY on Console
    connect(&localXbee, &QSerialPort::readyRead,
            this, &MainWindow::readData);
    // As data comes from console, write to localXbee
    connect(console, &Console::getData,
            this, &MainWindow::writeData);
    /**************************/

}

void MainWindow::updateAddressBook()
{
    setPathToAddressBook();
    loadRemoteAddr();
    updateXbeeSelector();
}

void MainWindow::updateXbeeSelector()
{
    // Create Xbee selector (via Combo box)
    //qDebug() << ui->RemoteXbeeSelector->count();
    for (int i = 0; i <= ui->RemoteXbeeSelector->count(); i++){
        ui->RemoteXbeeSelector->removeItem(0);
    }
    //qDebug() << address_book.getSize();
    for (int i = 0; i < address_book.getSize(); i++){
        ui->RemoteXbeeSelector->addItem(address_book.getName(i));
    }
}

/**
 * @brief MainWindow::showStatusMessage
 * @param message
 * Sets the message on the status bar to a given message
 */
void MainWindow::showStatusMessage(const QString &message)
{
    status->setText(message);
}

/**
 * @brief MainWindow::sdelay
 * @param secs
 * Pauses proccesses all running proccesses for "secs" seconds.
 */
void MainWindow::sdelay(int secs)
{
    QTime dieTime= QTime::currentTime().addSecs(secs);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

/**
 * @brief MainWindow::mdelay
 * @param secs
 * Pauses proccesses all running proccesses for "secs" seconds.
 */
void MainWindow::mdelay(int msecs)
{
    QTime dieTime= QTime::currentTime().addMSecs(msecs);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::setPathToAddressBook()
{
    qDebug() << "Updated pathToAddressBook";
    pathToAddressBook = settings->address_path_;
}

void MainWindow::loadRemoteAddr()
{
    QFile address_file(pathToAddressBook);
    if (!address_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Unable to open file";
        return;
    }
    address_book.clear();
    QTextStream in(&address_file);
    while (!in.atEnd()) {
         QString line = in.readLine();
         address_book.add(line.split(' '));
    }
    address_file.close();

}

void MainWindow::setRemoteAddr(QString name)
{
    remoteAddr = address_book.getAddress(name);
}

void MainWindow::on_RemoteXbeeSelector_currentTextChanged(const QString &arg1)
{
    setRemoteAddr(arg1);
    ui->RemoteAddress->setText(address_book.getAddress(arg1).toHex()); //Display what our current remote address is
}


// Toggled Relays
void MainWindow::on_Relay1_toggled(bool checked)
{
    int RelayNum = 1;
    localXbee.setRemoteRelay(remoteAddr, RelayNum, checked);
    setIndicator(RelayNum, checked);
    console->putData("Relay", RelayNum, checked);
}

void MainWindow::on_Relay2_toggled(bool checked)
{
    int RelayNum = 2;
    localXbee.setRemoteRelay(remoteAddr, RelayNum, checked);
    setIndicator(RelayNum, checked);
    console->putData("Relay", RelayNum, checked);
}

void MainWindow::on_Relay3_toggled(bool checked)
{
    int RelayNum = 3;
    localXbee.setRemoteRelay(remoteAddr, RelayNum, checked);
    setIndicator(RelayNum, checked);
    console->putData("Relay", RelayNum, checked);
}

void MainWindow::on_Relay4_toggled(bool checked)
{
    int RelayNum = 4;
    localXbee.setRemoteRelay(remoteAddr, RelayNum, checked);
    setIndicator(RelayNum, checked);
    console->putData("Relay", RelayNum, checked);
}
void MainWindow::setIndicator(int RelayNum, bool status)
{

    QString color = "green";
    if(status == 0) color = "red";

    switch(RelayNum){
    case 1:
        ui->Indicator1->setStyleSheet("QLabel { background-color :"+  color + ";}");
        break;
    case 2:
        ui->Indicator2->setStyleSheet("QLabel { background-color :"+  color + ";}");
        break;
    case 3:
        ui->Indicator3->setStyleSheet("QLabel { background-color :"+  color + ";}");
        break;
    case 4:
        ui->Indicator4->setStyleSheet("QLabel { background-color :"+  color + ";}");
        break;

    }
}
    ;
// Toggled API's
void MainWindow::on_LocalAPI_toggled(bool checked)
{
    localXbee.write("+++");
    mdelay(300);
    localXbee.write("ATAP");

    if(checked)
        localXbee.write("1\r");
    else
        localXbee.write("0\r");

    localXbee.write("ATWR\r");
    localXbee.write("ATCN\r");
    mdelay(300);
    console->putData("LocalAPI", checked);
}

void MainWindow::on_RemoteAPI_toggled(bool checked)
{
    localXbee.setRemoteAPI(remoteAddr, checked);
    console->putData("RemoteAPI", checked);
}

//Toggled Sleep
void MainWindow::on_SleepButton_toggled(bool checked)
{
    localXbee.setSleep(remoteAddr, checked);
    console->putData("Sleep", checked);
}


void MainWindow::on_RefreshButton_clicked()
{
   updateIndicators();
}

void MainWindow::updateIndicators()
{
//    localXbee.clear();
//    qDebug() << "Update Indicators";
//    localXbee.getRemoteRelay(remoteAddr, 1);
//    localXbee.getRemoteRelay(remoteAddr, 2);
//    localXbee.getRemoteRelay(remoteAddr, 3);
//    localXbee.getRemoteRelay(remoteAddr, 4);

//    QByteArray data = readData();

//    setIndicator(1, bool(data[3]-'0'));
//    setIndicator(2, bool(data[5]-'0'));
//    setIndicator(3, bool(data[7]-'0'));
//    setIndicator(4, bool(data[9]-'0'));
}

