#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QString>


namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    void openfile();
    void set_src(QString src);
    ~Form();

public slots:
    void on_outputButton_clicked();
    void on_outputClear_clicked();
    void on_turnButton_clicked();

private:
    Ui::Form *ui;
};

#endif // FORM_H
