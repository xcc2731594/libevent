#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <typeinfo>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QTcpSocket *mSocket;
public slots:
    void on_clicked();
    void on_readdata();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
