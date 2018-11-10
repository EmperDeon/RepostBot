/*
	Copyright (c) 2018 by Ilya Barykin
	Released under the MIT License.
	See the provided LICENSE.TXT file for details.
*/

#ifdef JSON_APPEND_HEADER

#include <QtCore/QString>
#include <QtCore/QList>

#endif


#ifdef JSON_APPEND_JSON
#pragma clang diagnostic push
#pragma ide diagnostic ignored "CannotResolve"

bool contains(const QString &k) const {
    return has_key(k);
}

bool has_key(const QString &k) const {
    if (!is_object() && !is_null()) throw std::runtime_error("has_key called on non-object json");

    return find(k.toStdString()) != end();
}

template<typename ValueType>
bool has_value(ValueType v) const {
    return std::find(begin(), end(), v) != end();
}

bool has_value(const QString &v) const {
    return std::find(begin(), end(), v.toStdString()) != end();
}

QList <QString> keys() const {
    QList <QString> list;

    for (const auto &it : items()) {
        list << QString::fromStdString(it.key());
    }

    return list;
}

json &operator[](const QString &key) {
    return operator[](key.toStdString());
}

void eraseV(const value_type val) {
    for (const auto &it : items()) {
        if (it.value() == val) {
            if (is_array()) {
                erase(std::strtol(it.key().c_str(), nullptr, 10));
            } else {
                erase(it.key());
            }
            return;
        }
    }
}

void eraseAllV(const value_type val) {
    std::list <std::string> keys;

    for (const auto &it : items()) {
        if (it.value() == val) {
            keys.insert(keys.begin(), it.key());
        }
    }

    for (const auto &key : keys) {
        if (is_array()) {
            erase(std::strtol(key.c_str(), nullptr, 10));
        } else {
            erase(key);
        }
    }
}

size_type erase(const char *key) {
    // this erase only works for objects
    if (JSON_LIKELY(is_object())) {
        return m_value.object->erase(key);
    }

    JSON_THROW(type_error::create(307, "cannot use erase() with " + std::string(type_name())));
}

size_type erase(const QString &key) {
    erase(key.toUtf8().data());
}

QString dumpQ(int indent = -1) const {
    return QString::fromStdString(dump(indent));
}

// In Qt empty is for destroying
bool isEmpty() const {
    return empty();
}

template<typename ValueTypeCV, typename ValueType = detail::uncvref_t <ValueTypeCV>,
        detail::enable_if_t<
                not detail::is_basic_json<ValueType>::value and
                detail::has_from_json<basic_json_t, ValueType>::value and
                not detail::has_non_default_from_json<basic_json_t, ValueType>::value,
                int> = 0>
ValueType get(ValueType def) const noexcept(noexcept(
        JSONSerializer<ValueType>::from_json(std::declval<const basic_json_t &>(),
                                             std::declval<ValueType &>()))) {

    if (is_null()) {
        return def;
    }

    // we cannot static_assert on ValueTypeCV being non-const, because
    // there is support for get<const basic_json_t>(), which is why we
    // still need the uncvref
    static_assert(not std::is_reference<ValueTypeCV>::value,
                  "get() cannot be used with reference types, you might want to use get_ref()");
    static_assert(std::is_default_constructible<ValueType>::value,
                  "types must be DefaultConstructible when used with get()");

    ValueType ret;
    JSONSerializer<ValueType>::from_json(*this, ret);
    return ret;
}

template<typename ValueTypeCV, typename ValueType = detail::uncvref_t <ValueTypeCV>,
        detail::enable_if_t<not std::is_same<basic_json_t, ValueType>::value and
                            detail::has_non_default_from_json<basic_json_t, ValueType>::value,
                int> = 0>
ValueType get(ValueType def) const noexcept(noexcept(
        JSONSerializer<ValueTypeCV>::from_json(std::declval<const basic_json_t &>()))) {
    if (is_null()) {
        return def;
    }

    static_assert(not std::is_reference<ValueTypeCV>::value,
                  "get() cannot be used with reference types, you might want to use get_ref()");
    return JSONSerializer<ValueTypeCV>::from_json(*this);
}

#pragma clang diagnostic pop
#endif

