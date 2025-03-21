#include "TickDelegate.h"

#include "model/pairs/Tick.h"


TickDelegate::TickDelegate(QObject *parent)
{
}

QWidget *TickDelegate::createEditor(
    QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant data = index.model()->data(index, Qt::EditRole);

    if (data.canConvert<const Tick*>()) {
        QComboBox *editor = new QComboBox(parent);
        for (const Tick* tick : Tick::TICKS)
        {
            editor->addItem(tick->name(), QVariant::fromValue(tick));
        }
        return editor;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void TickDelegate::setEditorData(
    QWidget *editor, const QModelIndex &index) const
{
    QComboBox *combo = qobject_cast<QComboBox*>(editor);
    if (combo)
    {
        const auto &variant = index.data(Qt::EditRole);
        if (variant.canConvert<const Tick*>())
        {
            const Tick *tick = variant.value<const Tick *>();
            int indexCombo = combo->findData(QVariant::fromValue(tick));
            if (indexCombo >= 0)
            {
                combo->setCurrentIndex(indexCombo);
            }
            return;
        }
    }
    else
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void TickDelegate::setModelData(
    QWidget *editor,
    QAbstractItemModel *model,
    const QModelIndex &index) const
{
    QComboBox *combo = qobject_cast<QComboBox*>(editor);
    if (!combo)
    {
        QStyledItemDelegate::setModelData(editor, model, index);
        return;
    }

    // Here, we assume the combo box always returns a Tick pointer.
    QVariant currentData = combo->currentData();
    if (!currentData.canConvert<const Tick*>())
    {
        QStyledItemDelegate::setModelData(editor, model, index);
        return;
    }
    model->setData(index, currentData, Qt::EditRole);
}

QString TickDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    if (value.canConvert<const Tick*>())
    {
        const Tick *tick = value.value<const Tick *>();
        return tick->name();
    }
    return QStyledItemDelegate::displayText(value, locale);
}

