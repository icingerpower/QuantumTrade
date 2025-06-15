#ifndef INDICATORS_H
#define INDICATORS_H

#include <QAbstractListModel>

class IndicatorAbstract;

class Indicators : public QAbstractListModel
{
    Q_OBJECT

public:
    static Indicators *instance(const QString &templateId);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    explicit Indicators(const QString &templateId, QObject *parent = nullptr);
    static const QString SETTINGS_KEY_BASE;
    struct IndicatorInfo{
        QString id;
        QString name;
        QString description;
        bool checked;
    };
    QList<IndicatorInfo> m_functionInfos;
    QString m_settingsKey;
    void _loadFromSettings();
    void _saveInSettings();
};

#endif // INDICATORS_H
