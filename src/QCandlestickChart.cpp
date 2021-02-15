#include "../inc/QCandlestickChart.h"

QCandlestickChart::QCandlestickChart(QWidget *parent) {}

QCandlestickChart::~QCandlestickChart() {}

void QCandlestickChart::addAvapiSeries(avapi::time_series &avapi_series,
                                       const QString &title)
{

    QCandlestickSeries *series = new QCandlestickSeries();
    series->setIncreasingColor(QColor(Qt::green));
    series->setDecreasingColor(QColor(Qt::red));
    series->setName(title);

    std::reverse(avapi_series.begin(), avapi_series.end());

    for (auto it = avapi_series.begin(); it != avapi_series.end(); ++it) {

        const qreal timestamp = it->first;
        const qreal open = it->second[0];
        const qreal high = it->second[1];
        const qreal low = it->second[2];
        const qreal close = it->second[3];

        QCandlestickSet *set = new QCandlestickSet(timestamp);
        set->setOpen(open);
        set->setHigh(high);
        set->setLow(low);
        set->setClose(close);
        series->append(set);

        this->m_series_xlabel
            << QDateTime::fromSecsSinceEpoch(timestamp).toString(
                   "yyyy-MM-dd hh:mm");
    }
    this->addSeries(series);

    this->createDefaultAxes();
    QBarCategoryAxis *axisX =
        qobject_cast<QBarCategoryAxis *>(this->axes(Qt::Horizontal).at(0));
    axisX->setCategories(this->m_series_xlabel);
    axisX->setLabelsAngle(-90);

    QValueAxis *axisY =
        qobject_cast<QValueAxis *>(this->axes(Qt::Vertical).at(0));
    axisY->setMax(axisY->max() * 1.01);
    axisY->setMin(axisY->min() * 0.99);

    this->setAcceptHoverEvents(true);
    this->legend()->setVisible(true);
    this->legend()->setAlignment(Qt::AlignBottom);
}