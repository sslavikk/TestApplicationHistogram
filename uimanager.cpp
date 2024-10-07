#include "uimanager.h"

UiManager::UiManager(QObject *parent) : QObject(parent)
{

}

void UiManager::setPlot(CustomPlotItem *item)
{
    _plot = item;
}

void UiManager::setPath(QString path)
{
    if (!path.isEmpty())
        _currentPath = path.remove("file:///");
    qDebug()<<path;
}

QString UiManager::getPath()
{
    return _currentPath;
}

void UiManager::startParse(QString path)
{
     qDebug()<<path;
    if (path.isEmpty())
    {
        emit showWarning(tr("Choose a path"));
    }
    QThread* thread = new QThread();
    FileReader* reader = new FileReader(path);
    reader->moveToThread(thread);
    connect(reader, &FileReader::changeStatus,this,&UiManager::changeStatus);
    connect(reader, &FileReader::appendData,_plot,&CustomPlotItem::onAddData);
    connect(thread,&QThread::started,reader,&FileReader::onParseFile);
    connect(reader,&FileReader::parsedEnd,thread,&QThread::quit);
    connect(reader,&FileReader::showWarning,this,&UiManager::showWarning);
    thread->start();
}


