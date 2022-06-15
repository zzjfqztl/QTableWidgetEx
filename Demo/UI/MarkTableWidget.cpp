#include "MarkTableWidget.h"
#include "QAbstractButton"
#include "QApplication"
#include <QHeaderView>
#include <QStylePainter>
#include "QMenu"
#include "qevent.h"
#include <QDebug>
#include "QRect"

MarkTableWidget::MarkTableWidget(const QString& title, QWidget *parent) : QTableWidget(parent)
{
	QAbstractButton* btn = findChild<QAbstractButton*>();
	if (btn)
	{
		btn->setText(title);
		btn->installEventFilter(this);
		// adjust the width of the vertical header to match the preferred corner button width
		// (unfortunately QAbstractButton doesn't implement any size hinting functionality)
		QStyleOptionHeader opt;
		opt.text = btn->text();
		opt.textAlignment = Qt::AlignCenter;
		QSize s = (btn->style()->sizeFromContents(QStyle::CT_HeaderSection, &opt, QSize(), btn).
			expandedTo(QApplication::globalStrut()));
		if (s.isValid())
			verticalHeader()->setMinimumWidth(s.width());
	}
	
	tableItemDelegate_ = new TableItemDelegate();
	setItemDelegate(tableItemDelegate_);
	setMouseTracking(true);
	setAttribute(Qt::WA_Hover);
}


void MarkTableWidget::setCornerWidgetText(const QString& title)
{
	QAbstractButton* btn = findChild<QAbstractButton*>();
	if (btn)
	{
		btn->setText(title);
		// adjust the width of the vertical header to match the preferred corner button width
		// (unfortunately QAbstractButton doesn't implement any size hinting functionality)
		QStyleOptionHeader opt;
		opt.text = btn->text();
		opt.textAlignment = Qt::AlignCenter;
		QSize s = (btn->style()->sizeFromContents(QStyle::CT_HeaderSection, &opt, QSize(), btn).
			expandedTo(QApplication::globalStrut()));
		if (s.isValid())
			verticalHeader()->setMinimumWidth(s.width());
	}
}


QColor MarkTableWidget::getNormalColor() const
{
	return normalColor_;
}

void MarkTableWidget::setNormalColor(QColor color)
{
	normalColor_ = color;
	tableItemDelegate_->SetNormalColor(normalColor_);
}

QColor MarkTableWidget::getSelectColor() const
{
	return selectColor_;
}

void MarkTableWidget::setSelectColor(QColor color)
{
	selectColor_ = color;
	tableItemDelegate_->SetSelectColor(selectColor_);
}


bool MarkTableWidget::eventFilter(QObject* o, QEvent* e)
{
	if (e->type() == QEvent::Paint)
	{
		QAbstractButton* btn = qobject_cast<QAbstractButton*>(o);
		if (btn)
		{
			// paint by hand (borrowed from QTableCornerButton)
			QStyleOptionHeader opt;
			opt.init(btn);
			QStyle::State state = QStyle::State_None;
			if (btn->isEnabled())
				state |= QStyle::State_Enabled;
			if (btn->isActiveWindow())
				state |= QStyle::State_Active;
			if (btn->isDown())
				state |= QStyle::State_Sunken;
			opt.state = state;
			opt.rect = btn->rect();
			opt.text = btn->text(); // this line is the only difference to QTableCornerButton
			opt.textAlignment = Qt::AlignCenter;
			opt.position = QStyleOptionHeader::Middle;
			QStylePainter painter(btn);
			painter.drawControl(QStyle::CE_Header, opt);
			return true; // eat event
		}
	}
	return false;
	
}

void MarkTableWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		bMousePress_ = true;
		startPos_ = event->pos();
	}
	__super::mousePressEvent(event);
}

void MarkTableWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (bMousePress_)
	{
		endPos_ = event->pos();
		calcSelectItems();
	}
	__super::mouseMoveEvent(event);
}

void MarkTableWidget::mouseReleaseEvent(QMouseEvent* event)
{
	bMousePress_ = false;
	endPos_ = event->pos();
	
	QList<QPair<int, int>> leftRectItems;
	QList<QPair<int, int>> rightRectItems;
	QList<QPair<int, int>> rectItems;
	tableItemDelegate_->SetSelectColor(selectColor_);
	tableItemDelegate_->SetNormalColor(normalColor_);
	tableItemDelegate_->SetSelectItems(leftRectItems, rightRectItems, rectItems);
	update();
	__super::mouseReleaseEvent(event);
}

