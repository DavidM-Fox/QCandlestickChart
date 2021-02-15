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
    std::string symbol = "GME";
    std::string api_key = avapi::readFirstLineFromFile("../../api.key");
    avapi::Quote quote(symbol, api_key);
    avapi::time_series series = quote.getIntradaySeries("15min");

    QCandlestickChart *chart = new QCandlestickChart();
    chart->setTitle(QString::fromStdString("GME Intraday"));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->addAvapiSeries(series, QString::fromStdString("GME"));

    QChartView *chart_view = new QChartView(chart);
    chart_view->setRenderHint(QPainter::Antialiasing);
    chart_view->setRubberBand(QChartView::HorizontalRubberBand);
    chart_view->setMouseTracking(true);

    ui->gridLayout->addWidget(chart_view, 0, 0);
}

// MainWindow Deconstructor
MainWindow::~MainWindow() { delete ui; }