/********************************************************************************
** Form generated from reading UI file 'mainusergui.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINUSERGUI_H
#define UI_MAINUSERGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include "color_wheel.hpp"
#include "qwt_knob.h"

QT_BEGIN_NAMESPACE

class Ui_MainUserGUI
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QSplitter *splitter_2;
    QCheckBox *rojo;
    QCheckBox *verde;
    QCheckBox *azul;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QwtKnob *Knob;
    QLabel *label_2;
    color_widgets::ColorWheel *colorWheel;
    QWidget *gridLayoutWidget;
    QGridLayout *grid_modo;
    QCheckBox *PWM_GPIO;
    QPlainTextEdit *plainTextEdit;
    QWidget *tab_2;
    QPushButton *ADCButton;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_2;
    QLCDNumber *lcdCh1;
    QLCDNumber *lcdCh3;
    QLCDNumber *lcdCh2;
    QLCDNumber *lcdCh4;
    QWidget *tab_3;
    QSplitter *splitter_4;
    QSplitter *splitter;
    QLabel *serialPortLabel;
    QComboBox *serialPortComboBox;
    QSplitter *splitter_3;
    QPushButton *runButton;
    QPushButton *pingButton;
    QSplitter *splitter_5;
    QPushButton *pushButton;
    QLabel *statusLabel;

    void setupUi(QWidget *MainUserGUI)
    {
        if (MainUserGUI->objectName().isEmpty())
            MainUserGUI->setObjectName(QString::fromUtf8("MainUserGUI"));
        MainUserGUI->resize(868, 468);
        tabWidget = new QTabWidget(MainUserGUI);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 851, 421));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        splitter_2 = new QSplitter(tab);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setGeometry(QRect(60, 80, 97, 60));
        splitter_2->setOrientation(Qt::Vertical);
        rojo = new QCheckBox(splitter_2);
        rojo->setObjectName(QString::fromUtf8("rojo"));
        splitter_2->addWidget(rojo);
        verde = new QCheckBox(splitter_2);
        verde->setObjectName(QString::fromUtf8("verde"));
        splitter_2->addWidget(verde);
        azul = new QCheckBox(splitter_2);
        azul->setObjectName(QString::fromUtf8("azul"));
        splitter_2->addWidget(azul);
        layoutWidget = new QWidget(tab);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(540, 50, 171, 243));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        Knob = new QwtKnob(layoutWidget);
        Knob->setObjectName(QString::fromUtf8("Knob"));
        Knob->setUpperBound(1.000000000000000);
        Knob->setValue(0.500000000000000);
        Knob->setSingleSteps(0u);
        Knob->setPageSteps(10u);

        gridLayout->addWidget(Knob, 1, 1, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        colorWheel = new color_widgets::ColorWheel(layoutWidget);
        colorWheel->setObjectName(QString::fromUtf8("colorWheel"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(colorWheel->sizePolicy().hasHeightForWidth());
        colorWheel->setSizePolicy(sizePolicy);

        gridLayout->addWidget(colorWheel, 0, 1, 1, 1);

        gridLayoutWidget = new QWidget(tab);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(60, 222, 241, 51));
        grid_modo = new QGridLayout(gridLayoutWidget);
        grid_modo->setSpacing(6);
        grid_modo->setContentsMargins(11, 11, 11, 11);
        grid_modo->setObjectName(QString::fromUtf8("grid_modo"));
        grid_modo->setContentsMargins(0, 0, 0, 0);
        PWM_GPIO = new QCheckBox(gridLayoutWidget);
        PWM_GPIO->setObjectName(QString::fromUtf8("PWM_GPIO"));
        PWM_GPIO->setIconSize(QSize(16, 16));
        PWM_GPIO->setTristate(false);

        grid_modo->addWidget(PWM_GPIO, 0, 0, 2, 1);

        plainTextEdit = new QPlainTextEdit(gridLayoutWidget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setFrameShape(QFrame::NoFrame);

        grid_modo->addWidget(plainTextEdit, 0, 1, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        ADCButton = new QPushButton(tab_2);
        ADCButton->setObjectName(QString::fromUtf8("ADCButton"));
        ADCButton->setGeometry(QRect(260, 170, 75, 23));
        layoutWidget1 = new QWidget(tab_2);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(160, 30, 271, 131));
        gridLayout_2 = new QGridLayout(layoutWidget1);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        lcdCh1 = new QLCDNumber(layoutWidget1);
        lcdCh1->setObjectName(QString::fromUtf8("lcdCh1"));
        lcdCh1->setFrameShape(QFrame::Box);
        lcdCh1->setFrameShadow(QFrame::Plain);
        lcdCh1->setSmallDecimalPoint(false);
        lcdCh1->setDigitCount(5);
        lcdCh1->setSegmentStyle(QLCDNumber::Flat);
        lcdCh1->setProperty("value", QVariant(0.000000000000000));

        gridLayout_2->addWidget(lcdCh1, 0, 0, 1, 1);

        lcdCh3 = new QLCDNumber(layoutWidget1);
        lcdCh3->setObjectName(QString::fromUtf8("lcdCh3"));
        lcdCh3->setFrameShape(QFrame::Box);
        lcdCh3->setFrameShadow(QFrame::Plain);
        lcdCh3->setSmallDecimalPoint(false);
        lcdCh3->setDigitCount(5);
        lcdCh3->setSegmentStyle(QLCDNumber::Flat);
        lcdCh3->setProperty("value", QVariant(0.000000000000000));

        gridLayout_2->addWidget(lcdCh3, 0, 1, 1, 1);

        lcdCh2 = new QLCDNumber(layoutWidget1);
        lcdCh2->setObjectName(QString::fromUtf8("lcdCh2"));
        lcdCh2->setFrameShape(QFrame::Box);
        lcdCh2->setFrameShadow(QFrame::Plain);
        lcdCh2->setSmallDecimalPoint(false);
        lcdCh2->setDigitCount(5);
        lcdCh2->setSegmentStyle(QLCDNumber::Flat);
        lcdCh2->setProperty("value", QVariant(0.000000000000000));

        gridLayout_2->addWidget(lcdCh2, 1, 0, 1, 1);

        lcdCh4 = new QLCDNumber(layoutWidget1);
        lcdCh4->setObjectName(QString::fromUtf8("lcdCh4"));
        lcdCh4->setFrameShape(QFrame::Box);
        lcdCh4->setFrameShadow(QFrame::Plain);
        lcdCh4->setSmallDecimalPoint(false);
        lcdCh4->setDigitCount(5);
        lcdCh4->setSegmentStyle(QLCDNumber::Flat);
        lcdCh4->setProperty("value", QVariant(0.000000000000000));

        gridLayout_2->addWidget(lcdCh4, 1, 1, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        tabWidget->addTab(tab_3, QString());
        splitter_4 = new QSplitter(MainUserGUI);
        splitter_4->setObjectName(QString::fromUtf8("splitter_4"));
        splitter_4->setGeometry(QRect(220, 10, 641, 22));
        splitter_4->setOrientation(Qt::Horizontal);
        splitter = new QSplitter(splitter_4);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        serialPortLabel = new QLabel(splitter);
        serialPortLabel->setObjectName(QString::fromUtf8("serialPortLabel"));
        serialPortLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        splitter->addWidget(serialPortLabel);
        serialPortComboBox = new QComboBox(splitter);
        serialPortComboBox->setObjectName(QString::fromUtf8("serialPortComboBox"));
        splitter->addWidget(serialPortComboBox);
        splitter_4->addWidget(splitter);
        splitter_3 = new QSplitter(splitter_4);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setOrientation(Qt::Horizontal);
        runButton = new QPushButton(splitter_3);
        runButton->setObjectName(QString::fromUtf8("runButton"));
        splitter_3->addWidget(runButton);
        pingButton = new QPushButton(splitter_3);
        pingButton->setObjectName(QString::fromUtf8("pingButton"));
        splitter_3->addWidget(pingButton);
        splitter_4->addWidget(splitter_3);
        splitter_5 = new QSplitter(MainUserGUI);
        splitter_5->setObjectName(QString::fromUtf8("splitter_5"));
        splitter_5->setGeometry(QRect(10, 440, 691, 18));
        splitter_5->setOrientation(Qt::Horizontal);
        pushButton = new QPushButton(splitter_5);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        splitter_5->addWidget(pushButton);
        statusLabel = new QLabel(splitter_5);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        splitter_5->addWidget(statusLabel);
        QWidget::setTabOrder(serialPortComboBox, pingButton);
        QWidget::setTabOrder(pingButton, runButton);

        retranslateUi(MainUserGUI);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainUserGUI);
    } // setupUi

    void retranslateUi(QWidget *MainUserGUI)
    {
        MainUserGUI->setWindowTitle(QCoreApplication::translate("MainUserGUI", "GUIPanel", nullptr));
        rojo->setText(QCoreApplication::translate("MainUserGUI", "Rojo", nullptr));
        verde->setText(QCoreApplication::translate("MainUserGUI", "Verde", nullptr));
        azul->setText(QCoreApplication::translate("MainUserGUI", "Azul", nullptr));
        label->setText(QCoreApplication::translate("MainUserGUI", "Color", nullptr));
        label_2->setText(QCoreApplication::translate("MainUserGUI", "Brillo", nullptr));
        PWM_GPIO->setText(QString());
        plainTextEdit->setPlainText(QCoreApplication::translate("MainUserGUI", "chequeado PWM \n"
"No chequeado GPIO", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainUserGUI", "LEDs", nullptr));
        ADCButton->setText(QCoreApplication::translate("MainUserGUI", "LeeADC", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainUserGUI", "ADC", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainUserGUI", "Otro tab", nullptr));
        serialPortLabel->setText(QCoreApplication::translate("MainUserGUI", "Puerto Serie:", nullptr));
        runButton->setText(QCoreApplication::translate("MainUserGUI", "Inicio", nullptr));
        pingButton->setText(QCoreApplication::translate("MainUserGUI", "Ping", nullptr));
        pushButton->setText(QCoreApplication::translate("MainUserGUI", "Estado:", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainUserGUI", "Detenido", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainUserGUI: public Ui_MainUserGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINUSERGUI_H