void MarkTableWidget::calcSelectItems()
{
	QList<QPair<int, int>> leftRectItems;
	QList<QPair<int, int>> rightRectItems;
	QList<QPair<int, int>> rectItems;
	
	QPoint startPos;
	QPoint endPos;
	GetValidRect(startPos, endPos);

	auto startItem = this->itemAt(startPos);
	auto endItem = this->itemAt(endPos);
	

	if (startItem != nullptr && endItem != nullptr)
	{
		int nLeft = startItem->column();
		int nTop = startItem->row();
		int nRight = endItem->column();
		int nBottom = endItem->row();
		int nLeftHover = 0;
		int nRightHover = 0;
		
		for (int iLoop = nTop; iLoop <= nBottom; iLoop++)
		{
			for (int jLoop = nLeft; jLoop <= nRight; jLoop++)
			{
				if (jLoop == nLeft)
				{
					auto leftItem = itemAt(iLoop, jLoop);
					if (leftItem)
					{
						if (iLoop == nTop)
						{
							QWidget* w = this->cellWidget(iLoop, jLoop);
							if (w)
							{
								auto itemwidgets = w->findChildren<TableItemWidget*>();
								if (itemwidgets.count() > 0)
								{
									for (auto& itr : itemwidgets)
									{
										nLeftHover = itr->GetSelectRect(w->mapFromParent(startPos), QRect(w->mapFromParent(startPos), w->mapFromParent(endPos)));
										if (nLeftHover == 3)
										{
											rectItems.append(QPair<int,int>(iLoop,jLoop));
										}
										else if (nLeftHover == 2)
										{
											rightRectItems.append(QPair<int, int>(iLoop, jLoop));
										}
										else if (nLeftHover == 1)
										{
											leftRectItems.append(QPair<int, int>(iLoop, jLoop));
										}
									}
								}
							}
						}
						else
						{
							if (nLeftHover == 3)
							{
								rectItems.append(QPair<int, int>(iLoop, jLoop));
							}
							else if (nLeftHover == 2)
							{
								rightRectItems.append(QPair<int, int>(iLoop, jLoop));
							}
							else if (nLeftHover == 1)
							{
								leftRectItems.append(QPair<int, int>(iLoop, jLoop));
							}
						}
						
					}

				}
				else if (jLoop == nRight)
				{
					continue;
				}
				else
				{
					auto item = itemAt(iLoop, jLoop);
					if (item)
					{
						rectItems.append(QPair<int, int>(iLoop, jLoop));
					}
				}
			}

		}

		for (int iLoop = nBottom; iLoop >= nTop; iLoop--)
		{
			auto rightitem = itemAt(iLoop, nRight);
			if (rightitem)
			{
				if (iLoop == nBottom)
				{
					QWidget* w = this->cellWidget(iLoop, nRight);
					if (w)
					{
						auto itemwidgets = w->findChildren<TableItemWidget*>();
						if (itemwidgets.count() > 0)
						{
							for (auto& itr : itemwidgets)
							{
								nRightHover = itr->GetSelectRect(w->mapFromParent(endPos), QRect(w->mapFromParent(startPos), w->mapFromParent(endPos)));
								if (nRightHover == 3)
								{
									rectItems.append(QPair<int, int>(iLoop, nRight));
								}
								else if (nRightHover == 2)
								{
									rightRectItems.append(QPair<int, int>(iLoop, nRight));
								}
								else if (nRightHover == 1)
								{
									leftRectItems.append(QPair<int, int>(iLoop, nRight));
								}
							}
						}
					}
				}
				else
				{
					if (nRightHover == 3)
					{
						rectItems.append(QPair<int, int>(iLoop, nRight));
					}
					else if (nRightHover == 2)
					{
						rightRectItems.append(QPair<int, int>(iLoop, nRight));
					}
					else if (nRightHover == 1)
					{
						leftRectItems.append(QPair<int, int>(iLoop, nRight));
					}
				}

			}
			
		}
	}
	tableItemDelegate_->SetSelectColor(normalColor_);
	tableItemDelegate_->SetSelectColor(selectColor_);
	tableItemDelegate_->SetSelectItems(leftRectItems, rightRectItems, rectItems);
	update();
}

