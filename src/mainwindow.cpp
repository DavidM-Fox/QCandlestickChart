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
    avapi::time_series series = quote.getIntradaySeries("15min");

    QCandlestickChartView *chart_view = new QCandlestickChartView();
    chart_view->setChartTitle(QString::fromStdString("GME Intraday"));
    chart_view->addChartSeries(series, QString::fromStdString("GME"));

    this->setCentralWidget(chart_view);
}

// MainWindow Deconstructor
MainWindow::~MainWindow() { delete ui; }