#include "TableItemWidget.h"
#include "QTimer"
#include <QDebug>
#include <QKeyEvent>
#include "QPainter"
#include "qnamespace.h"
#include "QTextOption"

TableItemWidget::TableItemWidget(QWidget* parent /*= nullptr*/): QLabel(parent)
{
	setAlignment(Qt::AlignCenter);
	setMouseTracking(true);
	setAttribute(Qt::WA_Hover);
	installEventFilter(this);
}

QColor TableItemWidget::getNormalColor() const
{
	return normalColor_;
}

void TableItemWidget::setNormalColor(QColor color)
{
	normalColor_ = color;
}

QColor TableItemWidget::getHoverColor() const
{
	return hoverColor_;
}

void TableItemWidget::setHoverColor(QColor color)
{
	hoverColor_ = color;
}

QColor TableItemWidget::getTextNormalColor() const
{
	return textNormalColor_;
}

void TableItemWidget::setTextNormalColor(QColor color)
{
	textNormalColor_ = color;
}

QColor TableItemWidget::getNoDataColor() const
{
	return noDataColor_;
}

void TableItemWidget::setNoDataColor(QColor color)
{
	noDataColor_ = color;
}

void TableItemWidget::SetData(const QMap<QString, int>& datas)
{
	data_ = datas;
	markFlag_ = 0;
	auto itrFind = datas.find(QString("L"));
	if (itrFind != datas.end())
	{
		left_num_ = itrFind.value();
		markFlag_ = 1;
	}

	auto itrFind2 = datas.find(QString("R"));
	if (itrFind2 != datas.end())
	{
		right_num_ = itrFind2.value();
		markFlag_ = 2;
	}
	if (data_.size() == 2)
	{
		markFlag_ = 3;
		
	}
	UpdateData();
}

void TableItemWidget::SetChecked(bool bCheck)
{
	bCheck_ = bCheck;
	update();
}

void TableItemWidget::SetHoverState(int nHover)
{
	nHover_ = nHover;
	update();
}

