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

#include "jsongadget.h"
#include <QtTest>

class TestGadget
{
    Q_GADGET
    Q_PROPERTY(bool boolean MEMBER boolean)
    Q_PROPERTY(int integer MEMBER integer)
    Q_PROPERTY(qreal real MEMBER real)
    Q_PROPERTY(QString string MEMBER string)
    Q_PROPERTY(QVariantList array MEMBER array)
    Q_PROPERTY(QVariantMap object MEMBER object)
    Q_PROPERTY(QUrl url MEMBER url)
    Q_PROPERTY(QUuid uuid MEMBER uuid)
    Q_PROPERTY(void *ignored MEMBER ignored STORED false)

public:
    bool boolean = false;
    int integer = 0;
    qreal real = 0.0;
    QString string;
    QVariantList array;
    QVariantMap object;
    QUrl url;
    QUuid uuid;
    void *ignored = nullptr;
};

class TestCase : public QObject
{
    Q_OBJECT

private slots:
    void toJson()
    {
        TestGadget gadget { true, 1, 3.14, "gadget", {1,2,3}, {{"a",1}, {"b",2}, {"c",3}}, QUrl("www.qt.io"), "30423278-7d5c-4ebf-880c-fd019fd22493" };

        QJsonObject json = Gadget::toJson<TestGadget>(&gadget);
        QCOMPARE(json.value("boolean"), true);
        QCOMPARE(json.value("integer"), 1);
        QCOMPARE(json.value("real"), 3.14);
        QCOMPARE(json.value("string"), "gadget");
        QCOMPARE(json.value("array"), QJsonArray::fromVariantList({1, 2, 3}));
        QCOMPARE(json.value("object"), QJsonObject::fromVariantMap({{"a",1}, {"b",2}, {"c",3}}));
        QCOMPARE(json.value("url"), "www.qt.io");
        QCOMPARE(json.value("uuid"), "30423278-7d5c-4ebf-880c-fd019fd22493");
        QCOMPARE(json.value("ignored"), QJsonValue::Undefined);
    }

    void fromJson()
    {
        QJsonObject json {
            {"boolean", true},
            {"integer", 1},
            {"real", 3.14},
            {"string", "gadget"},
            {"array", QJsonArray::fromVariantList({1,2,3})},
            {"object", QJsonObject::fromVariantMap({{"a",1}, {"b",2}, {"c",3}})},
            {"url", "www.qt.io"},
            {"uuid", "30423278-7d5c-4ebf-880c-fd019fd22493"},
            {"ignored", 0xDEAD},
        };

        TestGadget gadget = Gadget::fromJson<TestGadget>(json);
        QCOMPARE(gadget.boolean, true);
        QCOMPARE(gadget.integer, 1);
        QCOMPARE(gadget.real, 3.14);
        QCOMPARE(gadget.string, "gadget");
        QCOMPARE(gadget.array, QVariantList({1, 2, 3}));
        QCOMPARE(gadget.object, QVariantMap({{"a",1}, {"b",2}, {"c",3}}));
        QCOMPARE(gadget.url, QUrl("www.qt.io"));
        QCOMPARE(gadget.uuid, QUuid("30423278-7d5c-4ebf-880c-fd019fd22493"));
        QCOMPARE(gadget.ignored, nullptr);
    }
};

QTEST_GUILESS_MAIN(TestCase)

#include "test.moc"
