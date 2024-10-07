#include "qmlplot.h"

#include <QDebug>
#define MAXBARS 15
CustomPlotItem::CustomPlotItem(QQuickItem* parent) : QQuickPaintedItem(parent), m_CustomPlot(nullptr), m_timerId(0)
{
    setFlag(QQuickItem::ItemHasContents, true);
    setAcceptedMouseButtons(Qt::AllButtons);

    connect(this, &QQuickPaintedItem::widthChanged, this, &CustomPlotItem::updateCustomPlotSize);
    connect(this, &QQuickPaintedItem::heightChanged, this, &CustomPlotItem::updateCustomPlotSize);
}

CustomPlotItem::~CustomPlotItem()
{
    delete m_CustomPlot;
    m_CustomPlot = nullptr;

    if (m_timerId != 0)
    {
        killTimer(m_timerId);
    }
}
void CustomPlotItem::onAddData(QVector<DataGraph> data)
{
    if (data.isEmpty()) return;
    QVector<double> postions;
    QVector<QString> names;
    for (int i=0;i<data.length();i++)
    {
        if (i>MAXBARS-1) break;
        if (i>=m_bars.length())
        {
                        QCPBars* bars = new QCPBars(m_CustomPlot->xAxis, m_CustomPlot->yAxis);
                        startSettingBar(bars);
                        bars->setData(QVector<double>()<<(i * 2 + 1), QVector<double>()<< data.at(i).value, true);

                        //_ticker->addTick(i * 2 + 1, data.at(i).key);
                        m_bars.append( bars);
                        m_CustomPlot->xAxis->setRange(0, m_bars.length() * 2 + 2);
                        QCPItemText* text = new QCPItemText(m_CustomPlot);
                        QFont font("Tahoma");
                        font.setPixelSize(14);
                        text->setFont(font);
                        QPointF point = bars->dataPixelPosition(0);
                        point.setY(point.y()-text->font().pixelSize()/2);
                        text->position->setCoords(point);
                        text->setText(QString::number(bars->data()->at(0)->value));
                        text->position->setType(QCPItemPosition::PositionType::ptAbsolute);
                        text->setVisible(true);
                        m_texts.append(text);
        }
        else {
            m_bars[i]->setData(QVector<double>()<<(i * 2 + 1), QVector<double>()<< data.at(i).value, true);
            m_texts.at(i)->setText(QString::number(data.at(i).value));
            QPointF point = m_bars[i]->dataPixelPosition(0);
            point.setY(point.y()-m_texts.at(i)->font().pixelSize()/2);
            m_texts.at(i)->position->setCoords(point);
        }
        postions.append(i * 2 + 1);
        names.append(data.at(i).key);


    }
    if (m_CustomPlot->yAxis->range().upper < data.first().value)
        m_CustomPlot->yAxis->setRange(0, data.first().value * 1.1);
    _ticker->setTicks(postions,names);
        m_CustomPlot->replot(QCustomPlot::rpQueuedReplot);

//    foreach (QString key, data.keys())
//    {
//        QVector<double> fossilData1;
//        QVector<double> ticks1;
//        if (_dataWords.contains(key))
//        {
//            uint val = m_bars[key]->data()->at(0)->value + data.value(key).toUInt();
//            _dataWords[key] += data.value(key).toUInt();
//            if (m_CustomPlot->yAxis->range().upper < val)
//                m_CustomPlot->yAxis->setRange(0, val * 1.05);
//            //m_CustomPlot->rescaleAxes();
//            ticks1 << m_bars[key]->data()->at(0)->key;
//            fossilData1 << val;
//            m_bars[key]->setData(ticks1, fossilData1, true);

//            // m_bars[key].text->position->setCoords((double)m_bars[key].bar->dataPixelPosition(0).x(),50);
//            // qDebug()<<m_bars[key].bar->dataPixelPosition(0)<<key;
//        }
//        else
//        {
//            QCPBars* bars = new QCPBars(m_CustomPlot->xAxis, m_CustomPlot->yAxis);
//            startSettingBar(bars);
//            m_CustomPlot->xAxis->setRange(0, m_bars.keys().length() * 2 + 2);
//            ticks1 << m_bars.keys().length() * 2 + 1;
//            fossilData1 << data.value(key).toUInt();
//            bars->setData(ticks1, fossilData1, true);

//            _ticker->addTick(m_bars.keys().length() * 2 + 1, key);
//            m_bars.insert(key, bars);

//        }
//        if (_minKey.isEmpty()) _minKey = key;

//        if (m_bars[_minKey]->data()->at(0)->value > m_bars[key]->data()->at(0)->value)
//        {
//            if (!m_bars[_minKey]->visible()) m_bars[_minKey]->setVisible(true);
//            fossilData1.clear();
//            ticks1.clear();
//            ticks1<<m_bars[key]->data()->at(0)->key;
//            fossilData1<<m_bars[_minKey]->data()->at(0)->value;
//            m_bars[key]->setData(ticks1,fossilData1);
//            _minKey = key;
//            _ticker->ticks()[m_bars[key]->data()->at(0)->key]=key;
//        }
//        if (m_bars.count()>15)
//        {
//            qDebug()<<"setvisible"<<key<<_minKey;
//            //m_bars[_minKey]->setVisible(false);

//        }
//        m_CustomPlot->replot(QCustomPlot::rpQueuedReplot);
//    }
}
void CustomPlotItem::initCustomPlot()
{
    m_CustomPlot = new QCustomPlot();
    updateCustomPlotSize();
    m_CustomPlot->xAxis->setLabel("Name");
    m_CustomPlot->yAxis->setLabel("Value");
    m_CustomPlot->xAxis->setRange(0, 1);
    //m_CustomPlot->xAxis->setTickLabels(false);
    //m_CustomPlot->xAxis->setTicks(false);
    m_CustomPlot->yAxis->setRange(0, 5);
    m_CustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    _ticker = QSharedPointer<QCPAxisTickerText>(new QCPAxisTickerText);
    _ticker->setTickStepStrategy(QCPAxisTickerText::TickStepStrategy::tssReadability);
    m_CustomPlot->xAxis->setTicker(_ticker);

    connect(m_CustomPlot, &QCustomPlot::afterReplot, this, &CustomPlotItem::onCustomReplot);

    m_CustomPlot->replot();
}
void CustomPlotItem::startSettingBar(QCPBars* bar)
{
    if (bar)
    {
        bar->setAntialiased(false);
        bar->setWidth(1);
        bar->setPen(QPen(QColor(Qt::black).lighter(100)));
        bar->setBrush(QColor(Qt::green));
    }
}

