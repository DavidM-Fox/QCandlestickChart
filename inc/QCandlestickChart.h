#ifndef QCANDLESTICKCHART_H
#define QCANDLESTICKCHART_H
#include "avapi.h"
#include <QtCharts>
#include <vector>

class QCandlestickChartView : public QChartView {
    Q_OBJECT

public:
    explicit QCandlestickChartView(QWidget *parent = 0);
    ~QCandlestickChartView();

    void addAvapiSeries(avapi::time_series &series, const QString &title,
                        const avapi::function &func);
    void setupChartAxes();

private slots:
    void sltTooltip(bool status, QCandlestickSet *set);

private:
    QCandlestickSeries *m_series;
    std::vector<QString> m_functions;
    QStringList m_categories;
    QLabel *m_tooltip = nullptr;
};
#endif // QCANDLESTICKCHART_H