#include "../inc/mainwindow.h"
#include "../inc/QCandlestickChart.h"
#include "../inc/mainwindow_ui.h"

// MainWindow Constructor for stocker application
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // Import ui from QT Designer
    ui->setupUi(this);

    // Get a daily time series for TSLA from the last 20 days
    avapi::Quote quote("TSLA", avapi::readFirstLineFromFile("../../api.key"));
    avapi::time_series series = quote.getTimeSeries(avapi::DAILY, 20);

    // Creating the chart view
    QCandlestickChartView *chart_view = new QCandlestickChartView();
    chart_view->setChartTitle(QString::fromStdString("TSLA"));

    // Add the avapi::time_series and set defaults
    chart_view->addAvapiSeries(series, QString::fromStdString("Last 20 Days"),
                               avapi::DAILY);
    chart_view->setViewDefaults();
    chart_view->setChartDefaults();
    this->setCentralWidget(chart_view);
}

// MainWindow Deconstructor
MainWindow::~MainWindow() { delete ui; }