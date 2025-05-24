#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QAbstractListModel>

class FunctionAbstract;

class Functions : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit Functions(const QString &templateId, QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    static const QString SETTINGS_KEY_BASE;
    struct FunctionInfo{
        QString id;
        QString name;
        QString description;
        bool checked;
    };
    QList<FunctionInfo> m_functionInfos;
    QString m_settingsKey;
    void _loadFromSettings();
    void _saveInSettings();
};

#endif // FUNCTIONS_H
