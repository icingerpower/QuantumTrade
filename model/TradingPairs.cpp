#include "../common/workingdirectory/WorkingDirectoryManager.h"

#include "readers/StreamReaderAbstract.h"
#include "readers/VariableAvailablility.h"
#include "model/pairs/VariableAbstract.h"
#include "model/pairs/Tick.h"

#include "TradingPairs.h"

const QString TradingPairs::SETTINGS_KEY_BASE{"TradingPairs_"};
TradingPairs::TradingPairs(const QString &templateId, QObject *parent)
    : QAbstractTableModel(parent)
{
    m_settingsKey = SETTINGS_KEY_BASE + templateId;
    _initColInfos();
    auto variablesAvailability = StreamReaderAbstract::allAvailableVariables();
    QList<Qt::CheckState> checkedDefault;
    for (const auto &colInfo : m_colInfos)
    {
        checkedDefault << Qt::Unchecked;
    }
    for (const auto &variableAvailability : variablesAvailability)
    {
        m_variableNames << variableAvailability.variable->name();
        m_checked << checkedDefault;
        QVariantList variantList;
        for (const auto &colInfo : m_colInfos)
        {
            variantList << colInfo.data(&variableAvailability);
        }
        m_listOfVariantList << variantList;
    }
    _loadFromSettings();
}

TradingPairs *TradingPairs::instance(const QString &templateId)
{
    static QHash<QString, TradingPairs *> instances;
    if (!instances.contains(templateId))
    {
        static QList<QSharedPointer<TradingPairs>> list;
        list << QSharedPointer<TradingPairs>{new TradingPairs{templateId}};
        instances[templateId] = list.last().data();
    }
    return instances[templateId];
}

bool TradingPairs::isRowChecked(int row) const
{
    for (const auto &checkState : m_checked[row])
    {
        if (checkState == Qt::Checked)
        {
            return true;
        }
    }
    return false;
}

QMultiHash<QString, VariableAvailability> TradingPairs::getSelectedVariables() const
{
    QMultiHash<QString, VariableAvailability> selectedVariables;
    int i=0;
    int nCols = columnCount();
    auto variablesAvailability = StreamReaderAbstract::allAvailableVariables();
    for (const auto &variableAvailability : variablesAvailability)
    {
        if (isRowChecked(i))
        {
            selectedVariables[variableAvailability.variable->name()]
                = variableAvailability;
        }
        ++i;
    }
    return selectedVariables;
}

void TradingPairs::_loadFromSettings()
{
    auto settings = WorkingDirectoryManager::instance()->settingsLocalIfClient();
    auto name_colIdChecked
            = settings->value(m_settingsKey)
            .value<QHash<QString, QSet<QString>>>();
    for (int i=0; i<m_variableNames.size(); ++i)
    {
        const auto &variableName = m_variableNames[i];
        if (name_colIdChecked.contains(variableName))
        {
            for (int j=0; j<m_colInfos.size(); ++j)
            {
                if (m_colInfos[j].checkable
                        && name_colIdChecked[variableName].contains(m_colInfos[j].id))
                {
                    m_checked[i][j] = Qt::Checked;
                }
            }
        }
    }
}

void TradingPairs::_saveInSettings()
{
    auto settings = WorkingDirectoryManager::instance()->settingsLocalIfClient();
    QHash<QString, QSet<QString>> name_colIdChecked;
    for (int i=0; i<m_variableNames.size(); ++i)
    {
        const auto &variableName = m_variableNames[i];
        for (int j=0; j<m_colInfos.size(); ++j)
        {
            if (m_colInfos[j].checkable && m_checked[i][j])
            {
                name_colIdChecked[variableName].insert(m_colInfos[j].id);
            }
        }
    }
    settings->setValue(m_settingsKey, QVariant::fromValue(name_colIdChecked));
}

