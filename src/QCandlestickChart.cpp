#include "../inc/QCandlestickChart.h"
#include <QDebug>

QCandlestickChartView::QCandlestickChartView(QWidget *parent)
    : QChartView(new QChart(), parent)
{
    this->setRenderHint(QPainter::Antialiasing);
    this->setRubberBand(QChartView::HorizontalRubberBand);
    this->setMouseTracking(true);

    this->m_series = new QCandlestickSeries();
    this->m_series->setIncreasingColor(QColor(Qt::green));
    this->m_series->setDecreasingColor(QColor(Qt::red));
    chart()->setAnimationOptions(QChart::SeriesAnimations);
}

QCandlestickChartView::~QCandlestickChartView() {}

void QCandlestickChartView::setChartTitle(const QString &title)
{
    chart()->setTitle(title);
}

// Add an avapi::time_series to this chart
void QCandlestickChartView::addChartSeries(avapi::time_series &series,
                                           const QString &title)
{
    this->m_series->setName(title);

    for (auto it = series.begin(); it != series.end(); ++it) {

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
        this->m_series->append(set);

        this->m_categories << QDateTime::fromSecsSinceEpoch(timestamp).toString(
            "yyyy-MM-dd hh:mm");
    }
    chart()->addSeries(this->m_series);

    chart()->createDefaultAxes();
    QBarCategoryAxis *axisX =
        qobject_cast<QBarCategoryAxis *>(chart()->axes(Qt::Horizontal).at(0));
    axisX->setCategories(this->m_categories);
    axisX->setLabelsAngle(-90);
    axisX->hide();

    QValueAxis *axisY =
        qobject_cast<QValueAxis *>(chart()->axes(Qt::Vertical).at(0));
    axisY->setMax(axisY->max() * 1.01);
    axisY->setMin(axisY->min() * 0.99);

    // chart()->setAcceptHoverEvents(true);
    chart()->legend()->setVisible(true);
    chart()->legend()->setAlignment(Qt::AlignBottom);

    connect(this->m_series, SIGNAL(hovered(bool, QCandlestickSet *)), this,
            SLOT(sltTooltip(bool, QCandlestickSet *)));
}

void QCandlestickChartView::sltTooltip(bool status, QCandlestickSet *set)
{
    if (this->m_tooltip == nullptr) {
        this->m_tooltip = new QLabel(this);
        this->m_tooltip->setStyleSheet(
            "background: rgba(51,51,51,185);"
            "color: rgb(255, 255, 255);"
            "border:0px groove gray;border-radius:10px;padding:2px 4px;"
            "border:2px groove gray;border-radius:10px;padding:2px 4px;");
        this->m_tooltip->setVisible(false);
    }
    if (status) {
        QPoint cursor_g = QCursor::pos();
        QPoint cursor_w = QWidget::mapFromGlobal(QCursor::pos());
        QString date_time = QDateTime::fromSecsSinceEpoch(set->timestamp())
                                .toString("yyyy-MM-dd | hh:mm\n");
        QString open = "Open: " + QString::number(set->open()) + "\n";
        QString high = "High: " + QString::number(set->high()) + "\n";
        QString low = "Low: " + QString::number(set->low()) + "\n";
        QString close = "Close: " + QString::number(set->close());
        this->m_tooltip->setText(date_time + open + high + low + close);
        this->m_tooltip->move(cursor_g.x(), cursor_g.y());
        // this->m_tooltip->move(cursor_w.x(), cursor_w.y());
        this->m_tooltip->show();

        qDebug() << "global: " << cursor_g.x() << cursor_g.y();
        qDebug() << "global: " << cursor_w.x() << cursor_w.y();
    }
    else {
        this->m_tooltip->hide();
    }
}