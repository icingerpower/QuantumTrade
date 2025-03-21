#include "../common/workingdirectory/WorkingDirectoryManager.h"

#include "model/pairs/Tick.h"

#include "TemplateParams.h"

const QString TemplateParams::SETTINGS_KEY_BASE{"TemplateParams_"};

const QString TemplateParams::PARAM_DAYS_DATA{"daysTraining"};
const QString TemplateParams::PARAM_DAYS_LIFESPAN{"daysLifespan"};
const QString TemplateParams::PARAM_TICK{"tick"};

TemplateParams::TemplateParams(const QString &templateId, QObject *parent)
    : QAbstractTableModel(parent)
{
    m_settingsKey = SETTINGS_KEY_BASE + templateId;
    _initParams();
    _loadFromSettings();
}

QVariant TemplateParams::getValue(const QString &paramId) const
{
    return m_params[m_idToParamIndex[paramId]].value;
}

void TemplateParams::_initParams()
{
    m_params << ParamInfo{
        PARAM_DAYS_DATA,
        tr("Days training"),
        tr("Number of days for training the model"),
        1500
    };
    m_params << ParamInfo{
        PARAM_DAYS_LIFESPAN,
        tr("Days lifespan"),
        tr("Number of days as lifespan before a new model needs to be regenerated"),
        10
    };
    m_params << ParamInfo{
        PARAM_TICK,
            tr("Tick"),
            tr("Tick used by the model"),
            QVariant::fromValue(&Tick::TICK_DAY_1)
    };
    for (int i=0; i<m_params.size(); ++i)
    {
        m_idToParamIndex[m_params[i].id] = i;
    }
}

void TemplateParams::_loadFromSettings()
{
    auto settings = WorkingDirectoryManager::instance()->settingsLocalIfClient();
    const auto &value = settings->value(m_settingsKey);
    const auto &idValues = value.value<QHash<QString, QVariant>>();
    for (auto it = idValues.begin(); it != idValues.end(); ++it)
    {
        if (m_idToParamIndex.contains(it.key()))
        {
            int rowIndex = m_idToParamIndex[it.key()];
            m_params[rowIndex].value = it.value();
        }
    }
}

void TemplateParams::_saveInSettings()
{
    auto settings = WorkingDirectoryManager::instance()->settingsLocalIfClient();
    QHash<QString, QVariant> idValues;
    for (const auto &paramInfo : m_params)
    {
        idValues[paramInfo.id] = paramInfo.value;
    }
    settings->setValue(m_settingsKey, QVariant::fromValue(idValues));
}

QVariant TemplateParams::headerData(
    int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        static QStringList header{tr("Param"), tr("Value")};
        return header[section];
    }
    return QVariant{};
}

int TemplateParams::rowCount(const QModelIndex &parent) const
{
    return m_params.size();
}

int TemplateParams::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant TemplateParams::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (index.column() == 0)
        {
           return m_params[index.row()].name;
        }
        else
        {
           return m_params[index.row()].value;
        }
    }
    else if (role == Qt::ToolTipRole)
    {
        return m_params[index.row()].description;
    }
    return QVariant();
}

bool TemplateParams::setData(
    const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole && m_params[index.row()].value != value)
    {
        m_params[index.row()].value = value;
        _saveInSettings();
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags TemplateParams::flags(
    const QModelIndex &index) const
{
    if (index.column() == 1)
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
