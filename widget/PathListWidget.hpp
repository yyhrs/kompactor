//------------------------------------------------------------------------------
//
// PathListWidget.hpp created by Yyhrs 2017
//
//------------------------------------------------------------------------------

#ifndef PATHLISTWIDGET_HPP
#define PATHLISTWIDGET_HPP

#include <QListWidget>

class PathListWidget: public QListWidget
{
	Q_OBJECT

public:
	enum Behaviour
	{
		AllEntries,
		Files,
		RecursiveFiles
	};
	enum Action
	{
		Add,
		Remove
	};
	Q_ENUM(Action)

	explicit PathListWidget(QWidget *parent = nullptr);

	const QMap<PathListWidget::Action, QAction *> &actions() const;
	void                                          setBehaviour(Behaviour behaviour);
	void                                          addItem(const QString &path);

signals:
	void listModified();
	void fileDropped();

protected:
	QMimeData *mimeData(const QList<QListWidgetItem*> items) const override;
	void      dragEnterEvent(QDragEnterEvent *event) override;
	void      dragMoveEvent(QDragMoveEvent *event) override;
	void      dropEvent(QDropEvent *event) override;

private:
	Behaviour               m_behaviour{AllEntries};
	QMap<Action, QAction *> m_actions;
};

#endif // PATHLISTWIDGET_HPP
