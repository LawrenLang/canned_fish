#include "mainWindow.h"
#include "ui_mainwindow.h"
#include "deal.h"
#include "form.h"

#include <QDir>
#include <QProgressDialog>
#include <QWidget>
#include <QApplication>
#include <qdesktopwidget.h>
#include <QProcess>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_inputButton_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory(this, "请选择图片输入文件夹", "/");
    if(!filePath.isEmpty())
    {
        ui->inputText->setText(filePath);
    }
}



void MainWindow::on_inputClear_clicked()
{
    ui->inputText->setText("");
}



void MainWindow::openfile()
{
    QProcess process(this);
    process.start("cmd", QStringList() << "/c" << "start " + tr(ui->inputText->toPlainText().toUtf8()));
    process.waitForStarted();
    process.waitForFinished();
}

void MainWindow::on_turnButton_clicked()
{
    QString src_path = ui->inputText->toPlainText();

    if(src_path.mid(0, 8) == "file:///") src_path = src_path.mid(8);


    if(QDir(src_path).exists() && src_path != "")
    {
        if(ui->checkDst->isChecked())
        {
            Form* form = new Form ();
            form->setFixedSize(620, 276);
            form->set_src(src_path);
            form->show();
            return;
        }
        int sum = solve(src_path, "");
        QMessageBox msg;
        msg.setIcon(QMessageBox::NoIcon);
        msg.setText(QString::fromStdString("转化完成！\n共转化%1个文件！").arg(sum));
        msg.setWindowTitle("提示");

        msg.addButton("确认", QMessageBox::RejectRole);
        msg.addButton("打开文件夹", QMessageBox::AcceptRole);
        msg.show();

        QObject::connect(&msg,&QMessageBox::accepted, this, &MainWindow::openfile);
        msg.exec();
    }
    else
    {
        QString msg = "请输入正确的输入地址";
        QMessageBox::warning(this, QStringLiteral("警告"), msg);
    }

}


void MainWindow::on_help_clicked()
{
    QString path = ":/prefix1/help.txt";
    if(path.isEmpty()==false)//如果非空
    {
        QFile file(path);//创建文件对象

        //只读方式,默认只读UTF-8
        bool isok=file.open(QIODevice::ReadOnly);//以只读的方式打开
        if(isok)
        {
            //读文件,一次性读完
           // QByteArray array=file.readAll();//因为这个方法返回的是一个字节数组
            //一行一行读
            QString array;
            while(file.atEnd()==false)
            {
                array+=file.readLine();
            }
            QLabel *label = new QLabel(array);
            QScreen *desktop = QApplication::primaryScreen();
            QFont font ( "Microsoft YaHei", 14); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
            label->setFont(font);
            label->setContentsMargins(30,10,30,10);
            label->move((desktop->availableGeometry().width() - label->width())/ 2, (desktop->availableGeometry().height() - label->height()) /2);
            label->setWindowTitle("帮助指南");
            label->adjustSize();
            label->show();

        }
        file.close();//关闭文件
    }
}
