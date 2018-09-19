/*
	Copyright (c) 2017-2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/
#include "TestHelper.h"

TestModel::TestModel(const QString &s) : string(s) {}

QString TestModel::toString() {
    return string;
}

