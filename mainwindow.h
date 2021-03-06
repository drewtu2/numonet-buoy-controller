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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "xbee.h"
#include "addressbook.h"
#include "console.h"
#include "ui_mainwindow.h"

#include <QtCore/QtGlobal>
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>

QT_BEGIN_NAMESPACE

class QLabel;

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class Console;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Console *console;
    AddressBook address_book;
    ~MainWindow();


private slots:
    void openSerialPort();
    void closeSerialPort();
    void updateAddressBook();
    void about();
    void writeData(const QByteArray &data);
    QByteArray readData();

    void handleError(QSerialPort::SerialPortError error);

    void on_Relay1_toggled(bool checked);
    void on_Relay2_toggled(bool checked);
    void on_Relay3_toggled(bool checked);
    void on_Relay4_toggled(bool checked);

    void on_LocalAPI_toggled(bool checked);
    void on_RemoteAPI_toggled(bool checked);

    void on_SleepButton_toggled(bool checked);

    void on_RemoteXbeeSelector_currentTextChanged(const QString &arg1);

    void on_RefreshButton_clicked();



private:
    void showStatusMessage(const QString &message);
    void initActionsConnections();
    void loadRemoteAddr();
    void sdelay(int secs);
    void mdelay(int secs);
    void setRemoteAddr(QString name);
    void setIndicator(int RelayNum, bool status);
    void updateIndicators();
    void updateXbeeSelector();
    void setPathToAddressBook();


    QByteArray remoteAddr;
    Ui::MainWindow *ui;
    QLabel *status;
    SettingsDialog *settings;
    Xbee localXbee;
    QString pathToAddressBook;
};

#endif // MAINWINDOW_H