int TableItemWidget::GetSelectRect(QPoint pos, QRect allRect)
{
	QPoint p = this->frameGeometry().topLeft();
	QRect rect(p.x()- 10,p.y(),this->width()+20,this->height());
	QRect leftRect(rect.x(), rect.y(), rect.width() / 2, rect.height());
	QRect rightRect(rect.x() + rect.width() / 2, rect.y(), rect.width() / 2, rect.height());

	if (allRect.contains(rect))
	{
		return 3;
	}
	else 
	{
		if (markFlag_ == 3)
		{
			if (leftRect.contains(pos))
			{
				if (allRect.intersects(rightRect))
				{
					return 3;
				}
				return 1;
			}
			else if (rightRect.contains(pos))
			{
				if (allRect.intersects(leftRect))
				{
					return 3;
				}
				return 2;
			}
			else
			{
				return 0;
			}
		}
		else if (markFlag_ == 2)
		{
			if (rect.contains(pos))
			{
				return 3;
			}
			else
			{
				return 0;
			}
		}
		else if (markFlag_ == 1)
		{
			if (rect.contains(pos))
			{
				return 3;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

void TableItemWidget::mousePressEvent(QMouseEvent* event)
{
	auto pos = event->pos();
	QRect rect = this->rect();
	if (markFlag_ == 3)
	{
		QRect leftRect(rect.x(), rect.y(), rect.width() / 2, rect.height());
		QRect rightRect(rect.x()+ rect.width() / 2, rect.y(), rect.width() / 2, rect.height());
		if (leftRect.contains(pos))
		{
			emit mousePressSgn(0);
			if (left_num_ > 0)
			{
				left_num_--;
			}
			qDebug() << "0";
		}
		else if (rightRect.contains(pos))
		{
			emit mousePressSgn(1);
			if (right_num_ > 0)
			{
				right_num_--;
			}
			qDebug() << "1";
		}
	}
	else if (markFlag_ == 2)
	{
		if (rect.contains(pos))
		{
			emit mousePressSgn(1);
			if (right_num_ > 0)
			{
				right_num_--;
			}
			qDebug() << "1";
		}
	}
	else if (markFlag_ == 1)
	{
		if (rect.contains(pos))
		{
			emit mousePressSgn(0);
			if (left_num_ > 0)
			{
				left_num_--;
			}
			qDebug() << "0";
		}
	}
	if (markFlag_ != 0)
	{
		UpdateData();
	}
	__super::mousePressEvent(event);
}

void TableItemWidget::mouseMoveEvent(QMouseEvent* event)
{
	__super::mouseMoveEvent(event);
}

void TableItemWidget::mouseReleaseEvent(QMouseEvent* event)
{
	__super::mouseReleaseEvent(event);
}

bool TableItemWidget::eventFilter(QObject* o, QEvent* e)
{
	 if (e->type() == QEvent::HoverMove) {
		//设置对应鼠标形状,这个必须放在这里而不是下面,因为可以在鼠标没有按下的时候识别
		QHoverEvent* hoverEvent = (QHoverEvent*)e;
		QPoint pos = hoverEvent->pos();
		QRect rect = this->rect();
		nHover_ = 0;
		if (markFlag_ == 3)
		{
			QRect leftRect(rect.x(), rect.y(), rect.width() / 2, rect.height());
			QRect rightRect(rect.x() + rect.width() / 2, rect.y(), rect.width() / 2, rect.height());
			if (leftRect.contains(pos))
			{
				nHover_ = 1;
			}
			else if (rightRect.contains(pos))
			{
				nHover_ = 2;
			}
		}
		else if (markFlag_ == 2)
		{
			if (rect.contains(pos))
			{
				nHover_ = 2;
			}
		}
		else if (markFlag_ == 1)
		{
			if (rect.contains(pos))
			{
				nHover_ = 1;
			}
		}
		update();
		emit hoverItemSgn(this);
	 }
	 else if(e->type() == QEvent::HoverLeave)
	 {
		 nHover_ = 0;
		 update();
	 }
	return false;
}

void TableItemWidget::UpdateData()
{
	update();
}

void TableItemWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.save();
	QRect rect = this->rect();
	QRect leftRect(rect.x(), rect.y(), rect.width() / 2, rect.height());
	QRect rightRect(rect.x() + rect.width() / 2, rect.y(), rect.width() / 2, rect.height());
	if (bCheck_)
	{
		painter.fillRect(rect, hoverColor_);
	}
	if (!bCheck_)
	{
		if (nHover_ == 3)
		{
			painter.fillRect(rect, hoverColor_);
		}
		else if (nHover_ == 2)
		{

			if (markFlag_ == 3)
			{
				painter.fillRect(rightRect, hoverColor_);
				painter.fillRect(leftRect, normalColor_);
			}
			else
			{
				painter.fillRect(rect, hoverColor_);
			}

		}
		else if (nHover_ == 1)
		{
			if (markFlag_ == 3)
			{
				painter.fillRect(leftRect, hoverColor_);
				painter.fillRect(rightRect, normalColor_);
			}
			else
			{
				painter.fillRect(rect, hoverColor_);
			}
		}
		else if (nHover_ == 0)
		{
			painter.fillRect(rect, normalColor_);
		}
	}

	if (markFlag_ == 3)
	{

		if (left_num_ <= 0)
		{
			painter.setBrush(Qt::BrushStyle::NoBrush);
			painter.setPen(noDataColor_);
		}
		else
		{
			painter.setBrush(Qt::BrushStyle::NoBrush);
			painter.setPen(textNormalColor_);
		}
		painter.drawText(leftRect, QString("%1-").arg(left_num_), QTextOption(Qt::AlignRight | Qt::AlignVCenter));

		if (right_num_ <= 0)
		{
			painter.setBrush(Qt::BrushStyle::NoBrush);
			painter.setPen(noDataColor_);
		}
		else
		{
			painter.setBrush(Qt::BrushStyle::NoBrush);
			painter.setPen(textNormalColor_);
		}
		painter.drawText(rightRect, QString("-%1").arg(right_num_), QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
	}
	else if (markFlag_ == 2)
	{
		if (right_num_ <= 0)
		{
			painter.setBrush(Qt::BrushStyle::NoBrush);
			painter.setPen(noDataColor_);
		}
		else
		{
			painter.setBrush(Qt::BrushStyle::NoBrush);
			painter.setPen(textNormalColor_);
		}
		painter.drawText(rect, QString("%1%2").arg(right_mark_).arg(right_num_), QTextOption(Qt::AlignCenter));
	}
	else if (markFlag_ == 1)
	{
		if (left_num_ <= 0)
		{
			painter.setBrush(Qt::BrushStyle::NoBrush);
			painter.setPen(noDataColor_);
		}
		else
		{
			painter.setBrush(Qt::BrushStyle::NoBrush);
			painter.setPen(textNormalColor_);
		}
		painter.drawText(rect, QString("%1%2").arg(left_mark_).arg(left_num_), QTextOption(Qt::AlignCenter));

	}
	painter.restore();
}

