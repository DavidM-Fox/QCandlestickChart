#include "../inc/mainwindow.h"
#include "../inc/./ui_mainwindow.h"
#include "../inc/QCandlestickChart.h"
#include "../inc/stocker_api.hpp"

// MainWindow Constructor for stocker application
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // Import ui from QT Designer
    ui->setupUi(this);
    std::string data_file = "..\\..\\stock_data\\Weekly_GME.csv";
    std::string title_series = "GME";
    std::string title_chart = "Weekly Data";

    std::string data_file_b = "..\\..\\stock_data\\Monthly_GME.csv";
    std::string title_series_b = "GME";
    std::string title_chart_b = "Monthly Data";

    QCandlestickChart *chart_a =
        new QCandlestickChart(data_file, title_series, title_chart);

    QCandlestickChart *chart_b =
        new QCandlestickChart(data_file_b, title_series_b, title_chart_b);

    ui->gridLayout->addWidget(chart_a->get_ChartView(), 0, 0);
    ui->gridLayout->addWidget(chart_b->get_ChartView(), 0, 1);
}

// MainWindow Deconstructor
MainWindow::~MainWindow() { delete ui; }