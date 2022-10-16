#include "communicate.h"

Communicate::Communicate(QObject *parent)
    : QObject{parent}
{
    s = new QTcpSocket;
    s->connectToHost(QHostAddress("127.0.0.1"),8000);

    connect(s,SIGNAL(readyRead()),this,SLOT(readData()));

}

void Communicate::writeData(const QJsonObject &json)
{
    QJsonDocument d(json);
    QByteArray sendData = d.toJson();
    int len = sendData.size();
    //先发送数据长度 再发送数据本身
    s->flush(); //保证数据立马发送
    s->write((char*)&len,sizeof(int));
    s->write(sendData);
}
void Communicate::readData()
{
    QByteArray data;
    //还有数据可读
    while(s->bytesAvailable())
    {
        data += s->readAll();
    }
    QJsonDocument dt = QJsonDocument::fromJson(data);
    if(dt.isNull())
        return ;
    QJsonObject obj = dt.object();
     //逻辑处理

}
