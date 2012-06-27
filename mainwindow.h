#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);


    QList<double> presArray;
    QList<double> voltArray;
    QList<double> mins;
    QList<double> maxs;
    QList<double> TSort;
    QString filepath;

    QList<double> TMin;
    QList<double> TMax;

    int N;
    double stress, sense, errorSense, errorStress;

    QList<QString> dblToStrList(QList<double>* list);
    double findMin(QList<double> *list);
    double findMax(QList<double> *list);
    QList<double> derivate(QList<double>* xList, QList<double>* yList);
    QList<double> periodCalc(QList<double>* array);
    QList<double> multiplyArray(QList<double>* array, int value);
    double findLinearMean(QList<double>* list, double val);
    double findLinearCount(QList<double>* list, double val);
    void stressSenseCalc();
    QList<double> takeFromPairList(QList<QPair<double, double> >* source, int b);
    int biggerArraySize(QList<double>* array1, QList<double>* array2);
    int approxIndex(QList<double> *xList, QList<double>* yList);
    double findMinIndex(QList<double>* list);
    double approxKoef(QList<double> *xList, QList<double>* yList);
    double approxAddKoef(QList<double> *xList, QList<double>* yList);

        ~MainWindow();

private slots:
    void on_openButton_clicked();
    void drawGraphicVoltPres();
    void calculate();    
    void writePeriodsFiles();
    void writeResultFile();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