void MarkTableWidget::GetValidRect(QPoint& leftTopPos, QPoint& rightBottomPos)
{
	auto rect1 = visualItemRect(item(0, 0));
	auto rect2 = visualItemRect(item(rowCount() - 1, columnCount() - 1));
	QPoint leftPos = rect1.topLeft();
	QPoint rightPos = rect2.bottomRight();
	auto rect3 = QRect(startPos_, endPos_);
	QPoint pos;
	pos = rect3.topLeft();
	if (rect3.width() < 0)
	{
		pos.setX(rect3.x() + rect3.width());
	}
	if (rect3.height() < 0)
	{
		pos.setY(rect3.y() + rect3.height());
	}
	auto rect4 = QRect(pos.x(), pos.y(), qAbs(rect3.width()), qAbs(rect3.height()));
	leftTopPos = rect4.topLeft();
	rightBottomPos = rect4.bottomRight();
	if (leftTopPos.x() < leftPos.x())
	{
		leftTopPos.setX(leftPos.x());
	}
	if (leftTopPos.y() < leftPos.y())
	{
		leftTopPos.setX(leftPos.y());
	}

	if (leftTopPos.x() > rightPos.x())
	{
		leftTopPos.setX(rightPos.x());
	}
	if (leftTopPos.y() > rightPos.y())
	{
		leftTopPos.setY(rightPos.y());
	}


	if (rightBottomPos.x() > rightPos.x())
	{
		rightBottomPos.setX(rightPos.x());
	}
	if (rightBottomPos.y() > rightPos.y())
	{
		rightBottomPos.setY(rightPos.y());
	}

	if (rightBottomPos.x() < leftPos.x())
	{
		rightBottomPos.setX(leftPos.x());
	}
	if (rightBottomPos.y() < leftPos.y())
	{
		rightBottomPos.setX(leftPos.y());
	}
}

TableItemDelegate::TableItemDelegate()
{

}

void TableItemDelegate::SetSelectColor(QColor color)
{
	selectColor_ = color;
}


void TableItemDelegate::SetNormalColor(QColor color)
{
	normalColor_ = color;
}

void TableItemDelegate::SetSelectItems(const QList<QPair<int, int>>& leftItems, const QList<QPair<int, int>>& rightItems, const QList<QPair<int, int>>& rectItems)
{
	leftRectItems_ = leftItems;
	rightRectItems_ = rightItems;
	rectItems_ = rectItems;
}

void TableItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QStyleOptionViewItem itemOption(option);
	if (itemOption.state & QStyle::State_HasFocus)
	{
		itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
	}
	/*if (index.column() == baseSizeCol_)
	{
		itemOption.palette.setColor(QPalette::Window, QColor(255,0,0));
		painter->fillRect(itemOption.rect, itemOption.palette.color(QPalette::Window));
	}*/
	int rowIndex = index.row();//ÐÐºÅ
	int colIndex = index.column();//ÁÐºÅ

	for(auto &itr : rectItems_)
	{
		if (itr.first == rowIndex && itr.second == colIndex)
		{
			itemOption.palette.setColor(QPalette::Window, selectColor_);
			painter->fillRect(itemOption.rect, itemOption.palette.color(QPalette::Window));
		}
	
	}
	for (auto& itr : leftRectItems_)
	{
		if (itr.first == rowIndex && itr.second == colIndex)
		{
			itemOption.palette.setColor(QPalette::Window, selectColor_);
			QRect leftRect = QRect(itemOption.rect.x(), itemOption.rect.y(), itemOption.rect.width() / 2, itemOption.rect.height());
			painter->fillRect(leftRect, itemOption.palette.color(QPalette::Window));
			itemOption.palette.setColor(QPalette::Window, normalColor_);
			QRect rightRect = QRect(itemOption.rect.x() + itemOption.rect.width() / 2, itemOption.rect.y(), itemOption.rect.width() / 2, itemOption.rect.height());
			painter->fillRect(rightRect, itemOption.palette.color(QPalette::Window));
		}

	}
	for (auto& itr : rightRectItems_)
	{
		if (itr.first == rowIndex && itr.second == colIndex)
		{

			itemOption.palette.setColor(QPalette::Window, selectColor_);
			QRect rightRect = QRect(itemOption.rect.x() + itemOption.rect.width() / 2, itemOption.rect.y(), itemOption.rect.width() / 2, itemOption.rect.height());
			painter->fillRect(rightRect, itemOption.palette.color(QPalette::Window));
			itemOption.palette.setColor(QPalette::Window, normalColor_);
			QRect leftRect = QRect(itemOption.rect.x(), itemOption.rect.y(), itemOption.rect.width() / 2, itemOption.rect.height());
			painter->fillRect(leftRect, itemOption.palette.color(QPalette::Window));
		}

	}

	QStyledItemDelegate::paint(painter, itemOption, index);
}
