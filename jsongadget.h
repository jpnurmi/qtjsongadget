/*
 * Copyright (c) 2020 J-P Nurmi <jpnurmi@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef JSONGADGET_H
#define JSONGADGET_H

#include <QtCore/qjsonobject.h>
#include <QtCore/qjsonvalue.h>
#include <QtCore/qloggingcategory.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/qvariant.h>

Q_LOGGING_CATEGORY(lcJsonGadget, "jsongadget", QtWarningMsg)

namespace Gadget
{
template <typename T>
static T fromJson(const QJsonObject &json)
{
    T gadget;
    const QMetaObject *metaObject = &T::staticMetaObject;
    int propertyCount = metaObject->propertyCount();
    for (int i = 0; i < propertyCount; ++i) {
        QMetaProperty property = metaObject->property(i);
        if (!property.isWritable() || !property.isStored())
            continue;
        QVariant value = json.value(QString::fromLatin1(property.name())).toVariant();
        if (!value.isValid())
            qCWarning(lcJsonGadget) << "unsupported property" << property.name() << property.typeName();
        else if (!property.writeOnGadget(&gadget, value))
            qCWarning(lcJsonGadget) << "failed to write property" << property.name() << value;
    }
    return gadget;
}

template <typename T>
static QJsonObject toJson(const void *gadget)
{
    if (!gadget)
        return QJsonObject();

    QJsonObject json;
    const QMetaObject *metaObject = &T::staticMetaObject;
    int propertyCount = metaObject->propertyCount();
    for (int i = 0; i < propertyCount; ++i) {
        QMetaProperty property = metaObject->property(i);
        if (!property.isReadable() || !property.isStored())
            continue;
        QVariant value = property.readOnGadget(gadget);
        if (value.isValid())
            json.insert(QString::fromLatin1(property.name()), QJsonValue::fromVariant(value));
        else
            qCWarning(lcJsonGadget) << "failed to read property" << property.name();
    }
    return json;
}
} // Gadget

#endif // JSONGADGET_H
