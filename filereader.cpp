#include "filereader.h"
const QByteArray Separator("\r\n");
FileReader::FileReader(QString path, QObject *parent) : QObject(parent),_path(path)
{
//qDebug()<<_path;
}

void FileReader::onParseFile()
{
    QFile file(_path);
    if (!file.exists())
    {
        emit showWarning(tr("File is missing"));
        return;
    }
    if (!file.open(QIODevice::WriteOnly))
    {
        emit showWarning(tr("File is not open"));
        return;
    }
    QStringList list = {"first",
                       "second",
                        "third",
                        "fours",
                        "fifth",
                        "sixth",
                        "seventh",
                        "eigth",
                        "ninth",
                        "ten",
                        "eleven",
                        "twelve",
                        "thirteen ",
                        "fourteen ",
                        "fifteen ",
                        "sixteen ",
                        "seventeen ",
                        "eighteen ",
                        "nineteen ",
                        "111",
                        "222",
                        "333",
                       };
    QByteArray arr;
    qsrand(QDateTime::currentMSecsSinceEpoch());
    int high = list.length()-1;
    int low = 0;
    for (int i=0;i<1000;i++)
    {
        int val = (qrand() % ((high + 1) - low) + low);
        file.write(QString("%1%2").arg(list.at(val)).arg(QString(Separator)).repeated(5).toUtf8());
    }
    file.close();
    if (!file.open(QIODevice::ReadOnly))
    {
        emit showWarning(tr("File is not open"));
        return;
    }
    QByteArray buffer;
    QFileInfo info(file);
    auto sizeFile = info.size();
    qint64 currentSize = 0;
    int step = info.size()/100;
    if (step>100000)
        step = 100000;
    while (!file.atEnd())
    {
        QMap<QString,int> map;
        QByteArray buf = file.read(step);
        currentSize +=buf.size();
        buffer.append(buf);
        while(buffer.contains(Separator))
        {
            QString str = buffer.mid(0,buffer.indexOf(Separator));
            str = str.simplified();
            buffer.remove(0,buffer.indexOf(Separator)+Separator.length());
            if (!str.isEmpty())
            {
                if (!map.contains(str))
                {
                    map[str]=0;
                }
                map[str] = map[str]+1;
            }
        }

        emit changeStatus(currentSize*100/sizeFile);
        QVector<DataGraph> arr;
        foreach (QString key, map.keys())
        {
            if (_map.contains(key))
                _map[key]+=map.value(key);
            else _map[key] = map.value(key);
        }
        foreach (QString key, _map.keys())
        {
            arr.append(DataGraph(_map.value(key),key));
        }
        if (arr.length()>1)
        for (int i=1;i<arr.length();i++)
        {
            if (i>0)
            if (arr[i].value>arr[i-1].value)
            {
                DataGraph data = arr[i-1];
                arr[i-1] = arr[i];
                arr[i] = data;
                i--;
                i--;
            }
        }
        emit appendData(arr);
        this->thread()->msleep(10);//пауза для вдиимости реального добавления данных
    }
    emit parsedEnd();
}
