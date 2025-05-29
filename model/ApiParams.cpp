#include "../common/workingdirectory/WorkingDirectoryManager.h"

#include "model/readers/StreamReaderAbstract.h"

#include "ApiParams.h"

const QStringList ApiParams::COL_NAMES{
    QObject::tr("Reader")
    , QObject::tr("Param")
    , QObject::tr("Value")
};
const int ApiParams::IND_VALUE = 2;
const QString ApiParams::SETTINGS_KEY{"ApiParams"};

ApiParams::ApiParams(QObject *parent)
    : QAbstractTableModel(parent)
{
    auto streamReaders = StreamReaderAbstract::allStreamReaders();
    for (const auto &reader : streamReaders)
    {
        const QString &id = reader->id();
        const QString &name = reader->name();
        const auto &paramsDefault = reader->paramsDefault();
        for (auto it = paramsDefault.constBegin();
             it != paramsDefault.constEnd(); ++it)
        {
            m_listOfVariantList
                << QVariantList{name,
                                it.value().name,
                                it.value().value,
                                id};
        }
    }
    _loadFromSettings();
}

void ApiParams::_loadFromSettings()
{
    auto settings = WorkingDirectoryManager::instance()
                        ->settingsLocalIfClient();
    auto id_values = settings->value(SETTINGS_KEY)
                         .value<QHash<QString, QVariant>>();
    for (auto &variantList : m_listOfVariantList)
    {
        const auto &id = variantList.last().toString();
        if (id_values.contains(id))
        {
            variantList[IND_VALUE] = id_values[id];
        }
    }
}

void ApiParams::_saveInSettings()
{
    auto settings = WorkingDirectoryManager::instance()->settingsLocalIfClient();
    QHash<QString, QVariant> id_values;
    for (const auto &variantList : m_listOfVariantList)
    {
        id_values[variantList.last().toString()] = variantList[IND_VALUE];
    }
    settings->setValue(SETTINGS_KEY, QVariant::fromValue(id_values));
}

ApiParams *ApiParams::instance()
{
    static ApiParams instance;
    return &instance;
}

QVariant ApiParams::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return COL_NAMES[section];
    }
    return QVariant{};
}

int ApiParams::rowCount(const QModelIndex &parent) const
{
    return m_listOfVariantList.size();
}

int ApiParams::columnCount(const QModelIndex &) const
{
    return COL_NAMES.size();
}

QVariant ApiParams::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        if (role == Qt::DisplayRole || role == Qt::EditRole)
        {
            return m_listOfVariantList[index.row()][index.column()];
        }
    }
    return QVariant();
}

bool ApiParams::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value)
    {
        m_listOfVariantList[index.row()][index.column()] = value;
        _saveInSettings();
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags ApiParams::flags(const QModelIndex &index) const
{
    if (index.column() == IND_VALUE)
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
