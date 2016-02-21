#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtXml>
#include <QDebug>
#include <cmath>
#include <array>
#include <QClipboard>
#include <QSignalMapper>
#include "aboutdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_berekenButton_clicked();

    void on_topBoxCombobox_currentIndexChanged(int index);

    void on_kleurCombobox_currentIndexChanged(int index);

    void on_isolatieCombobox_currentIndexChanged(int index);

    void on_raamCombobox_currentIndexChanged(int index);

    void on_lengtSpinbox_editingFinished();

    void on_klemBord_clicked();

    void on_kamerComboBox_activated(const QString &arg1);

    void on_deleteButton_clicked();
    void aboutMenuSignal(QWidget *);

    void on_aantalSpinBox_editingFinished();

private:
    Ui::MainWindow *ui;
    std::vector<QString> kleurVe;
    std::vector<QString> raamVe;
    std::vector<QString> currentInfo;
    std::vector<std::vector<QString> > allInfo;
    std::vector<double> som;
    void calculateEv();
};

#endif // MAINWINDOW_H
