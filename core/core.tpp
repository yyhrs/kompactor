//------------------------------------------------------------------------------
//
// core.tpp created by Yyhrs 2019-10-17
//
//------------------------------------------------------------------------------

#ifndef CORE_TPP
#define CORE_TPP

#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QMetaEnum>
#include <QProcess>
#include <QRegularExpression>
#include <QUrl>

namespace Enum
{
	template<typename Value, typename T>
	using Return = typename QtPrivate::QEnableIf<QtPrivate::IsQEnumHelper<Value>::Value, T>::Type;

	template<typename Value>
	inline Return<Value, QString> key(Value value)
	{
		return QMetaEnum::fromType<Value>().valueToKey(value);
	};

	template<typename Value>
	inline Return<Value, Value> value(const QString &key)
	{
		return static_cast<Value>(QMetaEnum::fromType<Value>().keyToValue(key.toLatin1()));
	};
}

namespace File
{
	inline void showInGraphicalShell(const QFileInfo &file)
	{
#ifdef Q_OS_LINUX
		QDesktopServices::openUrl(QUrl::fromLocalFile(file.absolutePath()));
#endif
#ifdef Q_OS_MACOS
		QDesktopServices::openUrl(QUrl::fromLocalFile(file.absolutePath()));
#endif
#ifdef Q_OS_WIN
		QProcess::startDetached("explorer", {"/select,", QDir::toNativeSeparators(file.absoluteFilePath())});
#endif
	}

	inline bool overwriteCopy(const QString &source, const QString &destination)
	{
		if (QFile::exists(destination))
			return QFile::remove(destination) && QFile::copy(source, destination);
		return QFile::copy(source, destination);
	}

	inline bool isValid(const QString &name)
	{
		static const QRegularExpression validator{R"(^(?!(?:CON|PRN|AUX|NUL|COM[1-9]|LPT[1-9])(?:\.[^.]*)?$)[^<>:"/\\|?*\x00-\x1F]*[^<>:"/\\|?*\x00-\x1F\ .]$)", QRegularExpression::CaseInsensitiveOption};

		return validator.match(name).hasMatch();
	}

	inline QString getValid(const QString &name)
	{
		static const QRegularExpression prohibitedNames{R"(^(CON|PRN|AUX|NUL|COM[1-9]|LPT[1-9])$)", QRegularExpression::CaseInsensitiveOption};
		static const QRegularExpression prohibitedCharacters{R"([<>:"\/\\|?*\x00-\x1F]|^[ .]|[ .]$)", QRegularExpression::CaseInsensitiveOption};
		QString							result{name};

		if (prohibitedNames.match(result).hasMatch())
			result.append('_');
		return result.replace(prohibitedCharacters, "_");
	}
}

#endif // CORE_TPP
