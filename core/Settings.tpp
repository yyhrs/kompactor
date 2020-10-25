//------------------------------------------------------------------------------
//
// Settings.tpp created by Yyhrs 15
//
//------------------------------------------------------------------------------

#include "Settings.hpp"

template<typename Key>
Settings::Return<Key, QVariant> Settings::value(Key key, const QVariant &defaultValue) const
{
	return value(QMetaEnum::fromType<Key>().valueToKey(key), defaultValue);
}

template<typename Key>
Settings::Return<Key, void> Settings::setValue(Key key, const QVariant &value)
{
	setValue(QMetaEnum::fromType<Key>().valueToKey(key), value);
}

template<typename Key>
Settings::Return<Key, void> Settings::remove(Key key)
{
	remove(QMetaEnum::fromType<Key>().valueToKey(key));
}

template<typename Key>
Settings::Return<Key, void> Settings::beginGroup(Key group)
{
	beginGroup(QMetaEnum::fromType<Key>().valueToKey(group));
}

template<typename Key>
Settings::Return<Key, QVariantMap> Settings::groupValues(Key group)
{
	return groupValues(QMetaEnum::fromType<Key>().valueToKey(group));
}

template<typename Key>
Settings::Return<Key, void> Settings::setGroupValues(Key group, const QVariantMap &values)
{
	groupValues(QMetaEnum::fromType<Key>().valueToKey(group), values);
}

template<typename Key>
Settings::Return<Key, bool> Settings::restoreState(Key key, QTableView *tableView) const
{
	return restoreState(QString(QMetaEnum::fromType<Key>().valueToKey(key)), tableView);
}

template<typename Key>
Settings::Return<Key, void> Settings::saveState(Key key, const QTableView *tableView)
{
	saveState(QString(QMetaEnum::fromType<Key>().valueToKey(key)), tableView);
}

template<typename Key>
Settings::Return<Key, void> Settings::restoreValues(Key key, QComboBox *combobox, const QStringList defaultValues) const
{
	restoreValues(QString(QMetaEnum::fromType<Key>().valueToKey(key)), combobox, defaultValues);
}

template<typename Key>
Settings::Return<Key, void> Settings::saveValues(Key key, const QComboBox *combobox)
{
	saveValues(QString(QMetaEnum::fromType<Key>().valueToKey(key)), combobox);
}

template<typename Key>
Settings::Return<Key, void> Settings::restoreValues(Key key, QListWidget *listWidget, const QStringList defaultValues) const
{
	restoreValues(QString(QMetaEnum::fromType<Key>().valueToKey(key)), listWidget, defaultValues);
}

template<typename Key>
Settings::Return<Key, void> Settings::saveValues(Key key, const QListWidget *listWidget)
{
	saveValues(QString(QMetaEnum::fromType<Key>().valueToKey(key)), listWidget);
}
