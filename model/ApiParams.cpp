#include "../common/workingdirectory/WorkingDirectoryManager.h"

#include "model/readers/StreamReaderAbstract.h"

#include "ApiParams.h"

const QStringList ApiParams::COL_NAMES{
    QObject::tr("Reader")
    , QObject::tr("Param")
    , QObject::tr("Value")
};
const int ApiParams::IND_VALUE = 2;
const int ApiParams::IND_ID_PARAM = 3;
const int ApiParams::IND_ID_READER = 4;
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
                                it.key(),
                                id};
        }
    }
    _loadFromSettings();
}

void ApiParams::_loadFromSettings()
{
    auto settings = WorkingDirectoryManager::instance()
                        ->settingsLocalIfClient();
    if (settings->contains(SETTINGS_KEY))
    {
        auto idReader_idParam_values = settings->value(SETTINGS_KEY)
        .value<QHash<QString, QHash<QString, QVariant>>>();
        for (auto &variantList : m_listOfVariantList)
        {
            const auto& idReader = variantList[IND_ID_READER].toString();
            const auto& idParam = variantList[IND_ID_PARAM].toString();
            if (idReader_idParam_values.contains(idReader)
                && idReader_idParam_values[idReader].contains(idParam))
            {
                variantList[IND_VALUE] = idReader_idParam_values[idReader][idParam];
            }
        }
    }
}

void ApiParams::_saveInSettings()
{
    auto settings = WorkingDirectoryManager::instance()->settingsLocalIfClient();
    QHash<QString, QHash<QString, QVariant>> idReader_idParam_values;
    for (const auto &variantList : m_listOfVariantList)
    {
        const auto& idReader = variantList[IND_ID_READER].toString();
        const auto& idParam = variantList[IND_ID_PARAM].toString();
        idReader_idParam_values[idReader][idParam] = variantList[IND_VALUE];
    }
    settings->setValue(SETTINGS_KEY, QVariant::fromValue(idReader_idParam_values));
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

SortedMap<QString, QVariant> ApiParams::getParams(
    const StreamReaderAbstract *streamReader) const
{
    SortedMap<QString, QVariant> params;
    for (const auto &listParams : m_listOfVariantList)
    {
        if (listParams[IND_ID_READER].toString() == streamReader->id())
        {
            QString paramId = listParams[IND_ID_PARAM].toString();
            params[paramId] = listParams[IND_VALUE];
        }
    }
    return params;
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
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
