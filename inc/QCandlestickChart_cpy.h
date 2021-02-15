#ifndef QCANDLESTICKCHART_H
#define QCANDLESTICKCHART_H
#include <QChartView>
#include <QLineSeries>
#include <QPainter>
#include <QtCharts/QCandlestickSeries>
#include <QtCharts>
#include <QtWidgets>
#include <vector>

class QCandlestickChart : public QChart {
    Q_OBJECT

public:
    explicit QCandlestickChart(QWidget *parent = 0);
    ~QCandlestickChart();

    QChartView *chartView();
    QString m_data_file;
    QString m_title_series;
    QString m_title_chart;

private:
    QCandlestickSeries *m_stock_series;
    QChart *m_chart;
    QChartView *m_chart_view;
    QStringList m_categories;

    void create_CandlestickSeries();
    void create_ChartView();
    int cur_width;
};

QCandlestickChart::QCandlestickChart(QWidget *parent)
{
    this->m_stock_series = new QCandlestickSeries();
    this->m_chart = new QChart();

    this->m_chart_view = new QChartView(this->m_chart);
}

QCandlestickChart::~QCandlestickChart()
{
    delete this->m_stock_series;
    delete this->m_chart;
    delete this->m_chart_view;
}

void QCandlestickChart::create_CandlestickSeries()
{
    this->m_stock_series = new QCandlestickSeries();
    this->m_stock_series->setIncreasingColor(QColor(Qt::green));
    this->m_stock_series->setDecreasingColor(QColor(Qt::red));
    this->m_stock_series->setName(this->m_title_series);

    QFile stock_data(this->m_data_file);
    if (!stock_data.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString line;
    QString delim = ",";
    QStringList data;

    std::vector<QStringList> list;

    QTextStream in(&stock_data);
    while (!in.atEnd()) {
        line = in.readLine();
        if (line.startsWith("t" || line.isEmpty()))
            continue;
        data = line.split(delim);
        list.push_back(data);
    }

    std::vector<QStringList>::reverse_iterator it = list.rbegin();
    for (; it != list.rend(); ++it) {
        data = *it;

        QDateTime date(QDate::fromString(data.at(0), "yyyy-MM-dd"));

        const qreal time_stamp = date.toSecsSinceEpoch();
        const qreal open = data.at(1).toDouble();
        const qreal high = data.at(2).toDouble();
        const qreal low = data.at(3).toDouble();
        const qreal close = data.at(4).toDouble();

        QCandlestickSet *set = new QCandlestickSet(time_stamp);
        set->setOpen(open);
        set->setHigh(high);
        set->setLow(low);
        set->setClose(close);

        this->m_stock_series->append(set);
        this->m_categories << date.toString("yyyy-MM-dd");
    }
}

void QCandlestickChart::create_ChartView()
{
    this->m_chart = new QChart();
    this->m_chart->addSeries(this->m_stock_series);
    this->m_chart->setTitle(this->m_title_chart);
    this->m_chart->setAnimationOptions(QChart::SeriesAnimations);

    this->m_chart->createDefaultAxes();
    QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis *>(
        this->m_chart->axes(Qt::Horizontal).at(0));
    axisX->setCategories(this->m_categories);
    axisX->setLabelsAngle(-90);

    QValueAxis *axisY =
        qobject_cast<QValueAxis *>(this->m_chart->axes(Qt::Vertical).at(0));
    axisY->setMax(axisY->max() * 1.01);
    axisY->setMin(axisY->min() * 0.99);

    this->m_chart->legend()->setVisible(true);
    this->m_chart->legend()->setAlignment(Qt::AlignBottom);

    this->m_chart_view = new QChartView(this->m_chart);
    this->m_chart_view->setRenderHint(QPainter::Antialiasing);
    this->m_chart_view->setRubberBand(QChartView::HorizontalRubberBand);
}

QChartView *QCandlestickChart::getChartView() { return this->m_chart_view; }

#endif // QCANDLESTICKCHART_H