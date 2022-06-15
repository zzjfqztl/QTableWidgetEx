/**
 * 主窗体部分
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "TableItemWidget.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    /**
     * @brief 主界面初始化
    */
    void InitUI();
private slots:
    void onHoverItem(TableItemWidget* item);
private:

    QList<TableItemWidget*> tableItemWidgets_;
    
    TableItemWidget* preHoverItem_ = nullptr;
};

#endif // MAINWINDOW_H
