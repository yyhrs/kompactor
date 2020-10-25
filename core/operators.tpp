//------------------------------------------------------------------------------
//
// operators.tpp created by Yyhrs 2019-10-24
//
//------------------------------------------------------------------------------

#ifndef OPERATORS_TPP
#define OPERATORS_TPP

#include <QDir>

inline QString operator/(const QString &parent, const QString &child)
{
	return parent + QDir::separator() + child;
}

inline QString operator/(const QFileInfo &parent, const QFileInfo &child)
{
	return parent.path() / child.filePath();
}

inline QString operator/(const QDir &root, const QString &child)
{
	return root.absolutePath() / child;
}

inline QString operator/(const QDir &root, const QFileInfo &child)
{
	return root.absolutePath() / child.filePath();
}

inline QString operator/(const QDir &root, const QDir &child)
{
	return root.absolutePath() / child.path();
}

#endif // OPERATORS_TPP
