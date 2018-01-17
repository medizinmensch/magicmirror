#ifndef WEATHERFORECAST_H
#define WEATHERFORECAST_H
#include <QVector>
#include "weatherdatapoint.h"
#include <QDesktopWidget>
#include <QDesktopWidget>
#include <QMainWindow>

/*
 *
#include <QDesktopWidget>
#include <QMainWindow>
#include <QApplication>
*/
class weatherforecast
{
public:
    weatherforecast();
    //~weatherforecast();activate if needed

    QString cityName;
    int dataCount;
    int minTemp;
    int maxTemp;
    QVector<weatherdatapoint> data;
};

#endif // WEATHERFORECAST_H
