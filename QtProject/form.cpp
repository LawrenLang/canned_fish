#include "form.h"
#include "ui_form.h"

#include "deal.h"

#include <QDir>
#include <QProgressDialog>
#include <QWidget>
#include <QApplication>
#include <qdesktopwidget.h>
#include <QProcess>
#include <QScreen>
#include <QFileDialog>

QString src_path;

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    src_path = "";
}

Form::~Form()
{
    delete ui;
}

void Form::on_outputButton_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory(this, "请选择图片输出文件夹", "/");
    if(!filePath.isEmpty())
    {
        ui->outputText->setText(filePath);
    }
}

void Form::on_outputClear_clicked()

{
    ui->outputText->setText("");
}

void Form::openfile()
{
    QProcess process(this);
    process.start("cmd", QStringList() << "/c" << "start " + tr(src_path.toUtf8()));
    process.waitForStarted();
    process.waitForFinished();
    if(ui->outputText->toPlainText() == "") return;
    process.start("cmd", QStringList() << "/c" << "start " + tr(ui->outputText->toPlainText().toUtf8()));
    process.waitForStarted();
    process.waitForFinished();
}

void Form::set_src(QString src)
{
    src_path = src;
}

void Form::on_turnButton_clicked()
{
    QString dst_path = ui->outputText->toPlainText();

    if(src_path.mid(0, 8) == "file:///") src_path = src_path.mid(8);

    if(dst_path.mid(0, 8) == "file:///") dst_path = dst_path.mid(8);

    if(QDir(dst_path).exists() && dst_path != "")
    {
        int sum = solve(src_path, dst_path);
        QMessageBox msg;
        msg.setIcon(QMessageBox::NoIcon);
        msg.setText(QString::fromStdString("转化完成！\n共转化%1个文件！").arg(sum));
        msg.setWindowTitle("提示");

        msg.addButton("确认", QMessageBox::RejectRole);
        msg.addButton("打开文件夹", QMessageBox::AcceptRole);
        msg.show();

        QObject::connect(&msg,&QMessageBox::accepted, this, &Form::openfile);
        msg.exec();
    }
    else
    {
        QString msg = "请输入正确的输入地址";
        QMessageBox::warning(this, QStringLiteral("警告"), msg);
    }
}
