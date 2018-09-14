/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#include <interfaces/Model.h>


void from_json(const nlohmann::json &j, QString &p) {
    p = QString::fromStdString(j);
}

void to_json(nlohmann::json &j, const QString &p) {
    j = p.toStdString();
}
