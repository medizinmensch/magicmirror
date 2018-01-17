#include "themewidget.h"
#include "ui_themewidget.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QApplication>
#include <QtCharts/QValueAxis>
#include <QTimer>
#include <QtCharts>
#include <QtNetwork>

ThemeWidget::ThemeWidget(QWidget *parent) : QWidget(parent), m_ui(new Ui_ThemeWidgetForm)
{
    // Timer, sodass alle paar sekunden / minuten das Wetter aktuallisiert wird (processGetWeather)
    weatherTimer = new QTimer(this);
    connect(weatherTimer, SIGNAL(timeout()), this, SLOT(processGetWeather()));
    weatherTimer->start(0);


    quoteTimer = new QTimer(this);
    connect(quoteTimer, SIGNAL(timeout()), this, SLOT(processGetQuote()));
    quoteTimer->start(0);

    clockTimer = new QTimer(this);
    connect(clockTimer, SIGNAL(timeout()), this, SLOT(processSetClock()));
    clockTimer->start(0);

    sensorTimer = new QTimer(this);
    connect(sensorTimer, SIGNAL(timeout()), this, SLOT(processGetSensor()));
    sensorTimer->start(0);

    m_ui->setupUi(this);

    // Set the colors from the light theme as default ones
    QPalette pal = qApp->palette();
    //pal.setColor(QPalette::Shadow, QRgb(0x0000FF));
    qApp->setPalette(pal);

}

void ThemeWidget::initChart(){
    QChartView *chartView;

    chartView = new QChartView(createLineChart());
    m_ui->gridLayout->addWidget(chartView, 1, 2);
    m_charts << chartView;
}

ThemeWidget::~ThemeWidget()
{
    delete m_ui;
}


void ThemeWidget::TestFunction(){

}


// LineWidget benutzt daten von "dataTable", hier wird "diese ge"dataTable gefüllt
DataTable ThemeWidget::initData()
{
    DataTable dataTable;

    DataList dataList;
    qreal yValue(0);
    m_valueMin=1000;
    for (int j(0); j < weatherForecast.dataCount; j++) {

        yValue = weatherForecast.data.at(j).temperature;

        if(yValue > m_valueMax){
            m_valueMax = yValue;
        }
        if(yValue < m_valueMin){
            m_valueMin = yValue;
        }

        QPointF value(j*3,yValue);
        QString label = "Slice " + QString::number(j);
        //QString label = weatherForecast.data.at(j).time;
        dataList << Data(value, label);
    }
    dataTable << dataList;

    return dataTable;
}

QChart *ThemeWidget::createLineChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Line chart");

    QString name("Temperature ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable) {
        QLineSeries *series = new QLineSeries(chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name);
        nameIndex++;
        chart->addSeries(series);
    }


     // TODO set Range mit int m_valueMax und m_valueCount;
    chart->createDefaultAxes();
    //chart->axisX()->setR
    chart->axisX()->setRange(QVariant(0), QVariant(40*3));
    chart->axisY()->setRange(m_valueMin - 3, m_valueMax + 3);

    // Add space to label to add space between labels and axis
    static_cast<QValueAxis *>(chart->axisY())->setLabelFormat("%.1f  ");

    return chart;
}

void ThemeWidget::updateUI()
{

    QChart::ChartTheme theme = QChart::ChartThemeDark;
    const auto charts = m_charts;

    for (QChartView *chartView : charts) {
        chartView->chart()->setTheme(theme);
    }
    QPalette pal = window()->palette();
    pal.setColor(QPalette::Window, QRgb(0x121218));
    pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
    window()->setPalette(pal);
    /*
    if (!m_charts.isEmpty() && m_charts.at(0)->chart()->theme() != theme) {
        for (QChartView *chartView : charts) {
            chartView->chart()->setTheme(theme);
        }
        QPalette pal = window()->palette();
        pal.setColor(QPalette::Window, QRgb(0x121218));
        pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
        window()->setPalette(pal);
    }
*/
    // Set antialiasing
    for (QChartView *chart : charts)
        chart->setRenderHint(QPainter::Antialiasing, true);

    // Set animation options
    for (QChartView *chartView : charts)
        chartView->chart()->setAnimationOptions(QChart::AllAnimations);
    // Update legend alignment
    if (!Qt::AlignBottom) {
        for (QChartView *chartView : charts)
            chartView->chart()->legend()->hide();
    } else {
        for (QChartView *chartView : charts) {
            chartView->chart()->legend()->setAlignment(Qt::AlignBottom);
            chartView->chart()->legend()->show();
        }
    }
}

