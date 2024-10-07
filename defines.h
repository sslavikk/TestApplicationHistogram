
#ifndef DEFINES_H
#define DEFINES_H
#include <QString>
#include <QtGlobal>

struct DataGraph{
    int value;
    QString key;
    DataGraph(int val, QString s)
    {
        value = val;
        key = s;
    }
    DataGraph()
    {
        value = 0;
        key = "";
    }
};

#endif // DEFINES_H
