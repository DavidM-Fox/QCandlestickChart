#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "include/stocker_api.hpp"
#include <stack>

// MainWindow Constructor for stocker application
MainWindow::MainWindow(QWidget *parent) : 
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Import ui from QT Designer
    ui->setupUi(this);
    std::string output_file = "..\\..\\stock_data\\Weekly_GME.csv"; 
    set_chart(output_file);
}

// MainWindow Deconstructor
MainWindow::~MainWindow() { delete ui; }

// Chart .csv data
void MainWindow::set_chart(std::string output_file) 
{
    QtCharts::QCandlestickSeries *stock_series = new QCandlestickSeries();


    stock_series->setName("GME");
    stock_series->setIncreasingColor(QColor(Qt::green));
    stock_series->setDecreasingColor(QColor(Qt::red));

    QFile stock_data(QString::fromStdString(output_file));
    if (!stock_data.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString line;
    QString delim = ",";
    QStringList data;
    QStringList categories;

    std::stack <QStringList> list;

    QTextStream in(&stock_data);
    while(!in.atEnd()) 
    {
        line = in.readLine();
        if (line.startsWith("#" || line.isEmpty()))
            continue; 
        data = line.split(delim);
        list.push(data);
    }

    while(!list.empty())
    {
        data = list.top();
        list.pop();

        QDateTime date(QDate::fromString(data.at(0), "yyyy-MM-dd"));

        const qreal time_stamp  = date.toSecsSinceEpoch();
        const qreal open        = data.at(1).toDouble();
        const qreal high        = data.at(2).toDouble();
        const qreal low         = data.at(3).toDouble();
        const qreal close       = data.at(4).toDouble();

        QCandlestickSet *set = new QCandlestickSet(time_stamp);
        set->setOpen(open);
        set->setHigh(high);
        set->setLow(low);
        set->setClose(close);

        stock_series->append(set);
        categories << date.toString("yyyy-MM-dd");
    }

    QChart *chart = new QChart();
    chart->addSeries(stock_series);
    chart->setTitle("GME data");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->createDefaultAxes();

    QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis *>(chart->axes(Qt::Horizontal).at(0));
    axisX->setCategories(categories);
    axisX->setLabelsAngle(-90);

    QValueAxis *axisY = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
    axisY->setMax(axisY->max() * 1.01);
    axisY->setMin(axisY->min() * 0.99);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    this->setCentralWidget(chartView);

    std::cout << std::endl;
}