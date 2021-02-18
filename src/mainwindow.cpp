#include "../inc/mainwindow.h"
#include "../inc/QCandlestickChart.h"
#include "../inc/mainwindow_ui.h"
#include <QChartView>
#include <Qdebug>

// MainWindow Constructor for stocker application
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // Import ui from QT Designer
    ui->setupUi(this);
    this->setMouseTracking(true);
    std::string symbol = "GME";
    std::string api_key = avapi::readFirstLineFromFile("../../api.key");
    avapi::Quote quote(symbol, api_key);
    avapi::time_series series = quote.getTimeSeries(avapi::DAILY, 30);

    QCandlestickChartView *chart_view = new QCandlestickChartView();
    chart_view->addAvapiSeries(series, QString::fromStdString("GME Daily"),
                               avapi::DAILY);
    chart_view->chart()->setTitle(QString::fromStdString("GME Last 20 Days"));
    chart_view->chart()->setAnimationOptions(QChart::SeriesAnimations);
    chart_view->setRenderHint(QPainter::Antialiasing);
    chart_view->setRubberBand(QChartView::HorizontalRubberBand);
    chart_view->setMouseTracking(true);
    this->setCentralWidget(chart_view);
}

// MainWindow Deconstructor
MainWindow::~MainWindow() { delete ui; }