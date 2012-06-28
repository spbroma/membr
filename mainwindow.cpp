#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  

    ui->toleranceSlider->setVisible(false);
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this,
            SLOT(drawGraphicVoltPres()));
    connect(ui->verticalSlider, SIGNAL(valueChanged(int)), this,
            SLOT(drawGraphicVoltPres()));
    connect(ui->toleranceSlider, SIGNAL(valueChanged(int)), this,
            SLOT(calculate()));
    connect(ui->WScaleSlider, SIGNAL(valueChanged(int)), this,
            SLOT(calculate()));
    connect(ui->dotSizeSlider, SIGNAL(valueChanged(int)), this,
            SLOT(calculate()));
    connect(ui->thicknessBox, SIGNAL(valueChanged(int)), this,
            SLOT(calculate()));
    connect(ui->saveTbutton, SIGNAL(pressed()), this, SLOT(writePeriodsFiles()));
    connect(ui->saveResultButton, SIGNAL(pressed()), this, SLOT(writeResultFile()));
    ui->horizontalSlider->setDisabled(true);
    ui->verticalSlider->setDisabled(true);
    ui->toleranceSlider->setDisabled(true);
    ui->dotSizeSlider->setDisabled(true);
    ui->WScaleSlider->setDisabled(true);
    ui->openButton->setDefault(true);
    ui->saveResultButton->setDisabled(true);
    ui->saveTbutton->setDisabled(true);
    double blabla;
    blabla = 0;

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_openButton_clicked()
{
    filepath = QFileDialog::getOpenFileName(0,"Âûáåðèòå ôàéë");
//    filepath = "H:/Qt/membraneMeasurement/membr2-3";
//    filepath = "C:/Users/roma/Dropbox/Docs/p/membraneMeasurement/membr2-3";

    if (!filepath.isEmpty())
    {
        ui->horizontalSlider->setEnabled(true);
        ui->verticalSlider->setEnabled(true);
        ui->toleranceSlider->setEnabled(true);
        ui->dotSizeSlider->setEnabled(true);
        ui->WScaleSlider->setEnabled(true);
        ui->openButton->setDefault(false);
        ui->saveResultButton->setEnabled(true);
        ui->saveTbutton->setEnabled(true);

        QFile file(filepath);
        file.open(QIODevice::ReadOnly);
        ui->filenameLine->setText(filepath.section('/', -3, -1));

        QString rawData;
        QList<QString> rawDataArray;


        presArray.clear();
        voltArray.clear();
        rawDataArray.clear();
        rawData.clear();

        rawData = file.readAll();
        file.close();

        rawDataArray = rawData.split("\n");

        if(rawDataArray.last().isEmpty())
        {
            while(rawDataArray.last().isEmpty())
                rawDataArray.removeLast();
        }

        N = rawDataArray.size();


        for (int i = 0; i < N; i++)
        {
            presArray.append(rawDataArray.at(i).split("\t").at(0).toDouble());
            voltArray.append(rawDataArray.at(i).split("\t").at(1).toDouble());
        }

        drawGraphicVoltPres();
        calculate();

    }


}

void MainWindow::drawGraphicVoltPres()
{

    QList<double> dVdPArray;
    double min, max, zero;

    mins.clear();
    maxs.clear();

    QGraphicsScene* scene = new QGraphicsScene;
    scene->clear();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing,true);

    int xScale = ui->horizontalSlider->value();
    int yScale = ui->verticalSlider->value();

    min = findMin(&voltArray);
    max = findMax(&voltArray);
    zero = -(min + max)*yScale/2;

//    temp



    dVdPArray = derivate(&presArray, &voltArray);

    double x, y, pt, pt2, dy, yMaxExt, yMinExt;
    yMaxExt = zero;
    yMinExt = zero;

    double rise, fall;
    rise = -10000;
    fall = +10000;

    pt = 1;
    pt2 = 5*pt;

    for (int i = 0; i < N-1; i++)
    {
        x = presArray.at(i)*xScale;
        y = -voltArray.at(i)*yScale;
        dy = -dVdPArray.at(i)*yScale;

        scene->addEllipse(x-pt/2, y-pt/2, pt, pt,QPen((Qt::black)),QBrush(Qt::black));

        if (y <= zero)
        {
            if (dy < 0)
                fall = y;

            if (fall < zero)
            {
                if (dy > 0)
                {
                    if (y < yMaxExt)
                    {
                        yMaxExt = y;
                        maxs.append(presArray.at(i));
                        scene->addEllipse(x-pt2/2, y-pt2/2, pt2, pt2,QPen((Qt::red)),QBrush(Qt::red));
                    }
                }
            }


        }
        else
            yMaxExt = zero;

        if (y >= zero)
        {
            if (dy > 0)
                rise = y;

            if (rise > zero)
            {
                if (dy < 0)
                {
                    if (y > yMinExt)
                    {
                        yMinExt = y;
                        mins.append(presArray.at(i));
                        scene->addEllipse(x-pt2/2, y-pt2/2, pt2, pt2,QPen((Qt::green)),QBrush(Qt::green));
                    }
                }
            }


        }
        else
            yMinExt = zero;
    }

    bool minIsFirst;
    minIsFirst = mins.at(0) < maxs.at(0);


}

