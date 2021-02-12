#ifndef QCANDLESTICKCHART_H
#define QCANDLESTICKCHART_H
#include <QtWidgets>
#include <QPainter>
#include <QtCharts>
#include <QtCharts/QCandlestickSeries>
#include <QChartView>
#include <QLineSeries>
#include <vector>

class QCandlestickChart : public QWidget
{
    Q_OBJECT

    public:
        explicit QCandlestickChart(
            std::string data_file,
            std::string title_series,
            std::string title_chart,
            QWidget *parent = 0);

        ~QCandlestickChart();
        
        QChartView *get_ChartView();
        
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

#endif // QCANDLESTICKCHART_H