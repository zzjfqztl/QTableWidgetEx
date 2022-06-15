#ifndef TABLEITEMWIDGET_H
#define TABLEITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include "QMap"

class TableItemWidget : public QLabel
{
    Q_OBJECT
		Q_PROPERTY(QColor normalColor READ getNormalColor WRITE setNormalColor)
		Q_PROPERTY(QColor hoverColor READ getHoverColor WRITE setHoverColor)
		Q_PROPERTY(QColor textNormalColor READ getTextNormalColor WRITE setTextNormalColor)
		Q_PROPERTY(QColor noDataColor READ getNoDataColor WRITE setNoDataColor)
public:
	explicit TableItemWidget(QWidget* parent = nullptr);

	QColor getNormalColor() const;
	void setNormalColor(QColor color);

	QColor getHoverColor() const;
	void setHoverColor(QColor color);

	QColor getTextNormalColor() const;
	void setTextNormalColor(QColor color);

	QColor getNoDataColor() const;
	void setNoDataColor(QColor color);

	void SetData(const QMap<QString,int> &datas);
	void SetChecked(bool bCheck);
	void SetHoverState(int nHover);
	int GetSelectRect(QPoint pos,QRect allRect);
signals:
	void mousePressSgn(int);
	void hoverItemSgn(TableItemWidget*);
protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	bool eventFilter(QObject* o, QEvent* e);
	void UpdateData();
	void paintEvent(QPaintEvent* event);
private:
	int left_num_ = 0;
	int right_num_ = 0;
	QString left_mark_ = QString("L:");
	QString right_mark_= QString("R:");
	QMap<QString, int> data_;
	int markFlag_ = 0;
	QColor normalColor_ = QColor(0,0,0,0);
	QColor hoverColor_= QColor(0,0,255);
	QColor textNormalColor_ = QColor(0, 255, 0);
	QColor noDataColor_ = QColor(96, 96, 96);
	int nHover_ = 0;
	bool bCheck_ = false;
};

#endif // FOCUSLINEEDIT_H
