#include <QDateTime>
#include <QSettings>

#include "../common/workingdirectory/WorkingDirectoryManager.h"

#include "TemplateManager.h"

const QString TemplateManager::KEY_TEMPLATE_MANAGER{"TemplateManager"};

TemplateManager::TemplateManager(QObject *parent)
    : QAbstractListModel(parent)
{
    m_listOfStringList << QStringList{tr("Default"), "id-default"};
    _loadFromSettings();
}

TemplateManager *TemplateManager::instance()
{
    static TemplateManager instance;
    return &instance;
}

QString TemplateManager::getId(const QModelIndex &index) const
{
    return m_listOfStringList[index.row()].last();
}

void TemplateManager::addTemplate(const QString &name)
{
    beginInsertRows(QModelIndex{}, m_listOfStringList.size(), m_listOfStringList.size());
    const QString id = name + QDateTime::currentDateTime().toString("yyyyNNddhhmmsszzz");
    m_listOfStringList << QStringList{name, id};
    _saveInSettings();
    endInsertRows();
}

void TemplateManager::removeTemplate(const QModelIndex &index)
{
    beginRemoveRows(QModelIndex{}, index.row(), index.row());
    m_listOfStringList.removeAt(index.row());
    _saveInSettings();
    endRemoveRows();
}

int TemplateManager::rowCount(const QModelIndex &) const
{
    return m_listOfStringList.size();
}

QVariant TemplateManager::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return m_listOfStringList[index.row()][index.column()];
    }
    return QVariant();
}

bool TemplateManager::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        m_listOfStringList[index.row()][index.column()] = value.toString();
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags TemplateManager::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void TemplateManager::_saveInSettings()
{
    auto settings = WorkingDirectoryManager::instance()->settingsLocalIfClient();
    if (m_listOfStringList.size() > 0)
    {
        settings->setValue(KEY_TEMPLATE_MANAGER, QVariant::fromValue(m_listOfStringList));
    }
    else if (settings->contains(KEY_TEMPLATE_MANAGER))
    {
        settings->remove(KEY_TEMPLATE_MANAGER);
    }
}

void TemplateManager::_loadFromSettings()
{
    auto settings = WorkingDirectoryManager::instance()->settingsLocalIfClient();
    m_listOfStringList = settings->value(
                              KEY_TEMPLATE_MANAGER).value<QList<QStringList>>();
}
