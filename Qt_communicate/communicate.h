#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

class Communicate : public QObject
{
    Q_OBJECT
public:
    explicit Communicate(QObject *parent = nullptr);
    void writeData(const QJsonObject &json);

signals:

public slots:
    void readData();


private:
    QTcpSocket *s;

};

#endif // COMMUNICATE_H
