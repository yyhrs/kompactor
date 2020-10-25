//------------------------------------------------------------------------------
//
// Settings.hpp created by Yyhrs 15
//
//------------------------------------------------------------------------------

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QComboBox>
#include <QCoreApplication>
#include <QListWidget>
#include <QMainWindow>
#include <QMetaEnum>
#include <QSettings>
#include <QTableView>

class Settings: public QSettings
{
	Q_OBJECT

public:
	enum Window
	{
		Geometry,
		State
	};
	Q_ENUM(Window)

	template<typename Key, typename T>
	using Return = typename QtPrivate::QEnableIf<QtPrivate::IsQEnumHelper<Key>::Value, T>::Type;

	Settings(QSettings::Scope scope = QSettings::UserScope, const QString &organization = QStringLiteral("SC"), QObject *parent = nullptr);
	Settings(const QString &fileName, QObject *parent = nullptr);
	virtual ~Settings() = default;

	using QSettings::value;
	template<typename Key> Return<Key, QVariant>	value(Key key, const QVariant &defaultValue = QVariant{}) const;

	using QSettings::setValue;
	template<typename Key> Return<Key, void>	setValue(Key key, const QVariant &value);

	using QSettings::remove;
	template<typename Key> Return<Key, void>	remove(Key key);

	using QSettings::beginGroup;
	template<typename Key> Return<Key, void>	beginGroup(Key group);

	QVariantMap										groupValues(const QString &group);
	template<typename Key> Return<Key, QVariantMap>	groupValues(Key group);
	void											setGroupValues(const QString &group, const QVariantMap &values);
	template<typename Key> Return<Key, void>		setGroupValues(Key group, const QVariantMap &values);

	void	restoreState(QMainWindow *mainWindow) const;
	void	saveState(const QMainWindow *mainWindow);

	bool										restoreState(const QString &key, QTableView *tableView) const;
	template<typename Key> Return<Key, bool>	restoreState(Key key, QTableView *tableView) const;
	void										saveState(const QString &key, const QTableView *tableView);
	template<typename Key> Return<Key, void>	saveState(Key key, const QTableView *tableView);

	void										restoreValues(const QString &key, QComboBox *combobox, const QStringList defaultValues) const;
	template<typename Key> Return<Key, void>	restoreValues(Key key, QComboBox *combobox, const QStringList defaultValues) const;
	void										saveValues(const QString &key, const QComboBox *combobox);
	template<typename Key> Return<Key, void>	saveValues(Key key, const QComboBox *combobox);
	void										restoreValues(const QString &key, QListWidget *listWidget, const QStringList defaultValues) const;
	template<typename Key> Return<Key, void>	restoreValues(Key key, QListWidget *listWidget, const QStringList defaultValues) const;
	void										saveValues(const QString &key, const QListWidget *listWidget);
	template<typename Key> Return<Key, void>	saveValues(Key key, const QListWidget *listWidget);
};

#include "Settings.tpp"

#endif // SETTINGS_HPP
