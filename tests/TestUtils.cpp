/*
	Copyright (c) 2017-2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#include "TestUtils.h"
#include <utils/Utils.h>
#include <QDebug>

void TestUtils::runTest() {
    QString str1 = "str str str", str2 = "strstrstr";

    qDebug() << (Utils::lastIndexBefore(str1, ' ', -1) == 7);
    qDebug() << (Utils::lastIndexBefore(str1, ' ', 0) == -1);
    qDebug() << (Utils::lastIndexBefore(str1, ' ', 1) == -1);
    qDebug() << (Utils::lastIndexBefore(str1, ' ', 2) == -1);
    qDebug() << (Utils::lastIndexBefore(str1, ' ', 3) == 3);
    qDebug() << (Utils::lastIndexBefore(str1, ' ', 4) == 3);
    qDebug() << (Utils::lastIndexBefore(str1, ' ', 6) == 3);
    qDebug() << (Utils::lastIndexBefore(str1, ' ', 10) == 7);

    qDebug() << (Utils::lastIndexBefore(str2, ' ', -1) == -1);
    qDebug() << (Utils::lastIndexBefore(str2, ' ', 3) == -1);
    qDebug() << (Utils::lastIndexBefore(str2, ' ', 10) == -1);


    qDebug() << (Utils::splitMessageTo(str1, 1).count() == 9);
    qDebug() << (Utils::splitMessageTo(str1, 3).count() == 3);
    qDebug() << (Utils::splitMessageTo(str1, 20).count() == 1);

    qDebug() << (Utils::splitMessageTo(str2, 1).count() == 9);
    qDebug() << (Utils::splitMessageTo(str2, 3).count() == 3);
    qDebug() << (Utils::splitMessageTo(str2, 20).count() == 1);
}
