#include <QSet>

#include "../common/workingdirectory/WorkingDirectoryManager.h"

#include "Functions.h"

#include "functions/FunctionAbstract.h"

const QString Functions::SETTINGS_KEY_BASE{"Functions_"};
Functions::Functions(const QString &templateId, QObject *parent)
    : QAbstractListModel(parent)
{
    m_settingsKey = SETTINGS_KEY_BASE + templateId;
    const auto &allFunctions = FunctionAbstract::allFunctions();
    for (const auto &allFunction : allFunctions)
    {
        m_functionInfos << FunctionInfo{
                           allFunction->id()
                           , allFunction->name()
                           , allFunction->description()
                           , true
    };
    }
    _loadFromSettings();
}

int Functions::rowCount(const QModelIndex &parent) const
{
    return m_functionInfos.size();
}

QVariant Functions::data(const QModelIndex &index, int role) const
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

bool Functions::setData(const QModelIndex &index, const QVariant &value, int role)
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

Qt::ItemFlags Functions::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
}

void Functions::_loadFromSettings()
{
    auto settings = WorkingDirectoryManager::instance()->settingsLocalIfClient();
    if (settings->contains(m_settingsKey))
    {
        QSet<QString> checkedIds = settings->value(m_settingsKey).value<QSet<QString>>();
        for (auto &functionInfo : m_functionInfos)
        {
            functionInfo.checked = checkedIds.contains(functionInfo.id);
        }
    }
}

void Functions::_saveInSettings()
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
