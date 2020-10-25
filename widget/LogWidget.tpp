//------------------------------------------------------------------------------
//
// LogWidget.tpp created by Yyhrs 2019-10-24
//
//------------------------------------------------------------------------------

#include "LogWidget.hpp"

template<typename Header>
LogWidget::Return<Header, void> LogWidget::addColumns(const QList<Header> &headers)
{
	m_logModel.addColumns(headers);
	setModel(&m_logModel);
}
