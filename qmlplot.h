#ifndef QMLPLOT_H
#define QMLPLOT_H
#include "CustomPlot/qcustomplot.h"
#include <QtQuick>
#include "defines.h"
class QCustomPlot;
class QCPAbstractPlottable;


class CustomPlotItem : public QQuickPaintedItem
{
    Q_OBJECT

public:
    CustomPlotItem( QQuickItem* parent = 0 );
    virtual ~CustomPlotItem();

    void paint( QPainter* painter );

    Q_INVOKABLE void initCustomPlot();
public slots:
    Q_INVOKABLE void onAddData(QVector<DataGraph> data);
protected:
    void routeMouseEvents( QMouseEvent* event );
    void routeWheelEvents( QWheelEvent* event );

    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void mouseDoubleClickEvent( QMouseEvent* event );
    virtual void wheelEvent( QWheelEvent *event );

    //virtual void timerEvent(QTimerEvent *event);

private:
    QCustomPlot*         m_CustomPlot;
    int                  m_timerId;
    QList<QCPBars*> m_bars;
    QList<QCPItemText*> m_texts;
    QSharedPointer<QCPAxisTickerText> _ticker;
    QString _minKey;
    QVector<DataGraph> _dataWords;
    void startSettingBar(QCPBars* bar);
private slots:
    void graphClicked( QCPAbstractPlottable* plottable );
    void onCustomReplot();
    void updateCustomPlotSize();

};

#endif // QMLPLOT_H
