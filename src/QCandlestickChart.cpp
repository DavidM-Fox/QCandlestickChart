#include "../inc/QCandlestickChart.h"
#include <QDebug>

// QCandlestickChartView class constructor
QCandlestickChartView::QCandlestickChartView(QWidget *parent)
    : QChartView(new QChart(), parent),
      m_functions({QString::fromStdString("MM-dd | hh:mm"),
                   QString::fromStdString("yyyy-MM-dd"),
                   QString::fromStdString("yyyy-MM-dd"),
                   QString::fromStdString("yyyy-MM")})
{
}

// QCandlestickChartView class deconstructor
QCandlestickChartView::~QCandlestickChartView() {}

// Add an avapi::time_series to this chart
void QCandlestickChartView::addAvapiSeries(avapi::time_series &series,
                                           const QString &title,
                                           const avapi::function &func)
{
    this->m_series = new QCandlestickSeries();
    this->m_series->setIncreasingColor(QColor(Qt::green));
    this->m_series->setDecreasingColor(QColor(Qt::red));
    this->m_series->setName(title);
    QString date_format = this->m_functions[func];

    for (auto it = series.begin(); it != series.end(); ++it) {
        const qreal timestamp = it->first;
        QCandlestickSet *set =
            new QCandlestickSet(it->second[0], it->second[1], it->second[2],
                                it->second[3], timestamp);
        this->m_series->append(set);
        this->m_categories << QDateTime::fromSecsSinceEpoch(timestamp).toString(
            date_format);
    }

    chart()->addSeries(this->m_series);
    setupChartAxes();
    connect(this->m_series, SIGNAL(hovered(bool, QCandlestickSet *)), this,
            SLOT(sltTooltip(bool, QCandlestickSet *)));
}

// Create default axes and legend for chart
void QCandlestickChartView::setupChartAxes()
{
    chart()->createDefaultAxes();
    QBarCategoryAxis *axisX =
        qobject_cast<QBarCategoryAxis *>(chart()->axes(Qt::Horizontal).at(0));
    axisX->setCategories(this->m_categories);
    axisX->setLabelsAngle(-90);

    QValueAxis *axisY =
        qobject_cast<QValueAxis *>(chart()->axes(Qt::Vertical).at(0));
    axisY->setMax(axisY->max() * 1.01);
    axisY->setMin(axisY->min() * 0.99);

    chart()->legend()->setVisible(true);
    chart()->legend()->setAlignment(Qt::AlignBottom);
}

// Slot function for when hovering over a candlestick set within the
// chartview. Shows a tooltip at cursor position with the set's data.
void QCandlestickChartView::sltTooltip(bool status, QCandlestickSet *set)
{
    if (this->m_tooltip == nullptr) {
        this->m_tooltip = new QLabel(this);
        this->m_tooltip->setStyleSheet(
            "background: rgba(51,51,51,185);"
            "color: rgb(255, 255, 255);"
            "border:0px groove gray;border-radius:10px;padding:2px 4px;"
            "border:2px groove gray;border-radius:10px;padding:2px 4px;");
        this->m_tooltip->hide();
    }
    if (status) {
        const QPoint cursor_g = QCursor::pos();
        const QPoint cursor_w = this->mapFromGlobal(cursor_g);
        const int x = cursor_w.x();
        const int y = cursor_w.y();
        this->m_tooltip->setText(
            QDateTime::fromSecsSinceEpoch(set->timestamp())
                .toString("yyyy-MM-dd | hh:mm\n") +
            "Open: " + QString::number(set->open()) + "\n" +
            "High: " + QString::number(set->high()) + "\n" +
            "Low: " + QString::number(set->low()) + "\n" +
            "Close: " + QString::number(set->close()));
        this->m_tooltip->show();
        this->m_tooltip->move(cursor_w);
    }
    else {
        this->m_tooltip->hide();
    }
}