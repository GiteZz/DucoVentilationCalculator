#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    kleurVe.push_back("F1");
    kleurVe.push_back("DucoRal");
    kleurVe.push_back("AndereRal");

    raamVe.push_back("Corto");
    raamVe.push_back("Medio");
    raamVe.push_back("Largo");


    QSignalMapper * mapper = new QSignalMapper(this);
    QObject::connect(mapper,SIGNAL(mapped(QWidget *)),this,SLOT(aboutMenuSignal(QWidget*)));

    QMenu * CB = ui->menuAbout;

    QObject::connect(CB, SIGNAL(aboutToShow()),mapper,SLOT(map()));
    mapper->setMapping(CB, CB);
    ui->aantalSpinBox->setValue(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_berekenButton_clicked()
{
    ui->finaalWaardes->addItem(ui->pbLineEdit->text());
    allInfo.push_back(currentInfo);
    ui->aantalSpinBox->setValue(1);
}

void MainWindow::on_topBoxCombobox_currentIndexChanged(int index)
{
    calculateEv();
}
void MainWindow::calculateEv(){

    currentInfo.clear();
    double standaardWaarde;
    int aantal = ui->aantalSpinBox->value();
    int lengte = ui->lengtSpinbox->value();
    int isolatie = ui->isolatieCombobox->currentIndex();
    int kleur = ui->kleurCombobox->currentIndex();
    if(kleur<=2)kleur = 0;
    else kleur = kleur - 2;
    qDebug()<< "lengte" << lengte;
    int index = ceil(lengte/(double)100) - 5;
    if(index <0) index =0;
    int raam = ui->raamCombobox->currentIndex();

    QDomDocument doc;
    //C:/Users/Gilles/Documents/QTProjects/DucoVentilatieRooster/
    QString temp = QCoreApplication::applicationDirPath() + "/LengteXML.xml";

    QFile file(temp);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Failed to open - 1";
    }else{
        if(!doc.setContent(&file)){
            qDebug() << "Failed to load - 2";
        }
        file.close();
    }
    QDomElement root = doc.firstChildElement();

    QDomNodeList items = root.elementsByTagName("LengteInterval");
    qDebug() << "index = " << index;
    QDomNode itemnode = items.at(index);
    if(itemnode.isElement()){

        QDomElement itemele = itemnode.toElement();
        qDebug() << itemele.attribute("ID");
        QDomNodeList inf = itemele.childNodes();
        qDebug() << kleur;
        QDomNode waarde = inf.at(raam);

        if(waarde.isElement()){
            qDebug() << "isel";
            QDomElement waardeElement = waarde.toElement();
            QString waardeString = waardeElement.attribute(kleurVe.at(kleur));
            standaardWaarde = waardeString.toDouble();
            qDebug() << standaardWaarde;

        }
    }

    if(ui->kleurCombobox->currentIndex()==1 || ui->kleurCombobox->currentIndex()==2){
        temp =QCoreApplication::applicationDirPath() + "/MeerPrijsPA.xml";
        file.setFileName(temp);

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            qDebug() << "Failed to open - 1";
        }else{
            if(!doc.setContent(&file)){
                qDebug() << "Failed to load - 2";
            }
            file.close();
        }
        root = doc.firstChildElement();

        items = root.elementsByTagName("TypeIsolatie");

        qDebug() <<"grootte:"<< items.size();
        itemnode = items.at(ui->kleurCombobox->currentIndex()-1);
        if(itemnode.isElement()){
            QDomElement meerP = itemnode.toElement();
            standaardWaarde *= meerP.attribute("eerstepercentage").toDouble()/100 + 1;
            standaardWaarde *= meerP.attribute("tweedepercentage").toDouble()/100 + 1;

        }else qDebug() << "geen elementç lalal";
    }



    double debietWaarde;
    //C:/Users/Gilles/Documents/QTProjects/DucoVentilatieRooster/

    temp =QCoreApplication::applicationDirPath() + "/MeerPrijsXML.xml";
    file.setFileName(temp);
    //QFile file(temp);
    qDebug() << file.fileName();
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Failed to open - 1";
    }else{
        if(!doc.setContent(&file)){
            qDebug() << "Failed to load - 2";
        }
        file.close();
    }
    root = doc.firstChildElement();

    items = root.elementsByTagName("TypeIsolatie");
    qDebug() << "index = " << index;
    itemnode = items.at(isolatie);
    if(itemnode.isElement()){
        QDomElement itemeles = itemnode.toElement();
        standaardWaarde += ((double)lengte/1000)*itemeles.attribute(raamVe.at(raam)).toDouble();
        debietWaarde = itemeles.attribute("q").toDouble();
        qDebug() << "debietwaarde: " << debietWaarde;
    }else qDebug() << "geen elementç";



    if(lengte>4000)debietWaarde *=4.02;
    else {
        qDebug() << debietWaarde;
        debietWaarde *= ((double)lengte/1000 +.02);
    }





    currentInfo.push_back(ui->kamerComboBox->currentText());
    currentInfo.push_back(ui->topBoxCombobox->currentText());
    currentInfo.push_back(ui->raamCombobox->currentText());
    currentInfo.push_back(ui->isolatieCombobox->currentText());
    currentInfo.push_back(ui->kleurCombobox->currentText());
    currentInfo.push_back(QString::number(lengte) + " mm");
    currentInfo.push_back(QString::number((int)debietWaarde));
    currentInfo.push_back(QString::number(aantal));
    currentInfo.push_back(QString::number(standaardWaarde));
    currentInfo.push_back(QString::number(aantal*standaardWaarde));
    som.push_back(aantal*standaardWaarde);
    QString tekst = ui->kamerComboBox->currentText() + " / " + ui->topBoxCombobox->currentText() + " / " + ui->raamCombobox->currentText() + " / " + ui->kleurCombobox->currentText() + " / Lengte = " + QString::number(lengte) + "mm / Debiet = " + QString::number((int)debietWaarde) + "m^3/u / adviesprijs = " + QString::number(standaardWaarde) + " x " + QString::number(aantal);
    ui->pbLineEdit->setText(tekst);
}

