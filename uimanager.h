#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
#include <QFileDialog>
#include <QApplication>
#include <QThread>
#include <QDebug>
#include "filereader.h"
#include "qmlplot.h"
class UiManager : public QObject
{
    Q_OBJECT

public:
    explicit UiManager(QObject *parent = nullptr);
    Q_INVOKABLE void setPlot(CustomPlotItem* item);
    Q_INVOKABLE void setPath(QString path);
    Q_INVOKABLE QString getPath();
    Q_INVOKABLE void startParse(QString path);
signals:
    Q_INVOKABLE void showWarning(QString str);
    Q_INVOKABLE void changeStatus(qreal value);

private:
    QString _currentPath;

    CustomPlotItem* _plot;
    int status;
};

#endif // UIMANAGER_H
