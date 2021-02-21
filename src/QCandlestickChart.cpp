#include "../inc/QCandlestickChart.h"

/**
 * @brief QCandlestickChartView class constructor
 * @param parent The parent of this widget
 */
QCandlestickChartView::QCandlestickChartView(QWidget *parent)
    : QChartView(new QChart(), parent)
{
}

/**
 * @brief QCandlestickChartView class deconstructor
 */
QCandlestickChartView::~QCandlestickChartView() {}

/**
 * @brief Add an avapi::time_series to the chart
 * @param a_series  The avapi::time_series
 * @param title     Title of the avapi::time_series
 * @param func      The type of avapi::time_series:
 * avapi::INTRADAY/DAILY/WEEKLY/MONTHLY
 */
void QCandlestickChartView::addAvapiSeries(avapi::time_series &a_series,
                                           const QString &title,
                                           const avapi::function &func)
{
    // Create new QCandlestickSeries object and set title
    QCandlestickSeries *series = new QCandlestickSeries();
    series->setName(title);

    // Set series inc/dec colors to a default green/red
    series->setIncreasingColor(QColor(Qt::green));
    series->setDecreasingColor(QColor(Qt::red));

    // Parse each data row of a_series into a new QCandlestickSet and append the
    // set to the previously created QCandlestickSeries
    for (auto it = a_series.begin(); it != a_series.end(); ++it) {
        const qreal timestamp = it->first;

        // a_series.second = std::vector<float>[open, high, low, close]
        QCandlestickSet *set =
            new QCandlestickSet(it->second[0], it->second[1], it->second[2],
                                it->second[3], timestamp);
        series->append(set);

        // Set category label for this set (String from timestamp)
        this->m_categories << QDateTime::fromSecsSinceEpoch(timestamp).toString(
            this->m_functions[func]);
    }

    chart()->addSeries(series);

    // Connect a hovered signal from the series to the SLOT function
    // sltTooltip()
    connect(series, SIGNAL(hovered(bool, QCandlestickSet *)), this,
            SLOT(sltTooltip(bool, QCandlestickSet *)));
}

/**
 * @brief Sets default RenderHint(Antialiasing) and RubberBand(Horizontal) for
 * this QChartView Object.
 */
void QCandlestickChartView::setViewDefaults()
{
    setRenderHint(QPainter::Antialiasing);
    setRubberBand(QChartView::HorizontalRubberBand);
}

/**
 * @brief Sets default AnimationOptions(SeriesAnimations) and default chart
 * axes/legend
 */
void QCandlestickChartView::setChartDefaults()
{
    chart()->setAnimationOptions(QChart::SeriesAnimations);
    setDefaultChartAxes();
    setDefaultChartLegend();
}

/**
 * @brief Set the chart title to the given QString
 * @param title The title as a QString
 */
void QCandlestickChartView::setChartTitle(const QString &title)
{
    chart()->setTitle(title);
}

/**
 * @brief Sets default X and Y axes for the chart
 */
void QCandlestickChartView::setDefaultChartAxes()
{
    chart()->createDefaultAxes();

    QBarCategoryAxis *axisX =
        qobject_cast<QBarCategoryAxis *>(chart()->axes(Qt::Horizontal).at(0));
    axisX->setCategories(this->m_categories);
    axisX->setLabelsAngle(-90);

    QValueAxis *axisY =
        qobject_cast<QValueAxis *>(chart()->axes(Qt::Vertical).at(0));
    axisY->setLabelFormat("$%0.1f");
    axisY->setMax(axisY->max() * 1.01);
    axisY->setMin(axisY->min() * 0.99);
}

/**
 * @brief Sets a default legend for the chart
 */
void QCandlestickChartView::setDefaultChartLegend()
{
    chart()->legend()->setVisible(true);
    chart()->legend()->setAlignment(Qt::AlignBottom);
}

/**
 * @brief SLOT - Shows a tooltip at cursor position when hovering over a
 * QCandlestickSet
 * @param status    Status of hovered signal
 * @param set       QCandlestickSet of interest
 */
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
        // Display set info within tooltip
        this->m_tooltip->setText(
            QDateTime::fromSecsSinceEpoch(set->timestamp())
                .toString("yyyy-MM-dd | hh:mm\n") +
            "Open: " + QString::number(set->open()) + "\n" +
            "High: " + QString::number(set->high()) + "\n" +
            "Low: " + QString::number(set->low()) + "\n" +
            "Close: " + QString::number(set->close()));

        // Show and move tooltip to the cursor's position
        this->m_tooltip->show();
        this->m_tooltip->move(this->mapFromGlobal(QCursor::pos()));
    }
    else {
        // When no longer hovering over a set, hide tooltip
        this->m_tooltip->hide();
    }
}

std::vector<QString> QCandlestickChartView::m_functions = {
    QString::fromStdString("MM-dd | hh:mm"), QString::fromStdString("MM-dd"),
    QString::fromStdString("MM-dd"), QString::fromStdString("yyyy-MM")};