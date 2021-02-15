#ifndef QCANDLESTICKCHART_H
#define QCANDLESTICKCHART_H
#include "avapi.h"
#include <QPainter>
#include <QtCharts>
#include <QtWidgets>
#include <algorithm>
#include <vector>

class QCandlestickChart : public QChart {
    Q_OBJECT

public:
    explicit QCandlestickChart(QWidget *parent = 0);
    ~QCandlestickChart();

    void addAvapiSeries(avapi::time_series &avapi_series, const QString &title);

private:
    QStringList m_series_xlabel;
};
#endif // QCANDLESTICKCHART_H