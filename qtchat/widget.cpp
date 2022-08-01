#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,this,&Widget::on_clicked);
    mSocket = new QTcpSocket();
    mSocket->connectToHost("127.0.0.1",9002);
//    connect(mSocket,&QTcpSocket::readyRead,this,&Widget::on_readdata);
    connect(mSocket,SIGNAL(readyRead()),this,SLOT(on_readdata()));
}

Widget::~Widget()
{
    delete ui;
    delete mSocket;
}

void Widget::on_clicked()
{
    qDebug()<<"connect";
    QJsonObject jsonObj;
    QJsonDocument jsondoc;
    jsonObj.insert("cmd","register");
    jsonObj.insert("user","xcc");
    jsondoc.setObject(jsonObj);
    mSocket->write(jsondoc.toJson());

}

void Widget::on_readdata()
{
    qDebug()<<"-----";
    QJsonDocument s = QJsonDocument::fromJson(mSocket->readAll());
    QString t = typeid(s).name();
    qDebug()<<t;
}