void MainWindow::calculate()
{    
    QList<double> tStud;
    tStud << 12.7062 << 4.3027 << 3.1824 << 2.7764 << 2.5706 << 2.4469
          <<  2.3646 << 2.3060 << 2.2622 << 2.2281 << 2.2010 << 2.1788
          <<  2.1604 << 2.1448 << 2.1314 << 2.1199 << 2.1098 << 2.1009
          <<  2.0930 << 2.0860 << 2.0796 << 2.0739 << 2.0687 << 2.0639
          <<  2.0595 << 2.0555 << 2.0518 << 2.0484 << 2.0452 << 2.0423
          <<  2.0395 << 2.0369 << 2.0345 << 2.0322 << 2.0301 << 2.0281;
    for (int i = 0; i < 100; i++)
    {
        tStud.append(2.0);
    }


    TMin.clear();
    TMax.clear();
    TSort.clear();

    ui->TMinList->clear();
    ui->TMaxList->clear();

    TMin = periodCalc(&mins);
    TMax = periodCalc(&maxs);


    TMin = multiplyArray(&TMin, 1000);
    TMax = multiplyArray(&TMax, 1000);


    ui->realToleranceLabel->setText(QString::number(ui->toleranceSlider->value()/100) + '%');

    //    temp
    ui->TMinList->addItems(dblToStrList(&TMin));
    ui->TMaxList->addItems(dblToStrList(&TMax));

    double linearMinMean, linearMaxMean, toleranceKoef;


    toleranceKoef = ui->toleranceSlider->value()/10000.0;

    linearMinMean = findLinearMean(&TMin, toleranceKoef);
    linearMaxMean = findLinearMean(&TMax, toleranceKoef);

//     ÌÍÊ
    int linMaxIndex = approxIndex(&maxs, &TMax);
    int linMinIndex = approxIndex(&mins, &TMin);
    double linMaxKoef = approxKoef(&maxs, &TMax);
    double linMinKoef = approxKoef(&mins, &TMin);
    double linMaxAddKoef = approxAddKoef(&maxs, &TMax);
    double linMinAddKoef = approxAddKoef(&mins, &TMin);

    double linMinMean, linMaxMean;
    linMinMean = 0;
    linMaxMean = 0;

    for(int i = 0; i <= linMaxIndex; i++)
    {
        linMaxMean += TMax.at(i);
    }
    linMaxMean = linMaxMean/(linMaxIndex + 1);

    for(int i = 0; i <= linMinIndex; i++)
    {
        linMinMean += TMin.at(i);
    }
    linMinMean = linMinMean/(linMinIndex + 1);

    ui->linMaxIndexLabel->setText(QString::number(linMaxIndex));
    ui->linMinIndexLabel->setText(QString::number(linMinIndex));
    ui->linMaxValLabel->setText(QString::number(linMaxMean));
    ui->linMinValLabel->setText(QString::number(linMinMean));

//    / ÌÍÊ


    int linearMinCount = findLinearCount(&TMin, toleranceKoef);
    int linearMaxCount = findLinearCount(&TMax, toleranceKoef);

    ui->lineEdit_3->setText(QString::number(linearMinMean));
    ui->lineEdit_4->setText(QString::number(linearMaxMean));

    ui->countLabelMin->setText(QString::number(linearMinCount));
    ui->countLabelMax->setText(QString::number(linearMaxCount));

    QList<double> WMax; //ìêì
    QList<double> WMin; //ìêì

    WMax.clear();
    WMin.clear();


    double firstMin = mins.at(0)*1000/linearMinMean;
    double firstMax = maxs.at(0)*1000/linearMaxMean;

    WMin.append(firstMin*0.775);
    for(int i = 1; i < mins.size(); i++)
        WMin.append(WMin.at(i-1) + 0.775);

    WMax.append(firstMax*0.775);
    for(int i = 1; i < maxs.size(); i++)
        WMax.append(WMax.at(i-1) + 0.775);


    QList<QPair<double, double> > presFlexPairs;
    QPair<double, double> presFlexPair;
    presFlexPairs.clear();

    for(int i = 0; i < TMin.size(); i++)
    {
        presFlexPair.first = mins.at(i)*1000;
        presFlexPair.second = WMin.at(i);
        presFlexPairs.append(presFlexPair);
    }

    for(int i = 0; i < TMax.size(); i++)
    {
        presFlexPair.first =  maxs.at(i)*1000;
        presFlexPair.second = WMax.at(i);
        presFlexPairs.append(presFlexPair);
    }
;

    for(int j = 0; j < presFlexPairs.size(); j++)
    {
        for(int i = 0; i < presFlexPairs.size() - 1; i++)
        {
            if(presFlexPairs.at(i).first > presFlexPairs.at(i+1).first)
                presFlexPairs.swap(i, i+1);
        }
    }

    ui->WMinList->clear();
    ui->WMinList->addItems(dblToStrList(&WMin));

    ui->WMaxList->clear();
    ui->WMaxList->addItems(dblToStrList(&WMax));



    QGraphicsScene* scene2 = new QGraphicsScene;
    scene2->clear();
    ui->graphicsView_2->setScene(scene2);
    ui->graphicsView_2->setRenderHint(QPainter::Antialiasing,true);

    QTransform matrix;
    double scaleValue = ui->WScaleSlider->value()/10.0;
    matrix.scale(scaleValue,scaleValue);
    ui->graphicsView_2->setTransform(matrix);

    double pt;
    pt = ui->dotSizeSlider->value()/100.0 ;
    double x,y;

//    for(int i = 0; i < maxs.size(); i++)
//    {
//        x = maxs.at(i);
//        y = -WMax.at(i);
//        scene2->addEllipse(x-pt/2, y-pt/2, pt, pt,QPen((Qt::black)),QBrush(Qt::black));
//    }

//    for(int i = 0; i < mins.size(); i++)
//    {
//        x = mins.at(i);
//        y = -WMin.at(i);
//        scene2->addEllipse(x-pt/2, y-pt/2, pt, pt,QPen((Qt::black)),QBrush(Qt::black));
//    }

//    for(int i = 0; i < linearMinCount; i++)
//    {
//        x = mins.at(i);
//        y = -WMin.at(i);
//        scene2->addEllipse(x-pt/2, y-pt/2, pt, pt,QPen((Qt::green)),QBrush(Qt::green));
//    }

//    for(int i = 0; i < linearMaxCount; i++)
//    {
//        x = maxs.at(i);
//        y = -WMax.at(i);
//        scene2->addEllipse(x-pt/2, y-pt/2, pt, pt,QPen((Qt::red)),QBrush(Qt::red));
//    }

//    Ïåðèîäû
    for(int i = 0; i < TMax.size(); i++)
    {
        x = maxs.at(i+1);
        y = -TMax.at(i)/50;
        scene2->addEllipse(x-pt/2, y-pt/2, pt, pt,QPen((Qt::red)),QBrush(Qt::red));
    }

    for(int i = 0; i < TMin.size(); i++)
    {
        x = mins.at(i+1);
        y = -TMin.at(i)/50;
        scene2->addEllipse(x-pt/2, y-pt/2, pt, pt,QPen((Qt::green)),QBrush(Qt::green));
    }
    for(int i = 0; i < linMinIndex; i++)
    {
        x = mins.at(i+1);
        y = -TMin.at(i)/50;
        scene2->addEllipse(x-pt/2, y-pt/2, pt, pt,QPen((Qt::blue)),QBrush(Qt::blue));
    }

    for(int i = 0; i < linMaxIndex; i++)
    {
        x = maxs.at(i+1);
        y = -TMax.at(i)/50;
        scene2->addEllipse(x-pt/2, y-pt/2, pt, pt,QPen((Qt::black)),QBrush(Qt::black));
    }

    scene2->addLine(maxs.first(), -(TMax.first()*linMaxKoef + linMaxAddKoef)/500, maxs.at(linMaxIndex), -(TMax.at(linMaxIndex)*linMaxKoef + linMaxAddKoef)/500, QPen((Qt::yellow)) );

    double a;
    int h = ui->thicknessBox->value();
    a = 0.00073;

    sense = presFlexPairs.at(linearMinCount+linearMaxCount-1).second*1000/
            presFlexPairs.at(linearMinCount+linearMaxCount-1).first;

    stress = pow(a, 2)/(sense*pow(10, -9)*h*pow(10, -9)*4.4*pow(10, 6));

//    double sense1 = 0.775/linearMaxMean;


    double RMS, difference, errorT, errorH;
    difference = 0;
    errorH = 100;
    double k = a*a*2/(4.4*1.55*pow(10, -6));

    if(linearMinCount > linearMaxCount)
    {
        for(int i = 0; i < linearMinCount; i++)
            difference += pow((linearMinMean - TMin.at(i)), 2);
        RMS = pow(difference/(linearMinCount*(linearMinCount-1)), 0.5);
        errorT = RMS*tStud.at(linearMinCount - 1);
//        errorSense = pow(pow(mins.at(0)*errorT/
//                             (linearMinMean*presFlexPairs.last().first), 2), 0.5);
        errorSense = (errorT*0.775*pow(10, 3)/pow(linearMinMean, 2));
        errorStress = k*pow(10, 9)*pow( (pow( (errorT/h) , 2) +
                                         pow( (linearMinMean*errorH/pow(h, 2)) ,2)) ,0.5);
    }
    else
    {
        for(int i = 0; i < linearMaxCount; i++)
            difference += pow((linearMaxMean - TMax.at(i)), 2);
        RMS = pow(difference/(linearMaxCount*(linearMaxCount-1)), 0.5);
        errorT = RMS*tStud.at(linearMaxCount - 1);
//        errorSense = pow(pow(maxs.at(0)*errorT*1000/
//                             (linearMaxMean*presFlexPairs.last().first), 2), 0.5);
        errorSense = (errorT*0.775*pow(10, 3)/pow(linearMaxMean, 2));
        errorStress = k*pow(10, 3)*pow( (pow( (errorT/h) , 2) +
                                         pow( (linearMaxMean*errorH/pow(h, 2)) ,2)) ,0.5);
//        errorStress = maxs.at(linearMaxCount - 1)*pow(a, 2)*pow(10, 15)/(4.4*pow(WMax.at(linearMaxCount - 1), 2)*h)        ;
    }

//    errorStress = pow(10, 18)*pow(( pow(errorSense*pow(a, 2)/(pow(sense, 2)*h*4.4) ,2) + pow(( (errorH*pow(a,2)) / (sense*pow(h, 2)* 4.4) ), 2)) , 0.5);

//    errorStress = (pow(10, -6)*a/4.4)*pow( (pow((errorH/(h*h*sense)) ,2) + pow((errorSense/(h*sense*sense)) ,2)),0.5);

    stress = round(stress/10.0)*10;
    sense = round(sense*100)/100.0;
    errorStress = round(errorStress);
    errorSense = round(errorSense*100)/100.0;

    ui->stress->setText(QString::number(stress) + "±" + QString::number(errorStress) + " ÌÏà");
    ui->sense->setText(QString::number(sense) + "±" + QString::number(errorSense) + " íì/Ïà");

}

