# QCandlestickChartView

A custom Qt5 widget written with C++ for displaying historical stock data in a candlestick format.

# Prerequisites
This widget requires my [avapi](https://github.com/DavidM-Fox/avapi) library to function along with its prerequisites as well.

# Example Usage
## Display daily stock data for a symbol of interest

```C++

#include "../inc/mainwindow.h"
#include "../inc/QCandlestickChart.h"
#include "../inc/mainwindow_ui.h"
#include <QChartView>

// MainWindow Constructor for stocker application
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // Import ui from QT Designer
    ui->setupUi(this);

    // Get a daily time series for TSLA from the last 20 days
    avapi::Quote quote("TSLA", avapi::readFirstLineFromFile("../../api.key"));
    avapi::time_series series = quote.getTimeSeries(avapi::DAILY, 20);

    QCandlestickChartView *chart_view = new QCandlestickChartView();
    chart_view->setRenderHint(QPainter::Antialiasing);
    chart_view->setRubberBand(QChartView::HorizontalRubberBand);

    chart_view->addAvapiSeries(series, QString::fromStdString("TSLA Last 20 Days"),
                               avapi::DAILY);
    chart_view->chart()->setTitle(QString::fromStdString("TSLA Daily"));
    chart_view->chart()->setAnimationOptions(QChart::SeriesAnimations);

    this->setCentralWidget(chart_view);
}

// MainWindow Deconstructor
MainWindow::~MainWindow() { delete ui; }

```
