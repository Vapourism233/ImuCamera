/**
 * Interfacing with SYD Dynamics Motion Module - QT Example
 *
 * @brief  Implements a basic GUI that interfaces with Motion Module using
 *         SYD Dynamics Communication Abstraction Layer (CAL) in C++.
 *
 * @note   Serial Port is used in this example. Users can choose directly connect to
 *         a signle motion module using its UART interface or use a SYD Dynamics
 *         Serial-to-CANbus converter to access an entire sensor network.
 *
 * @version Jul 28, 2016 - First Release Version
 *          Jun 20, 2017 - Add Sending Calibration Command Example
 *          Jul 11, 2017 - Add Change Setting Example
 *          Feb 22, 2018 - Add void Serial_ISR(char* rxData, int rxSize) to avoid
 *                         RX buffer overflow.
 *          Mar 07, 2018 - Example for EasyProfile V1-2-3-R3
 *
 * @attention
 * <h2><center>&copy; COPYRIGHT(c) 2018 SYD Dynamics ApS</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef HELLOMOTIONMODULE_H
#define HELLOMOTIONMODULE_H

#include <QtWidgets/QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtTest/QTest>

// To use the communication library, we need to include the following
// two header files:
#include "EasyProfile/EasyObjectDictionary.h"
#include "EasyProfile/EasyProfile.h"

namespace Ui {
class HelloMotionModule;
}

class HelloMotionModule : public QMainWindow
{
    Q_OBJECT

public:
    explicit HelloMotionModule(QWidget *parent = 0);
    ~HelloMotionModule();

    // Serial Port & GUI:
private:
    Ui::HelloMotionModule *ui;
    QSerialPort*          serial;   // We use the QSerialPort to interface with serial port.
                                    // QSerialPort is part of the QT Framework.
    void Serial_ISR(char* rxData, int rxSize);

private slots:
    void on_pushButton_Serialport_Open_clicked();
    void on_spinBox_TX_Request_ShortId_valueChanged(int arg1);
    void on_pushButton_UseBroadcastId_clicked();

    // TX (Send Request) :
private slots:
    void on_pushButton_TX_Request_RPY_clicked();
    void on_pushButton_TX_Request_Quaternion_clicked();

    // RX (Receive Sensor Data) :
private slots:
    void on_SerialRX();
};

#endif // HELLOMOTIONMODULE_H
