#include <QSet>

#include "../common/workingdirectory/WorkingDirectoryManager.h"

#include "Indicators.h"

#include "indicators/IndicatorAbstract.h"

const QString Indicators::SETTINGS_KEY_BASE{"Indicators_"};
Indicators::Indicators(const QString &templateId, QObject *parent)
    : QAbstractListModel(parent)
{
    m_settingsKey = SETTINGS_KEY_BASE + templateId;
    const auto &allIndicators = IndicatorAbstract::allIndicators();
    for (const auto &allIndicator : allIndicators)
    {
        m_functionInfos << IndicatorInfo{
                           allIndicator->id()
                           , allIndicator->name()
                           , allIndicator->description()
                           , true
    };
    }
    _loadFromSettings();
}

int Indicators::rowCount(const QModelIndex &parent) const
{
    return m_functionInfos.size();
}

QVariant Indicators::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return m_functionInfos[index.row()].name;
    }
    else if (role == Qt::CheckStateRole)
    {
        return m_functionInfos[index.row()].checked ? Qt::Checked : Qt::Unchecked;
    }
    if (!index.isValid())
    {
        return QVariant();
    }

    // FIXME: Implement me!
    return QVariant();
}

bool Indicators::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole && data(index, role) != value)
    {
        m_functionInfos[index.row()].checked = value.value<Qt::CheckState>() == Qt::Checked;
        _saveInSettings();
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags Indicators::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
}

void Indicators::_loadFromSettings()
{
    auto settings = WorkingDirectoryManager::instance()->settingsLocalIfClient();
    QSet<QString> checkedIds = settings->value(m_settingsKey).value<QSet<QString>>();
    for (auto &functionInfo : m_functionInfos)
    {
        functionInfo.checked = checkedIds.contains(functionInfo.id);
    }
}

void Indicators::_saveInSettings()
{
    auto settings = WorkingDirectoryManager::instance()->settingsLocalIfClient();
    QSet<QString> checkedIds;
    for (const auto &functionInfo : m_functionInfos)
    {
        if (functionInfo.checked)
        {
            checkedIds << functionInfo.id;
        }
    }
    if (checkedIds.size() > 0)
    {
        settings->setValue(m_settingsKey, QVariant::fromValue(checkedIds));
    }
    else if (settings->contains(m_settingsKey))
    {
        settings->remove(m_settingsKey);
    }
}