void MainWindow::writePeriodsFiles()
{
    QString str;
    QList<QString> maxList, minList;

    str = filepath;
    if (filepath.endsWith(".txt"))
    {
        str.remove(-4, 4);
        str.append("_periods.txt");
    }
    else
        str.append("_periods.txt");
    QFile file(str);
    QTextStream stream(&file);

    maxList = dblToStrList(&TMax);
    minList = dblToStrList(&TMin);

    stream << "min\tmax\n";

    if (TMax.size() > TMin.size())
    {
        while(TMax.size() > TMin.size())
            TMin.append(0);
    }
    else
        while(TMin.size() > TMax.size())
            TMax.append(0);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
       for(int i = 0; i < biggerArraySize(&TMin, &TMax); i++)
       {
//           if(minList.at(i).isNull() || maxList.at(i).isNull())
//               if(minList.at(i).isNull())
//                   minList.append("none");
//               else
//                   maxList.append("none");
//           else
               stream<< QString::number(TMin.at(i)).replace('.', ',') + "\t" + QString::number(TMax.at(i)).replace('.', ',')+ "\n";
       }
    }
    file.close();
}

void MainWindow::writeResultFile()
{
    QString str = filepath;
    if (filepath.endsWith(".txt") )
    {
        str.remove(-4, 4);
        str.append("_result.txt");
    }
    else
        str.append("_result.txt");

    QFile file(str);
    QTextStream stream(&file);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        stream << "Íàïðÿæåíèÿ\t×óâñòâèòåëüíîñòü\n" +
                  QString::number(stress) + " ± " + QString::number(errorStress) + " ÌÏà \t" +
                  QString::number(sense) + " ± " + QString::number(errorSense) + " íì/Ïà";
    }
    file.close();
}

