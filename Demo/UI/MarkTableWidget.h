#ifndef TABLEWIDGETEX_H
#define TABLEWIDGETEX_H

#include <QTableWidget>
#include "QTimer"
#include "QStyledItemDelegate"
#include "TableItemWidget.h"

class QMenu;
class TableItemDelegate : public QStyledItemDelegate
{
public:
    TableItemDelegate();
    void SetSelectColor(QColor color);
	void SetNormalColor(QColor color);
	void SetSelectItems(const QList<QPair<int,int>>& leftItems, const QList<QPair<int, int>>& rightItems, const QList<QPair<int, int>>& rectItems);
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
private:
    int baseSizeCol_ = -1;
    QColor selectColor_;
	QColor normalColor_ = QColor(0, 0, 0, 0);
	QList<QPair<int, int>> leftRectItems_;
	QList<QPair<int, int>>rightRectItems_;
	QList<QPair<int, int>> rectItems_;
};
class MarkTableWidget : public QTableWidget
{
    Q_OBJECT
        Q_PROPERTY(QColor selectColor READ getSelectColor WRITE setSelectColor)
		Q_PROPERTY(QColor normalColor READ getNormalColor WRITE setNormalColor)
public:
    explicit MarkTableWidget(const QString& title, QWidget *parent = nullptr);
	
	void setCornerWidgetText(const QString& title);
	QColor getSelectColor() const;
	void setSelectColor(QColor color);
	
	QColor getNormalColor() const;
	void setNormalColor(QColor color);

protected:
	bool eventFilter(QObject* o, QEvent* e);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
private:
	void calcSelectItems();
	void GetValidRect(QPoint& leftTopPos, QPoint& rightBottomPos);
private:
	QColor selectColor_ = QColor(0, 0, 255);
	QColor normalColor_ = QColor(0, 0, 255,0);
    TableItemDelegate* tableItemDelegate_ = nullptr;
	QPoint startPos_;
	QPoint endPos_;
	bool bMousePress_ = false;
};

#endif // TABLEWIDGETEX_H
