#include "vpnlogger.h"

#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QTextStream>

#include "ticonfmain.h"

vpnLogger::vpnLogger(QObject *parent) : QObject(parent)
{
    logMapper = new QSignalMapper(this);
    loggers = QMap<QString, QProcess*>();

    connect(logMapper, SIGNAL(mapped(QString)), this, SLOT(log(QString)));
}

void vpnLogger::addVPN(const QString &name, QProcess *proc)
{
    qInfo() << "add logger" << tiConfMain::main_config;
    loggers.insert(name, proc);
    connect(proc, SIGNAL(readyReadStandardError()), logMapper, SLOT(map()));
    connect(proc, SIGNAL(readyReadStandardOutput()), logMapper, SLOT(map()));
    logMapper->setMapping(proc, name);
}

void vpnLogger::log(const QString &name)
{
    QProcess *proc = loggers[name];

    qInfo() << "log::path::jajaja" << main_settings.getValue("paths/logs").toString();

    QFile logfile(QString("%1/vpn/%2.log").arg(tiConfMain::formatPath(main_settings.getValue("paths/logs").toString()), name));
    logfile.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&logfile);

    out << QString::fromUtf8(proc->readAll());
    logfile.flush();
    logfile.close();
}

void vpnLogger::process()
{

}