void MainWindow::on_kleurCombobox_currentIndexChanged(int index)
{
    calculateEv();
}

void MainWindow::on_isolatieCombobox_currentIndexChanged(int index)
{
    calculateEv();
}

void MainWindow::on_raamCombobox_currentIndexChanged(int index)
{
    calculateEv();
}

void MainWindow::on_lengtSpinbox_editingFinished()
{
    calculateEv();
}

void MainWindow::on_klemBord_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    double tot = 0.0;
    QString klip = "ruimte\tomschrijving\tdebiet(m";
    klip += QChar(0x00B3);
    klip +=      "/u)\taantal\tadviesprijs\ttotaal\r\n";
    for(int i = 0; i < allInfo.size(); i++){
        klip+=allInfo.at(i).at(0) + "\t";
        for(int s = 1; s <6; s++){
            if(s !=5){
                klip+=allInfo.at(i).at(s) + " / ";
            }else klip+=allInfo.at(i).at(s);
        }
        //          debiet                     aantal                    eenheidsprijs            totaalprijs
        klip+="\t"+ allInfo.at(i).at(6) + "\t"+allInfo.at(i).at(7)+ "\t"+allInfo.at(i).at(8)+"\t"+allInfo.at(i).at(9)+"\r\n";
        tot+=som.at(i);
    }
    klip+="\t\t\t\ttotaal:\t"+QString::number(tot);
    clipboard->setText(klip);
}

void MainWindow::on_kamerComboBox_activated(const QString &arg1)
{
    calculateEv();
}

void MainWindow::on_deleteButton_clicked()
{
    int index = ui->finaalWaardes->currentRow();
    delete ui->finaalWaardes->takeItem(index);
    allInfo.erase(allInfo.begin()+index);
    som.erase(som.begin()+index);
}
void MainWindow::aboutMenuSignal(QWidget *){
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_aantalSpinBox_editingFinished()
{
    calculateEv();
}
