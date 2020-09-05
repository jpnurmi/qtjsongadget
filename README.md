# QtJsonGadget

See [QJsonValue::fromVariant()](https://doc.qt.io/qt-5/qjsonvalue.html#fromVariant)
for a full list of supported property types. Use `Q_PROPERTY(... STORED false)` to
ignore certain properties, if desired.

## Q_GADGET

```cpp
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
```

## toJson()

```cpp
ExampleGadget gadget { true, 1, 3.14, "toJson" };
QJsonObject json = Gadget::toJson<ExampleGadget>(&gadget);
qDebug() << json; // QJsonObject({"prop1":true,"prop2":1,"prop3":3.14,"prop4":"toJson"})
```

## fromJson()

```cpp
QJsonObject json {
    {"prop1", false},
    {"prop2", 2},
    {"prop3", 6.18},
    {"prop4", "fromJson"}
};

ExampleGadget gadget = Gadget::fromJson<ExampleGadget>(json);
qDebug() << gadget.prop1 << gadget.prop2 << gadget.prop3 << gadget.prop4; // false 2 6.18 "fromJson"
```
