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
#include <QCoreApplication>
#include <QDebug>

class ExampleGadget
{
    Q_GADGET
    Q_PROPERTY(bool prop1 MEMBER prop1)
    Q_PROPERTY(int prop2 MEMBER prop2)
    Q_PROPERTY(qreal prop3 MEMBER prop3)
    Q_PROPERTY(QString prop4 MEMBER prop4)

public:
    bool prop1 = false;
    int prop2 = 0;
    qreal prop3 = 0.0;
    QString prop4;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    {
        ExampleGadget gadget { true, 1, 3.14, "toJson" };
        QJsonObject json = Gadget::toJson<ExampleGadget>(&gadget);
        qDebug() << json; // QJsonObject({"prop1":true,"prop2":1,"prop3":3.14,"prop4":"toJson"})
    }

    {
        QJsonObject json {
            {"prop1", false},
            {"prop2", 2},
            {"prop3", 6.18},
            {"prop4", "fromJson"}
        };
        ExampleGadget gadget = Gadget::fromJson<ExampleGadget>(json);
        qDebug() << gadget.prop1 << gadget.prop2 << gadget.prop3 << gadget.prop4; // false 2 6.18 "fromJson"
    }
}

#include "example.moc"
