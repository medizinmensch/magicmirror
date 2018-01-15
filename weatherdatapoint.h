#ifndef WEATHERDATAPOINT_H
#define WEATHERDATAPOINT_H
#include <QString>

class weatherdatapoint
{
public:
    weatherdatapoint();
    //~weatherdatapoint(); activate if needed

    QString time;
    QString description;
    double temperature;
    double humidity;
    double windSpeed;      // meter/sec?
    int windDirection;  //
    int clouds;
    int timeUnixUtc;

    // http://openweathermap.org/img/w/ICONNAME.png
};

#endif // WEATHERDATAPOINT_H