void TradingPairs::_initColInfos()
{
    m_colInfos << ColInfo{"Pair",
                  tr("Pair"),
                  tr("Traded pair"),
                  false,
                  [](const VariableAvailability *variable) -> QVariant {
        return variable->variable->name();
    }};
    m_colInfos << ColInfo{"PairIsOutput",
                  tr("Traded"),
                  tr("If the pair is traded and need to be predicted"),
                  true,
                  [](const VariableAvailability *variable) -> QVariant {
        return QVariant{};
    }};
    m_colInfos << ColInfo{"PairIsInput",
                  tr("In Model"),
                  tr("If the pair is used as input in the model to predict values"),
                  true,
                  [](const VariableAvailability *variable) -> QVariant {
        return QVariant{};
    }};
    for (const auto & tick : Tick::TICKS)
    {
        const auto &tickId = tick->id();
        m_colInfos << ColInfo{tick->id(),
                      tick->name(),
                      tr("If the pair is available for the period") + " " + tick->name(),
                      false,
                      [tickId](const VariableAvailability *variable) -> QVariant {
            if (variable->tickIds.contains(tickId))
            {
                return QObject::tr("Yes");
            }
            return QVariant{};
        }};
    }
}

QVariant TradingPairs::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
        {
            return m_colInfos[section].name;
        }
        if (role == Qt::ToolTipRole)
        {
            return m_colInfos[section].toolTip;
        }
        if (role == Qt::CheckStateRole) // Not enough, won’t go here
        {
            if (m_colInfos[section].checkable)
            {
                int nChecked = 0;
                int nUnchecked = 0;
                for (const auto &listChecked : m_checked)
                {
                    if (listChecked[section] == Qt::Checked)
                    {
                        ++nChecked;
                    }
                    else
                    {
                        ++nUnchecked;
                    }
                    if (nChecked > 0 && nUnchecked > 0)
                    {
                        return Qt::PartiallyChecked;
                    }
                }
                if (nChecked > 0)
                {
                    return Qt::Checked;
                }
                if (nUnchecked > 0)
                {
                    return Qt::Unchecked;
                }
            }
        }
    }
    return QVariant{};
}

bool TradingPairs::setHeaderData(
        int section,
        Qt::Orientation orientation,
        const QVariant &value,
        int role)
{
    if (role == Qt::CheckStateRole && m_colInfos[section].checkable)  // Not enough, won’t go here
    {
        auto newCheck = value.value<Qt::CheckState>();
        if (newCheck != Qt::PartiallyChecked)
        {
            for (auto &listCheck : m_checked)
            {
                listCheck[section] = newCheck;
            }
            emit headerDataChanged(Qt::Horizontal, section, section);
            emit dataChanged(index(0, section), index(rowCount()-1, section), QList<int>{role});
            return true;
        }
    }
    return false;
}

int TradingPairs::rowCount(const QModelIndex &parent) const
{
    return m_listOfVariantList.size();
}

int TradingPairs::columnCount(const QModelIndex &parent) const
{
    return m_colInfos.size();
}

QVariant TradingPairs::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return m_listOfVariantList[index.row()][index.column()];
    }
    else if (role == Qt::CheckStateRole)
    {
        if (m_colInfos[index.column()].checkable)
        {
            return m_checked[index.row()][index.column()];
        }
    }
    return QVariant();
}

bool TradingPairs::setData(
        const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole && m_colInfos[index.column()].checkable)
    {
        auto checkState = value.toBool() ? Qt::Checked : Qt::Unchecked;
        if (checkState != m_checked[index.row()][index.column()])
        {
            m_checked[index.row()][index.column()] = checkState;
            _saveInSettings();
            emit dataChanged(index, index);
            return true;
        }
    }
    return false;
}

Qt::ItemFlags TradingPairs::flags(const QModelIndex &index) const
{
    if (m_colInfos[index.column()].checkable)
    {
        return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    }
    return Qt::ItemIsEnabled;
}
