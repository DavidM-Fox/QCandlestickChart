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

    void addAvapiSeries(avapi::time_series &a_series, const QString &title,
                        const avapi::function &func);

    void setViewDefaults();
    void setChartDefaults();

    void setChartTitle(const QString &title);
    void setDefaultChartAxes();
    void setDefaultChartLegend();

private slots:
    void sltTooltip(bool status, QCandlestickSet *set);

private:
    static std::vector<QString> m_functions;
    QCandlestickSeries *m_series;
    QStringList m_categories;
    QLabel *m_tooltip = nullptr;
};
#endif // QCANDLESTICKCHART_H