#include "ExceptionVariableDb.h"


void ExceptionVariableDb::raise() const
{
    throw this;
}

ExceptionVariableDb *ExceptionVariableDb::clone() const
{
    return new ExceptionVariableDb;
}

const QString &ExceptionVariableDb::error() const
{
    return m_error;
}

void ExceptionVariableDb::setError(const QString &newError)
{
    m_error = newError;
}

const QString &ExceptionVariableDb::title() const
{
    return m_title;
}

void ExceptionVariableDb::setTitle(const QString &newTitle)
{
    m_title = newTitle;
}
