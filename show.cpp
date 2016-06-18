#include "show.h"
#include "ui_show.h"
#include "servermanager.h"

CShow::CShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CShow)
{
    ui->setupUi(this);
}

CShow::~CShow()
{
    delete ui;
}

void CShow::on_pushButton_clicked()
{

}

void CShow::on_pushButton_2_clicked()
{
    auto tableData = std::move(CServerManager::getReference().getTableData());

    ui->tableWidget->setRowCount(tableData.size());
    ui->tableWidget->setColumnCount(3);

    QStringList lables;

    for (auto& lable : std::begin(tableData)->second)
    {
       lables << lable.first;
    }

    ui->tableWidget->setHorizontalHeaderLabels(lables);
    ui->tableWidget->verticalHeader()->setVisible(false);



    for (auto& outer : tableData)
    {
        int innerIdx(0);
        int outerIdx(outer.first);
        for (auto& inner : outer.second)
        {
            ui->tableWidget->setColumnWidth(innerIdx, inner.second.size()+149);
            ui->tableWidget->setItem(outerIdx, innerIdx++, new QTableWidgetItem(inner.second));
        }
    }
}
