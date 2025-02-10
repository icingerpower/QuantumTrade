#ifndef TEMPLATEMANAGER_H
#define TEMPLATEMANAGER_H

#include <QAbstractListModel>

class TemplateManager : public QAbstractListModel
{
    Q_OBJECT

public:
    static TemplateManager *instance();
    static const QString KEY_TEMPLATE_MANAGER;
    QString getId(const QModelIndex &index) const;
    void addTemplate(const QString &name);
    void removeTemplate(const QModelIndex &index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    explicit TemplateManager(QObject *parent = nullptr);
    QList<QStringList> m_listOfStringList;
    void _saveInSettings();
    void _loadFromSettings();
};

#endif // TEMPLATEMANAGER_H
