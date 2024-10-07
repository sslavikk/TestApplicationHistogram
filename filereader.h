#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QThread>
#include "defines.h"
#include <QDateTime>
class FileReader : public QObject
{
    Q_OBJECT
public:
    explicit FileReader(QString path,QObject *parent = nullptr);

public slots:
    void onParseFile();

private:
    QString _path;
    QVector<DataGraph> _data;
    QMap<QString,int> _map;
signals:

    void changeStatus(double value);
    void appendData(QVector<DataGraph> data);
    void showWarning(QString err);
    void parsedEnd();
};

#endif // FILEREADER_H
