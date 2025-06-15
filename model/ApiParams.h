#ifndef APIPARAMS_H
#define APIPARAMS_H

#include "../common/utils/SortedMap.h"

#include <QAbstractTableModel>

class StreamReaderAbstract;

class ApiParams : public QAbstractTableModel
{
    Q_OBJECT

public:
    static ApiParams *instance();

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    SortedMap<QString, QVariant> getParams(
        const StreamReaderAbstract *streamReader) const;


    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    static const QString SETTINGS_KEY;
    static const QStringList COL_NAMES;
    static const int IND_VALUE;
    static const int IND_ID_PARAM;
    static const int IND_ID_READER;
    explicit ApiParams(QObject *parent = nullptr);
    QList<QVariantList> m_listOfVariantList;
    void _loadFromSettings();
    void _saveInSettings();
};

#endif // APIPARAMS_H
