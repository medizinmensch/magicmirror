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

ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui_ThemeWidgetForm)
{

    m_listCount = 5;
    m_valueMax = 20;
    m_valueCount = 10;

    weatherTimer = new QTimer(this);
    connect(weatherTimer, SIGNAL(timeout()), this, SLOT(processGetWeather()));
    weatherTimer->start(100);


   // m_dataTable = generateRandomData(m_listCount, m_valueMax, m_valueCount);

    m_ui->setupUi(this);

    // Set defaults

    // Set the colors from the light theme as default ones
    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window, QRgb(0xffffff));
    pal.setColor(QPalette::WindowText, QRgb(0xffffff));
    pal.setColor(QPalette::Background, QRgb(0x000000));
    pal.setColor(QPalette::Highlight, QRgb(0xfe2ef7));
    //pal.setColor(QPalette::Light, QRgb(0x00FF00)); //gren
    pal.setColor(QPalette::Base, QRgb(0x00FF00)); //gren
    //pal.setColor(QPalette::Normal, QRgb(0xfe2ef7));
    //pal.setColor(QPalette::Window, QRgb(0x0000FF)); //blue
    pal.setColor(QPalette::Shadow, QRgb(0x0000FF)); //blue
    //pal.setColor();
    qApp->setPalette(pal);

    while(weatherForecast.data.count()==0){
        qDebug() << "loooping...";
    }

    //updateUI();
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

DataTable ThemeWidget::initData() const
{
    DataTable dataTable;

    DataList dataList;
    qreal yValue(0);
    for (int j(0); j < weatherForecast.dataCount; j++) {
        yValue = weatherForecast.data.at(j).temperature;
        QPointF value(j,yValue);
        QString label = "Slice " + QString::number(j);
        dataList << Data(value, label);
    }
    dataTable << dataList;

    return dataTable;
}

QChart *ThemeWidget::createLineChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Line chart");

    QString name("Series ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable) {
        QLineSeries *series = new QLineSeries(chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }

    chart->createDefaultAxes();
    chart->axisX()->setRange(0, m_valueMax);
    chart->axisY()->setRange(0, m_valueCount);

    // Add space to label to add space between labels and axis
    static_cast<QValueAxis *>(chart->axisY())->setLabelFormat("%.1f  ");

    return chart;
}

void ThemeWidget::updateUI()
{

    QChart::ChartTheme theme = QChart::ChartThemeDark;
    const auto charts = m_charts;

    if (!m_charts.isEmpty() && m_charts.at(0)->chart()->theme() != theme) {
        for (QChartView *chartView : charts) {
            chartView->chart()->setTheme(theme);
        }
        QPalette pal = window()->palette();
        pal.setColor(QPalette::Window, QRgb(0x121218));
        pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
        window()->setPalette(pal);
    }

    // Set antialiasing
    for (QChartView *chart : charts)
        chart->setRenderHint(QPainter::Antialiasing, true);

    // Set animation options
    for (QChartView *chartView : charts)
        chartView->chart()->setAnimationOptions(QChart::AllAnimations);


    /*
     *
     *      m_ui->legendComboBox->addItem("No Legend ", 0);
            m_ui->legendComboBox->addItem("Legend Top", Qt::AlignTop);
            m_ui->legendComboBox->addItem("Legend Bottom", Qt::AlignBottom);
            m_ui->legendComboBox->addItem("Legend Left", Qt::AlignLeft);
            m_ui->legendComboBox->addItem("Legend Right", Qt::AlignRight);

    */
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

void ThemeWidget::processGetWeather(){
    weatherTimer->setInterval(2000);
    weatherManager = new QNetworkAccessManager(this);
    connect(weatherManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(weatherReplyFinished(QNetworkReply*)));
    QUrl url("http://api.openweathermap.org/data/2.5/forecast?id=2950159&APPID=85f8e39c1f41443aed236ccc25d0a34a&units=metric");
    QNetworkRequest request(url);
    weatherManager->get(request);
}

void ThemeWidget::processGetQuote(){
    quoteTimer->setInterval(86400000- QTime::currentTime().msecsSinceStartOfDay());
    quoteManager = new QNetworkAccessManager(this);
    connect(quoteManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(quoteReplyFinished(QNetworkReply*)));
    QUrl url("http://taeglicheszit.at/zitat-api.php?format=json");
    QNetworkRequest request(url);
    quoteManager->get(request);
}

void ThemeWidget::quoteReplyFinished(QNetworkReply *reply){
    if(reply->error())
        quoteLabel->setText("ERROR!!");
    else
    {
        QJsonObject jsonObject = QJsonDocument::fromJson(reply->readAll()).object();
        quoteLabel->setText(jsonObject["zitat"].toString());
        reply->deleteLater();
    }
}

void ThemeWidget::weatherReplyFinished(QNetworkReply *reply){
    if(reply->error()){
        QLabel *outData = new QLabel("Error reading Weather");
        outData->setStyleSheet("QLabel { color : white; }");
        weatherLabelVector.clear();
        weatherLabelVector.push_back(outData);
        weatherGrid->addWidget(weatherLabelVector.last(),0,0,1,1);
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
            weatherGrid->addWidget(weatherLabelVector.last(),counter,0,1,1);
            counter++;
        }
        weatherManager->clearAccessCache();
        reply->deleteLater();
    }

    initChart();
    initData();
    updateUI();
}
