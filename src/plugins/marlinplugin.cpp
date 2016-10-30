#include "marlinplugin.h"
#include <QLoggingCategory>
#include <QString>

QString MarlinPlugin::_ok = QStringLiteral("ok");
QString MarlinPlugin::_wait = QStringLiteral("wait");
QString MarlinPlugin::_extruderTemp = QStringLiteral("T:");
QString MarlinPlugin::_bedTemp = QStringLiteral("B:");

Q_LOGGING_CATEGORY(MARLIN_PLUGIN, "org.kde.atelier.core.firmware.marlin");

QString MarlinPlugin::name() const
{
    return QStringLiteral("Marlin");
}

MarlinPlugin::MarlinPlugin()
{
    qCDebug(MARLIN_PLUGIN) << name() << " plugin loaded!";
}

void MarlinPlugin::extractTemp(const QString &lastMessage)
{
    // ok T:185.4 /185.0 B:60.5 /60.0
    QStringList list = lastMessage.split(QChar::fromLatin1(' '));
    // T:185.4 - current temperature
    printerStatus.extruderTemp = list[0].mid(2).toFloat();
    // /185.0 - target temperature
    printerStatus.extruderTargetTemp = list[1].mid(1).toFloat();
    // B:185.4 - current temperature
    printerStatus.bedTemp = list[2].mid(2).toFloat();
    // /60.0 - target temperature
    printerStatus.bedTargetTemp = list[3].mid(1).toFloat();

    emit(printerStatusChanged(printerStatus));
}

bool MarlinPlugin::validateCommand(const QString &lastMessage)
{
    if (lastMessage.contains(_extruderTemp) || lastMessage.contains(_bedTemp)) {
        extractTemp(lastMessage);
    } else if (lastMessage.contains(_ok) || lastMessage.contains(_wait)) {
        return true;
    }
    return false;
}

QByteArray MarlinPlugin::translate(const QString &command)
{
    return command.toLocal8Bit();
}

bool MarlinPlugin::readyForNextCommand(const QString &lastMessage)
{
    return validateCommand(lastMessage);
}