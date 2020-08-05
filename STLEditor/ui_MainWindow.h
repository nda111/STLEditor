/********************************************************************************
** Form generated from reading UI file 'MainWindowp28140.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOWP28140_H
#define MAINWINDOWP28140_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

#include "STLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionUndo;
    QAction *actionRedo;
    QWidget *centralWidget;
    STLWidget *stlWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuAction;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);

        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));

        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QStringLiteral("actionUndo"));

        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QStringLiteral("actionRedo"));

        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));

        stlWidget = new STLWidget(centralWidget);
        stlWidget->setObjectName(QStringLiteral("openGLWidget"));
        stlWidget->setGeometry(QRect(0, 0, 641, 571));
        stlWidget->setAutoFillBackground(false);

        MainWindow->setCentralWidget(centralWidget);

        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 21));

        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));

        menuAction = new QMenu(menuBar);
        menuAction->setObjectName(QStringLiteral("menuAction"));

        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuAction->menuAction());
        menuFile->addAction(actionOpen);
        menuAction->addAction(actionUndo);
        menuAction->addAction(actionRedo);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", Q_NULLPTR));
        actionUndo->setText(QApplication::translate("MainWindow", "Undo", Q_NULLPTR));
        actionRedo->setText(QApplication::translate("MainWindow", "Redo", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        menuAction->setTitle(QApplication::translate("MainWindow", "Action", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOWP28140_H
