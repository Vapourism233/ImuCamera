/********************************************************************************
** Form generated from reading UI file 'HelloMotionModule.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELLOMOTIONMODULE_H
#define UI_HELLOMOTIONMODULE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HelloMotionModule
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QLabel *label_2;
    QGroupBox *groupBox_Example;
    QGridLayout *gridLayout_7;
    QLabel *label_16;
    QPushButton *pushButton_TX_Request_RPY;
    QFrame *line_2;
    QPushButton *pushButton_TX_Request_Quaternion;
    QSpacerItem *verticalSpacer_3;
    QSpinBox *spinBox_TX_Request_ShortId;
    QPushButton *pushButton_UseBroadcastId;
    QGroupBox *groupBox_SerialPort;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit_Serialport_Name;
    QPushButton *pushButton_Serialport_Open;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_6;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_4;
    QDoubleSpinBox *doubleSpinBox_RX_Pitch;
    QDoubleSpinBox *doubleSpinBox_RX_Roll;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_RX_RPY_TimeStamp;
    QDoubleSpinBox *doubleSpinBox_RX_Yaw;
    QLabel *label_7;
    QLabel *label_12;
    QLabel *label_RX_RPY_ID;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_5;
    QDoubleSpinBox *doubleSpinBox_RX_Q4;
    QDoubleSpinBox *doubleSpinBox_RX_Q1;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox_RX_Q3;
    QDoubleSpinBox *doubleSpinBox_RX_Q2;
    QLabel *label_10;
    QLabel *label_8;
    QLabel *label_11;
    QLabel *label_9;
    QLabel *label_RX_Q_TimeStamp;
    QLabel *label_13;
    QLabel *label_RX_Q_ID;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_8;
    QLabel *label_RX_Calib_Status;
    QLabel *label_RX_Setting_Status;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *HelloMotionModule)
    {
        if (HelloMotionModule->objectName().isEmpty())
            HelloMotionModule->setObjectName(QString::fromUtf8("HelloMotionModule"));
        HelloMotionModule->resize(682, 688);
        centralWidget = new QWidget(HelloMotionModule);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 1, 0, 1, 1);

        groupBox_Example = new QGroupBox(groupBox);
        groupBox_Example->setObjectName(QString::fromUtf8("groupBox_Example"));
        gridLayout_7 = new QGridLayout(groupBox_Example);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        label_16 = new QLabel(groupBox_Example);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setWordWrap(true);

        gridLayout_7->addWidget(label_16, 2, 0, 1, 1);

        pushButton_TX_Request_RPY = new QPushButton(groupBox_Example);
        pushButton_TX_Request_RPY->setObjectName(QString::fromUtf8("pushButton_TX_Request_RPY"));

        gridLayout_7->addWidget(pushButton_TX_Request_RPY, 0, 0, 1, 1);

        line_2 = new QFrame(groupBox_Example);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_7->addWidget(line_2, 3, 0, 1, 1);

        pushButton_TX_Request_Quaternion = new QPushButton(groupBox_Example);
        pushButton_TX_Request_Quaternion->setObjectName(QString::fromUtf8("pushButton_TX_Request_Quaternion"));

        gridLayout_7->addWidget(pushButton_TX_Request_Quaternion, 1, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer_3, 7, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_Example, 4, 0, 1, 2);

        spinBox_TX_Request_ShortId = new QSpinBox(groupBox);
        spinBox_TX_Request_ShortId->setObjectName(QString::fromUtf8("spinBox_TX_Request_ShortId"));
        spinBox_TX_Request_ShortId->setMinimum(0);
        spinBox_TX_Request_ShortId->setMaximum(2047);

        gridLayout_3->addWidget(spinBox_TX_Request_ShortId, 2, 0, 1, 1);

        pushButton_UseBroadcastId = new QPushButton(groupBox);
        pushButton_UseBroadcastId->setObjectName(QString::fromUtf8("pushButton_UseBroadcastId"));
        pushButton_UseBroadcastId->setEnabled(false);

        gridLayout_3->addWidget(pushButton_UseBroadcastId, 2, 1, 1, 1);


        gridLayout_2->addWidget(groupBox, 1, 0, 1, 1);

        groupBox_SerialPort = new QGroupBox(centralWidget);
        groupBox_SerialPort->setObjectName(QString::fromUtf8("groupBox_SerialPort"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_SerialPort->sizePolicy().hasHeightForWidth());
        groupBox_SerialPort->setSizePolicy(sizePolicy1);
        gridLayout = new QGridLayout(groupBox_SerialPort);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(groupBox_SerialPort);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit_Serialport_Name = new QLineEdit(groupBox_SerialPort);
        lineEdit_Serialport_Name->setObjectName(QString::fromUtf8("lineEdit_Serialport_Name"));

        gridLayout->addWidget(lineEdit_Serialport_Name, 0, 1, 1, 1);

        pushButton_Serialport_Open = new QPushButton(groupBox_SerialPort);
        pushButton_Serialport_Open->setObjectName(QString::fromUtf8("pushButton_Serialport_Open"));

        gridLayout->addWidget(pushButton_Serialport_Open, 0, 2, 1, 1);


        gridLayout_2->addWidget(groupBox_SerialPort, 0, 0, 1, 2);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        gridLayout_6 = new QGridLayout(groupBox_2);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        groupBox_3 = new QGroupBox(groupBox_2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_4 = new QGridLayout(groupBox_3);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        doubleSpinBox_RX_Pitch = new QDoubleSpinBox(groupBox_3);
        doubleSpinBox_RX_Pitch->setObjectName(QString::fromUtf8("doubleSpinBox_RX_Pitch"));
        doubleSpinBox_RX_Pitch->setReadOnly(true);
        doubleSpinBox_RX_Pitch->setButtonSymbols(QAbstractSpinBox::NoButtons);
        doubleSpinBox_RX_Pitch->setDecimals(1);
        doubleSpinBox_RX_Pitch->setMinimum(-720.000000000000000);
        doubleSpinBox_RX_Pitch->setMaximum(720.000000000000000);

        gridLayout_4->addWidget(doubleSpinBox_RX_Pitch, 1, 1, 1, 1);

        doubleSpinBox_RX_Roll = new QDoubleSpinBox(groupBox_3);
        doubleSpinBox_RX_Roll->setObjectName(QString::fromUtf8("doubleSpinBox_RX_Roll"));
        doubleSpinBox_RX_Roll->setReadOnly(true);
        doubleSpinBox_RX_Roll->setButtonSymbols(QAbstractSpinBox::NoButtons);
        doubleSpinBox_RX_Roll->setDecimals(1);
        doubleSpinBox_RX_Roll->setMinimum(-720.000000000000000);
        doubleSpinBox_RX_Roll->setMaximum(720.000000000000000);

        gridLayout_4->addWidget(doubleSpinBox_RX_Roll, 0, 1, 1, 1);

        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_4->addWidget(label_4, 0, 0, 1, 1);

        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_4->addWidget(label_5, 1, 0, 1, 1);

        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_4->addWidget(label_6, 2, 0, 1, 1);

        label_RX_RPY_TimeStamp = new QLabel(groupBox_3);
        label_RX_RPY_TimeStamp->setObjectName(QString::fromUtf8("label_RX_RPY_TimeStamp"));

        gridLayout_4->addWidget(label_RX_RPY_TimeStamp, 3, 1, 1, 1);

        doubleSpinBox_RX_Yaw = new QDoubleSpinBox(groupBox_3);
        doubleSpinBox_RX_Yaw->setObjectName(QString::fromUtf8("doubleSpinBox_RX_Yaw"));
        doubleSpinBox_RX_Yaw->setReadOnly(true);
        doubleSpinBox_RX_Yaw->setButtonSymbols(QAbstractSpinBox::NoButtons);
        doubleSpinBox_RX_Yaw->setDecimals(1);
        doubleSpinBox_RX_Yaw->setMinimum(-720.000000000000000);
        doubleSpinBox_RX_Yaw->setMaximum(720.000000000000000);

        gridLayout_4->addWidget(doubleSpinBox_RX_Yaw, 2, 1, 1, 1);

        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_4->addWidget(label_7, 3, 0, 1, 1);

        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_4->addWidget(label_12, 4, 0, 1, 1);

        label_RX_RPY_ID = new QLabel(groupBox_3);
        label_RX_RPY_ID->setObjectName(QString::fromUtf8("label_RX_RPY_ID"));

        gridLayout_4->addWidget(label_RX_RPY_ID, 4, 1, 1, 1);


        gridLayout_6->addWidget(groupBox_3, 0, 0, 1, 1);

        groupBox_4 = new QGroupBox(groupBox_2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_5 = new QGridLayout(groupBox_4);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        doubleSpinBox_RX_Q4 = new QDoubleSpinBox(groupBox_4);
        doubleSpinBox_RX_Q4->setObjectName(QString::fromUtf8("doubleSpinBox_RX_Q4"));
        doubleSpinBox_RX_Q4->setReadOnly(true);
        doubleSpinBox_RX_Q4->setButtonSymbols(QAbstractSpinBox::NoButtons);
        doubleSpinBox_RX_Q4->setDecimals(4);
        doubleSpinBox_RX_Q4->setMinimum(-99.000000000000000);

        gridLayout_5->addWidget(doubleSpinBox_RX_Q4, 3, 1, 1, 1);

        doubleSpinBox_RX_Q1 = new QDoubleSpinBox(groupBox_4);
        doubleSpinBox_RX_Q1->setObjectName(QString::fromUtf8("doubleSpinBox_RX_Q1"));
        doubleSpinBox_RX_Q1->setReadOnly(true);
        doubleSpinBox_RX_Q1->setButtonSymbols(QAbstractSpinBox::NoButtons);
        doubleSpinBox_RX_Q1->setDecimals(4);
        doubleSpinBox_RX_Q1->setMinimum(-99.000000000000000);
        doubleSpinBox_RX_Q1->setMaximum(99.000000000000000);

        gridLayout_5->addWidget(doubleSpinBox_RX_Q1, 0, 1, 1, 1);

        label_3 = new QLabel(groupBox_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_5->addWidget(label_3, 0, 0, 1, 1);

        doubleSpinBox_RX_Q3 = new QDoubleSpinBox(groupBox_4);
        doubleSpinBox_RX_Q3->setObjectName(QString::fromUtf8("doubleSpinBox_RX_Q3"));
        doubleSpinBox_RX_Q3->setReadOnly(true);
        doubleSpinBox_RX_Q3->setButtonSymbols(QAbstractSpinBox::NoButtons);
        doubleSpinBox_RX_Q3->setDecimals(4);
        doubleSpinBox_RX_Q3->setMinimum(-99.000000000000000);

        gridLayout_5->addWidget(doubleSpinBox_RX_Q3, 2, 1, 1, 1);

        doubleSpinBox_RX_Q2 = new QDoubleSpinBox(groupBox_4);
        doubleSpinBox_RX_Q2->setObjectName(QString::fromUtf8("doubleSpinBox_RX_Q2"));
        doubleSpinBox_RX_Q2->setReadOnly(true);
        doubleSpinBox_RX_Q2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        doubleSpinBox_RX_Q2->setDecimals(4);
        doubleSpinBox_RX_Q2->setMinimum(-99.000000000000000);

        gridLayout_5->addWidget(doubleSpinBox_RX_Q2, 1, 1, 1, 1);

        label_10 = new QLabel(groupBox_4);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_5->addWidget(label_10, 3, 0, 1, 1);

        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_5->addWidget(label_8, 1, 0, 1, 1);

        label_11 = new QLabel(groupBox_4);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_5->addWidget(label_11, 4, 0, 1, 1);

        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_5->addWidget(label_9, 2, 0, 1, 1);

        label_RX_Q_TimeStamp = new QLabel(groupBox_4);
        label_RX_Q_TimeStamp->setObjectName(QString::fromUtf8("label_RX_Q_TimeStamp"));

        gridLayout_5->addWidget(label_RX_Q_TimeStamp, 4, 1, 1, 1);

        label_13 = new QLabel(groupBox_4);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_5->addWidget(label_13, 5, 0, 1, 1);

        label_RX_Q_ID = new QLabel(groupBox_4);
        label_RX_Q_ID->setObjectName(QString::fromUtf8("label_RX_Q_ID"));

        gridLayout_5->addWidget(label_RX_Q_ID, 5, 1, 1, 1);


        gridLayout_6->addWidget(groupBox_4, 1, 0, 1, 1);

        groupBox_5 = new QGroupBox(groupBox_2);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_8 = new QGridLayout(groupBox_5);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        label_RX_Calib_Status = new QLabel(groupBox_5);
        label_RX_Calib_Status->setObjectName(QString::fromUtf8("label_RX_Calib_Status"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_RX_Calib_Status->setFont(font);

        gridLayout_8->addWidget(label_RX_Calib_Status, 0, 0, 1, 1);

        label_RX_Setting_Status = new QLabel(groupBox_5);
        label_RX_Setting_Status->setObjectName(QString::fromUtf8("label_RX_Setting_Status"));
        QFont font1;
        font1.setPointSize(12);
        label_RX_Setting_Status->setFont(font1);
        label_RX_Setting_Status->setWordWrap(true);

        gridLayout_8->addWidget(label_RX_Setting_Status, 1, 0, 1, 1);


        gridLayout_6->addWidget(groupBox_5, 2, 0, 1, 1);


        gridLayout_2->addWidget(groupBox_2, 1, 1, 1, 1);

        HelloMotionModule->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(HelloMotionModule);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        HelloMotionModule->setStatusBar(statusBar);

        retranslateUi(HelloMotionModule);

        QMetaObject::connectSlotsByName(HelloMotionModule);
    } // setupUi

    void retranslateUi(QMainWindow *HelloMotionModule)
    {
        HelloMotionModule->setWindowTitle(QCoreApplication::translate("HelloMotionModule", "HelloMotionModule (With EasyProfile V1.2.3 R3)", nullptr));
        groupBox->setTitle(QCoreApplication::translate("HelloMotionModule", "Send Data", nullptr));
        label_2->setText(QCoreApplication::translate("HelloMotionModule", "Send request to the device with Short ID", nullptr));
        groupBox_Example->setTitle(QString());
        label_16->setText(QCoreApplication::translate("HelloMotionModule", "Send a request, and then the module returns the requested data.", nullptr));
        pushButton_TX_Request_RPY->setText(QCoreApplication::translate("HelloMotionModule", "Example:  Request Roll/Pitch/Yaw", nullptr));
        pushButton_TX_Request_Quaternion->setText(QCoreApplication::translate("HelloMotionModule", "Example:  Request Quaternion", nullptr));
        pushButton_UseBroadcastId->setText(QCoreApplication::translate("HelloMotionModule", "Use Broadcast ID", nullptr));
        groupBox_SerialPort->setTitle(QCoreApplication::translate("HelloMotionModule", "Serial Port", nullptr));
        label->setText(QCoreApplication::translate("HelloMotionModule", "Port name", nullptr));
        lineEdit_Serialport_Name->setText(QCoreApplication::translate("HelloMotionModule", "COM5", nullptr));
        pushButton_Serialport_Open->setText(QCoreApplication::translate("HelloMotionModule", "Open Port", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("HelloMotionModule", "Received Data", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("HelloMotionModule", "RPY", nullptr));
        label_4->setText(QCoreApplication::translate("HelloMotionModule", "Roll (degree)", nullptr));
        label_5->setText(QCoreApplication::translate("HelloMotionModule", "Pitch (degree)", nullptr));
        label_6->setText(QCoreApplication::translate("HelloMotionModule", "Yaw (degree)", nullptr));
        label_RX_RPY_TimeStamp->setText(QCoreApplication::translate("HelloMotionModule", "-", nullptr));
        label_7->setText(QCoreApplication::translate("HelloMotionModule", "Timestamp (uS)", nullptr));
        label_12->setText(QCoreApplication::translate("HelloMotionModule", "Device ID", nullptr));
        label_RX_RPY_ID->setText(QCoreApplication::translate("HelloMotionModule", "-", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("HelloMotionModule", "Quaternion", nullptr));
        label_3->setText(QCoreApplication::translate("HelloMotionModule", "Q1", nullptr));
        label_10->setText(QCoreApplication::translate("HelloMotionModule", "Q4", nullptr));
        label_8->setText(QCoreApplication::translate("HelloMotionModule", "Q2", nullptr));
        label_11->setText(QCoreApplication::translate("HelloMotionModule", "Timestamp (uS)", nullptr));
        label_9->setText(QCoreApplication::translate("HelloMotionModule", "Q3", nullptr));
        label_RX_Q_TimeStamp->setText(QCoreApplication::translate("HelloMotionModule", "-", nullptr));
        label_13->setText(QCoreApplication::translate("HelloMotionModule", "Device ID", nullptr));
        label_RX_Q_ID->setText(QCoreApplication::translate("HelloMotionModule", "-", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("HelloMotionModule", "Status", nullptr));
        label_RX_Calib_Status->setText(QString());
        label_RX_Setting_Status->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class HelloMotionModule: public Ui_HelloMotionModule {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELLOMOTIONMODULE_H