void CustomPlotItem::paint(QPainter* painter)
{
    if (m_CustomPlot)
    {
        QPixmap picture(boundingRect().size().toSize());
        QCPPainter qcpPainter(&picture);

        m_CustomPlot->toPainter(&qcpPainter);
        setRenderTarget(QQuickPaintedItem::FramebufferObject);

        painter->drawPixmap(QPoint(), picture);
    }
}

void CustomPlotItem::mousePressEvent(QMouseEvent* event)
{
    qDebug() << Q_FUNC_INFO;
    routeMouseEvents(event);

}

void CustomPlotItem::mouseReleaseEvent(QMouseEvent* event)
{
    qDebug() << Q_FUNC_INFO;
    routeMouseEvents(event);
}

void CustomPlotItem::mouseMoveEvent(QMouseEvent* event) { routeMouseEvents(event); }

void CustomPlotItem::mouseDoubleClickEvent(QMouseEvent* event)
{
    qDebug() << Q_FUNC_INFO;
    routeMouseEvents(event);
}

void CustomPlotItem::wheelEvent(QWheelEvent* event) { routeWheelEvents(event); }

void CustomPlotItem::graphClicked(QCPAbstractPlottable* plottable)
{
    qDebug() << Q_FUNC_INFO << QString("Clicked on graph '%1 ").arg(plottable->name());
}

void CustomPlotItem::routeMouseEvents(QMouseEvent* event)
{
    if (m_CustomPlot)
    {
        QMouseEvent* newEvent =
            new QMouseEvent(event->type(), event->localPos(), event->button(), event->buttons(), event->modifiers());
        QCoreApplication::postEvent(m_CustomPlot, newEvent);
    }
}

void CustomPlotItem::routeWheelEvents(QWheelEvent* event)
{
    if (m_CustomPlot)
    {
        QWheelEvent* newEvent =
            new QWheelEvent(event->pos(), event->delta(), event->buttons(), event->modifiers(), event->orientation());
        QCoreApplication::postEvent(m_CustomPlot, newEvent);
    }
}

void CustomPlotItem::updateCustomPlotSize()
{
    if (m_CustomPlot)
    {

        m_CustomPlot->setGeometry(0, 0, (int)width(), (int)height());
        m_CustomPlot->setViewport(QRect(0, 0, (int)width(), (int)height()));
        for (int i=0;i<m_texts.length();i++)
        {
            //m_texts.at(i)->setText(QString::number(m_bars.at(i)->data()->at(0)->value));
            QPointF point = m_bars[i]->dataPixelPosition(0);
            point.setY(point.y()-m_texts.at(i)->font().pixelSize()/2);
            m_texts.at(i)->position->setCoords(point);
        }

    }
}

void CustomPlotItem::onCustomReplot()
{
    //qDebug() << Q_FUNC_INFO;

    update();
}
