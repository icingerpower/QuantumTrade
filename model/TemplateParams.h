#ifndef TEMPLATEPARAMS_H
#define TEMPLATEPARAMS_H

#include <QAbstractItemModel>

class Tick;

class TemplateParams : public QAbstractTableModel
{
    Q_OBJECT

public:
    static const QString PARAM_DAYS_DATA;
    static const QString PARAM_DAYS_LIFESPAN;
    static const QString PARAM_TICK;
    static const QString PARAM_TICK_GAP_PREDICTION;
    static const QString PARAM_PCA_N_COMPONENTS;
    static const QString PARAM_N_PAIRS_INVEST;
    static const QString PARAM_STOP_LOSS;
    static const QString PARAM_TAKE_PROFIT;

    static TemplateParams *instance(const QString &templateId);
    QVariant getValue(const QString &paramId) const;
    const Tick *getTick() const;

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    static const QString SETTINGS_KEY_BASE;
    explicit TemplateParams(const QString &templateId, QObject *parent = nullptr);
    struct ParamInfo{
        QString id;
        QString name;
        QString description;
        QVariant value;
    };
    QList<ParamInfo> m_params;
    QHash<QString, int> m_idToParamIndex;
    void _initParams();
    void _loadFromSettings();
    void _saveInSettings();
    QString m_settingsKey;
};

#endif // TEMPLATEPARAMS_H
