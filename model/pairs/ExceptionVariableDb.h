#ifndef EXCEPTIONVARIABLEDB_H
#define EXCEPTIONVARIABLEDB_H

#include <QException>

class ExceptionVariableDb : public QException
{
public:
    void raise() const override;
    ExceptionVariableDb *clone() const override;

    const QString &error() const;
    void setError(const QString &newError);
    const QString &title() const;
    void setTitle(const QString &newTitle);

private:
    QString m_error;
    QString m_title;
};

#endif // EXCEPTIONVARIABLEDB_H
