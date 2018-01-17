#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>


#include <QMainWindow>
#include <QTimer>

#include <QNetworkReply>
#include <QLabel>
#include <QGridLayout>
#include "weatherforecast.h"


QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class Ui_ThemeWidgetForm;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_CHARTS_USE_NAMESPACE

class ThemeWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ThemeWidget(QWidget *parent = 0);
    ~ThemeWidget();

private Q_SLOTS:
    void updateUI();

private:
    DataTable generateRandomData(int listCount, int valueMax, int valueCount) const;
    void connectSignals();
    QChart *createPieChart() const;
    QChart *createLineChart() const;

private:
    int m_listCount;

    int m_valueCount;
    QList<QChartView *> m_charts;
    DataTable m_dataTable;

    Ui_ThemeWidgetForm *m_ui;

//mystuff
    QTimer *clockTimer;
    weatherforecast weatherForecast;

    QTimer* weatherTimer;
    QNetworkAccessManager* weatherManager;
    QVector<QLabel*> weatherLabelVector;

    int m_valueMax;
    int m_valueMin;
    QTimer* sensorTimer;
    QTimer* quoteTimer;
    QNetworkAccessManager* quoteManager;
    //QLabel* quoteLabel;
    DataTable initData();
    void initChart();
    void TestFunction();
private slots:
    void quoteReplyFinished(QNetworkReply *reply);
    void weatherReplyFinished(QNetworkReply *reply);
    void processGetWeather();
    void processGetQuote();
    void processGetSensor();
    void processSetClock();
};

#endif /* THEMEWIDGET_H */
