#ifndef TRADINGPAIRS_H
#define TRADINGPAIRS_H

#include <QAbstractTableModel>

struct VariableAvailability;

class TradingPairs : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TradingPairs(const QString &templateId, QObject *parent = nullptr);

    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section,
                       Qt::Orientation orientation,
                       const QVariant &value,
                       int role = Qt::EditRole) override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    static const QString SETTINGS_KEY_BASE;
    void _initColInfos();
    void _loadFromSettings();
    void _saveInSettings();
    QString m_settingsKey;
    QStringList m_variableNames;
    QList<QVariantList> m_listOfVariantList;
    QList<QList<Qt::CheckState>> m_checked;
    struct ColInfo{
        QString id;
        QString name;
        QString toolTip;
        bool checkable;
        std::function<QVariant(const VariableAvailability *variable)> data;
    };
    QList<ColInfo> m_colInfos;
};

#endif // TRADINGPAIRS_H
