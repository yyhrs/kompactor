//------------------------------------------------------------------------------
//
// Settings.cpp created by Yyhrs 15
//
//------------------------------------------------------------------------------

#include <QHeaderView>

#include "Settings.hpp"

Settings::Settings(QSettings::Scope scope, const QString &organization, QObject *parent):
	QSettings{QSettings::IniFormat, scope, organization, qAppName(), parent}
{
}

Settings::Settings(const QString &fileName, QObject *parent):
	QSettings{fileName, QSettings::IniFormat, parent}
{
}

QVariantMap Settings::groupValues(const QString &group)
{
	QVariantMap result;

	beginGroup(group);
	for (auto &key: allKeys())
		result[key] = value(key);
	endGroup();
	return result;
}

void Settings::setGroupValues(const QString &group, const QVariantMap &values)
{
	beginGroup(group);
	for (const auto &key: values.keys())
		setValue(key, values[key]);
	endGroup();
}

void Settings::restoreState(QMainWindow *mainWindow) const
{
	mainWindow->restoreGeometry(value(Geometry).toByteArray());
	mainWindow->restoreState(value(State).toByteArray());
}

void Settings::saveState(const QMainWindow *mainWindow)
{
	setValue(Geometry, mainWindow->saveGeometry());
	setValue(State, mainWindow->saveState());
}

bool Settings::restoreState(const QString &key, QTableView *tableView) const
{
	return tableView->horizontalHeader()->restoreState(value(key).toByteArray());
}

void Settings::saveState(const QString &key, const QTableView *tableView)
{
	setValue(key, tableView->horizontalHeader()->saveState());
}

void Settings::restoreValues(const QString &key, QComboBox *combobox, const QStringList defaultValues) const
{
	combobox->addItems(value(key, defaultValues).toStringList());
}

void Settings::saveValues(const QString &key, const QComboBox *combobox)
{
	QStringList items;

	for (int index = 0; index < combobox->count(); ++index)
		items << combobox->itemText(index);
	setValue(key, items);
}

void Settings::restoreValues(const QString &key, QListWidget *listWidget, const QStringList defaultValues) const
{
	listWidget->addItems(value(key, defaultValues).toStringList());
}

void Settings::saveValues(const QString &key, const QListWidget *listWidget)
{
	QStringList items;

	for (int index = 0; index < listWidget->count(); ++index)
		items << listWidget->item(index)->text();
	setValue(key, items);
}
