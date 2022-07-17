#include "MyThread.h"
#include <QtCore>
#include <QDebug>

MyThread::MyThread()
{

}


void MyThread::getApp(MediaAppMain* appP)
{
    app = appP;
}
void MyThread::run()
{
    qDebug() << this->name << "running";

    //exec();

    //MediaAppMain* app = new MediaAppMain();
//    while (1)
    {

        app->rcvMsg(1);
        //this->msleep(1);
    }
}
