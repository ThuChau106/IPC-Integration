#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QtCore>
#include <QThread>
#include "MediaAppMain.h"

class MyThread : public QThread
{

public:
    MyThread();

    void run();
    void getApp(MediaAppMain* appP);

    QString name;
    MediaAppMain* app;

};

#endif // MYTHREAD_H
