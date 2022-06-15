#include "MainWindow.h"
#include "QBoxLayout"
#include "QPushButton"
#include "MarkTableWidget.h"
#include "QHeaderView"
#include "TableItemWidget.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	InitUI();
	showMaximized();
}

void MainWindow::InitUI()
{
	QWidget* centerWidget_ = new QWidget(this);
	centerWidget_->setObjectName("centerWidget");
	setCentralWidget(centerWidget_);

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->setSpacing(10);
	vLayout->setMargin(0);
	vLayout->setContentsMargins(20, 10, 20, 10);
	vLayout->setAlignment(Qt::AlignTop);
	centerWidget_->setLayout(vLayout);
	MarkTableWidget* tableWidget_ = new MarkTableWidget(QStringLiteral("面料"),centerWidget_);
	vLayout->addWidget(tableWidget_);

	QStringList strHeaders;
	strHeaders << QStringLiteral("面料1") << QStringLiteral("面料1") << QStringLiteral("面料1") << QStringLiteral("面料1") << QStringLiteral("面料1");
	tableWidget_->setObjectName("tableWidget");
	tableWidget_->setWordWrap(true);
	tableWidget_->horizontalHeader()->setDefaultSectionSize(80);
	tableWidget_->horizontalHeader()->setFixedHeight(80);
	tableWidget_->horizontalHeader()->setStretchLastSection(false); //设置充满表宽度
	tableWidget_->verticalHeader()->setDefaultSectionSize(24); //设置行距
	//tableWidget_->verticalHeader()->setFixedWidth(120); //设置行距
	tableWidget_->setFrameShape(QFrame::NoFrame); //设置无边框
	tableWidget_->setShowGrid(true); //设置不显示格子线
	tableWidget_->verticalHeader()->setVisible(true); //设置垂直头不可见
	tableWidget_->setSelectionBehavior(QAbstractItemView::SelectItems);   // 单个选中
	tableWidget_->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);   // 单个选中
	tableWidget_->horizontalHeader()->setHighlightSections(false);
	tableWidget_->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
	//tableWidget_->setItemDelegate(new NoFocusDelegate());
	tableWidget_->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
	tableWidget_->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

	tableWidget_->verticalHeader()->setSectionsMovable(true);
	tableWidget_->setColumnCount(strHeaders.size());
	tableWidget_->setHorizontalHeaderLabels(strHeaders);
	tableWidget_->setRowCount(strHeaders.size());
	tableWidget_->setVerticalHeaderLabels(strHeaders);
	int nRow = 0;
	for (auto& itr : strHeaders)
	{
		QTableWidgetItem* item = nullptr;
		for (int iLoop = 0; iLoop < strHeaders.count(); iLoop++)
		{
			item = new QTableWidgetItem();
			item->setTextAlignment(Qt::AlignCenter);
			QString strOutValue = "1-1";
			//item->setText(strOutValue);
			tableWidget_->setItem(nRow, iLoop, item);
			QRect r = tableWidget_->visualItemRect(item);
			QWidget* w = new QWidget(centerWidget_);
			QHBoxLayout* hLayout = new QHBoxLayout(w);
			hLayout->setContentsMargins(10, 0, 10, 0);
			TableItemWidget* itemWidget = new TableItemWidget(tableWidget_);
			connect(itemWidget, &TableItemWidget::hoverItemSgn, this, &MainWindow::onHoverItem);
			QMap<QString, int> dataMap;
			if (iLoop == 2)
			{
				dataMap.insert(QString("R"), 1);
			}
			else if (iLoop == 3)
			{
				dataMap.insert(QString("L"), 1);
			}
			else
			{
				dataMap.insert(QString("L"), 1);
				dataMap.insert(QString("R"), 1);
			}

			itemWidget->SetData(dataMap);
			hLayout->addWidget(itemWidget);
			tableItemWidgets_.append(itemWidget);
			tableWidget_->setCellWidget(nRow, iLoop, w);
		}
		nRow++;
	}
}

void MainWindow::onHoverItem(TableItemWidget* item)
{
	if (item != preHoverItem_)
	{
		if (preHoverItem_ != nullptr)
		{
			preHoverItem_->SetHoverState(0);
		}
		preHoverItem_ = item;
	}
}

