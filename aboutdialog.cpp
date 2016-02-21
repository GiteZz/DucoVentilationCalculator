#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    QPixmap pix(":/images/logoDuco.jpg");
    ui->logo->setPixmap(pix);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_closeButton_clicked()
{
    close();
}

void AboutDialog::on_qtWebsite_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.qt.io/"));
}

void AboutDialog::on_gillesprojects_clicked()
{
    QDesktopServices::openUrl(QUrl("www.gillesprojects.be"));
}

void AboutDialog::on_websiteDuco_clicked()
{
    QDesktopServices::openUrl(QUrl("www.duco.eu"));
}

void AboutDialog::on_broncode_clicked()
{
    QDesktopServices::openUrl(QUrl("www.github.com"));
}
