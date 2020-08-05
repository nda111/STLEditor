/********************************************************************************
** Form generated from reading UI file 'STLWidgetHp9984.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef STLWIDGETHP9984_H
#define STLWIDGETHP9984_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include "qvtkopenglnativewidget.h"

QT_BEGIN_NAMESPACE

class Ui_STLWidget
{
public:

    void setupUi(QVTKOpenGLNativeWidget *STLWidget)
    {
        if (STLWidget->objectName().isEmpty())
            STLWidget->setObjectName(QStringLiteral("STLWidget"));
        STLWidget->resize(400, 300);

        retranslateUi(STLWidget);

        QMetaObject::connectSlotsByName(STLWidget);
    } // setupUi

    void retranslateUi(QVTKOpenGLNativeWidget *STLWidget)
    {
        STLWidget->setWindowTitle(QApplication::translate("STLWidget", "STLWidget", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class STLWidget: public Ui_STLWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // STLWIDGETHP9984_H