// Sensoren auslesen
void ThemeWidget::processGetWeather(){
    if (sensorTimer->interval() == 0){

    }
    sensorTimer->setInterval(6000);
}
// Uhr updaten
void ThemeWidget::processSetClock(){
    clockTimer->setInterval(1000);
    int hour = QTime::currentTime().hour();
    QString hourString;
    if(hour<10){
        hourString = "0";
    }
    hourString += QString::number(hour);
    int minute = QTime::currentTime().minute();
    QString minuteString;
    if(minute<10){
        minuteString = "0";
    }
    minuteString += QString::number(minute);
    int second = QTime::currentTime().second();
    QString secondString;
    if(second<10){
        secondString = "0";
    }
    secondString += QString::number(second);
    m_ui->clockLabel->setText(hourString +":"+minuteString+":"+secondString);
}
// Wetter-json downloaden
void ThemeWidget::processGetWeather(){
    weatherTimer->setInterval(6000);
    weatherManager = new QNetworkAccessManager(this);
    connect(weatherManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(weatherReplyFinished(QNetworkReply*)));
    QUrl url("http://api.openweathermap.org/data/2.5/forecast?id=2950159&APPID=85f8e39c1f41443aed236ccc25d0a34a&units=metric");
    QNetworkRequest request(url);
    weatherManager->get(request);
}

// json downloaden (benutz ich später)
void ThemeWidget::processGetQuote(){
    quoteTimer->setInterval(86400000- QTime::currentTime().msecsSinceStartOfDay());
    quoteManager = new QNetworkAccessManager(this);
    connect(quoteManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(quoteReplyFinished(QNetworkReply*)));
    QUrl url("http://taeglicheszit.at/zitat-api.php?format=json");
    QNetworkRequest request(url);
    quoteManager->get(request);
}

// json reply parsen (benutz ich später
void ThemeWidget::quoteReplyFinished(QNetworkReply *reply){
    if(reply->error())
        m_ui->quoteLabel->setText("ERROR!!");
    else
    {
        QJsonObject jsonObject = QJsonDocument::fromJson(reply->readAll()).object();
        m_ui->quoteLabel->setText(jsonObject["zitat"].toString());
        //m_ui->quoteLabel->setText("Dies ist ein sehr sehrsehr sehrsehr sehrsehr sehr sehr  sehr sehr  sehr sehr  sehr sehr sehr sehr  sehr sehr  sehr sehr  sehr sehr sehr sehr  sehr sehr  sehr sehr  sehr sehr sehrsehr sehr langes Zitat.");
        reply->deleteLater();
    }
}

// json reply parsen
void ThemeWidget::weatherReplyFinished(QNetworkReply *reply){
    if(reply->error()){
        QLabel *outData = new QLabel("Error reading Weather");
        outData->setStyleSheet("QLabel { color : white; }");
        weatherLabelVector.clear();
        weatherLabelVector.push_back(outData);
        //weatherGrid->addWidget(weatherLabelVector.last(),0,0,1,1);
        weatherManager->clearAccessCache();
    }else{
        QJsonObject jsonObject = QJsonDocument::fromJson(reply->readAll()).object();
        QJsonObject jasonObjectCity = jsonObject["city"].toObject();
        weatherForecast.cityName = jasonObjectCity["name"].toString();
        weatherForecast.dataCount = jsonObject["cnt"].toInt();

        QJsonArray jsonArray = jsonObject["list"].toArray();
        int counter = 0;
        foreach (const QJsonValue & value, jsonArray){
            QJsonObject jasonObjectList = value.toObject();
            weatherdatapoint tmp;
            tmp.timeUnixUtc = jasonObjectList["dt"].toInt();
            tmp.time = jasonObjectList["dt_txt"].toString();

            QJsonObject jasonObjectListMain = jasonObjectList["main"].toObject();
            tmp.temperature = jasonObjectListMain["temp"].toDouble();
            tmp.humidity = jasonObjectListMain["humidity"].toDouble();

            QJsonArray QJsonArrayListWeather = jasonObjectList["weather"].toArray();
            QJsonObject jasonObjectListWeather = QJsonArrayListWeather[0].toObject();
            tmp.description = jasonObjectListWeather["description"].toString();

            QJsonObject jasonObjectListClouds = jasonObjectList["clouds"].toObject();
            tmp.clouds = jasonObjectListClouds["all"].toInt();

            QJsonObject jasonObjectListWind = jasonObjectList["wind"].toObject();
            tmp.windSpeed = jasonObjectListWind["speed"].toDouble();
            tmp.windDirection = jasonObjectListWind["deg"].toInt();

            weatherForecast.data.push_back(tmp);

            QLabel *outData = new QLabel("data");
            outData->setStyleSheet("QLabel { color : white; }");
            outData->setText(tmp.time + " Description:"+tmp.description+" Temperature: "+QString::number(tmp.temperature)+"°C Humidity: "+QString::number(tmp.humidity)+"% clouds:"+QString::number(tmp.clouds)+"% Windspeed:"+QString::number(tmp.windSpeed)+"m/s Windirection"+QString::number(tmp.windDirection)+"°");
            weatherLabelVector.push_back(outData);
            //weatherGrid->addWidget(weatherLabelVector.last(),counter,0,1,1);
            counter++;
        }
        weatherManager->clearAccessCache();
        reply->deleteLater();
    }
    m_dataTable = initData();
    initChart();
    updateUI();
}
