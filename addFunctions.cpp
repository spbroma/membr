#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"

QList<QString> MainWindow::dblToStrList(QList<double>* list)
{
    QList<QString> stringList;
    for(int i = 0; i < list->size(); i++)
    {
        stringList.append(QString::number(list->at(i)));
    }
    return stringList;
}

double MainWindow::findMax(QList<double>* list)
{
    double Max = list->at(0);
    for(int i = 0; i < list->size(); i++)
    {
        if(list->at(i) > Max)
            Max = list->at(i);
    }
    return Max;
}


double MainWindow::findMin(QList<double>* list)
{
    double Min = list->at(0);
    for(int i = 0; i < list->size(); i++)
    {
        if(list->at(i) < Min)
            Min = list->at(i);
    }
    return Min;
}

double MainWindow::findMinIndex(QList<double>* list)
{
    double Min = list->at(0);
    int index;
    for(int i = 0; i < list->size(); i++)
    {
        if(list->at(i) < Min)
        {
            Min = list->at(i);
            index = i;
        }

    }
    return index;
}

QList<double> MainWindow::derivate(QList<double>* xList, QList<double>* yList)
{
    double dx, dy, dydx;
    int i;

    QList<double> derivativeList;

    for (i = 0; i < xList->size()-1; i++)
    {
        dx = xList->at(i+1) - xList->at(i);
        dy = yList->at(i+1) - yList->at(i);
        dydx = dy/fabs(dx);
        derivativeList.append(dydx);
    }
    return derivativeList;
}

QList<double> MainWindow::periodCalc(QList<double> *array)
{
    QList<double> periods;
    periods.clear();
    for (int i = 0; i < array->size()-1; i++)
    {
        periods.append(array->at(i+1) - array->at(i));
    }
    return periods;
}

QList<double> MainWindow::multiplyArray(QList<double> *array, int value)
{
    QList<double> multiplied;
    multiplied.clear();

    for (int i = 0; i < array->size(); i++)
    {
        multiplied.append(array->at(i)*value);
    }
    return multiplied;
}

double MainWindow::findLinearMean(QList<double>* list, double val)
{
    double mean, sum, difference, min, max;
    sum = 0;
    mean = 0;
    difference = 0;
    min = 0;
    max = 0;

    QList<double> tempList;
    tempList.clear();
    int count;

        for(int i = 0; i < list->size(); i++)
        {
            sum = 0;
            tempList.clear();
            for (int j = 0; j < i+1; j++)
            {
                tempList.append(list->at(j));
                sum += list->at(j);
                mean = sum/(j+1);
            }
            min = findMin(&tempList);
            max = findMax(&tempList);
            difference = max - min;

            count = i+1;
            if (difference > val*mean)
                break;
        }
    return mean;
}

double MainWindow::findLinearCount(QList<double>* list, double val)
{
    double mean, sum, difference, min, max;
    sum = 0;
    mean = 0;
    difference = 0;
    min = 0;
    max = 0;

    int count;

    QList<double> tempList;
    tempList.clear();

        for(int i = 0; i < list->size(); i++)
        {
//            double valMean = val*mean;
            sum = 0;
            tempList.clear();
            for (int j = 0; j < i+1; j++)
            {
                tempList.append(list->at(j));
                sum += list->at(j);
                mean = sum/(j+1);
            }
            min = findMin(&tempList);
            max = findMax(&tempList);
            difference = max - min;

            count = i+1;
            if (difference > val*mean)
                break;
        }
    return count;
}

QList<double> MainWindow::takeFromPairList(QList<QPair<double, double> >* source, int b)
{
    QList<double> result;
    result.clear();
    double tempItem;

    for(int i = 0; i < source->size(); i++)
    {
        if(b == 0)
            tempItem = source->at(i).first;
        else
            tempItem = source->at(i).second;
        result.append(tempItem);
    }
    return result;
}

void MainWindow::stressSenseCalc()
{

}

int MainWindow::biggerArraySize(QList<double> *array1, QList<double> *array2)
{
    if(array1->size() > array2->size())
        return array1->size();
    else
        return array2->size();
}

int MainWindow::approxIndex(QList<double> *xList, QList<double>* yList)
{
    QList<double> a;
    int n;
    double sumXY, sumX, sumY, sumX2;

    for(int i = 0; i < yList->size(); i++)
    {
        n = i+1;
        sumXY = 0;
        sumX = 0;
        sumY = 0;
        sumX2 = 0;
        for (int j = 0; j<n; j++)
        {
            sumXY += xList->at(j+1)*yList->at(j);
            sumX += xList->at(j+1);
            sumY += yList->at(j);
            sumX2 += pow(xList->at(j), 2);
        }
        a.append((n*sumXY - sumX*sumY)/(n*sumX2 - pow(sumX, 2)));
    }
    int minIndex = findMinIndex(&a);
    return minIndex;
}

double MainWindow::approxKoef(QList<double> *xList, QList<double>* yList)
{
    QList<double> a;
    QList<double> b;
    a.clear();
    b.clear();

    int n;
    double sumXY, sumX, sumY, sumX2;

    for(int i = 0; i < yList->size(); i++)
    {
        n = i+1;
        sumXY = 0;
        sumX = 0;
        sumY = 0;
        sumX2 = 0;
        for (int j = 0; j<n; j++)
        {
            sumXY += xList->at(j+1)*yList->at(j);
            sumX += xList->at(j+1);
            sumY += yList->at(j);
            sumX2 += pow(xList->at(j), 2);
        }
        a.append((n*sumXY - sumX*sumY)/(n*sumX2 - pow(sumX, 2)));
        b.append((sumY - a.at(i)*sumY)/n);
    }
    int minIndex = findMinIndex(&a);
    return a.at(minIndex);
}

double MainWindow::approxAddKoef(QList<double> *xList, QList<double>* yList)
{
    QList<double> a;
    QList<double> b;
    a.clear();
    b.clear();

    int n;
    double sumXY, sumX, sumY, sumX2;

    for(int i = 0; i < yList->size(); i++)
    {
        n = i+1;
        sumXY = 0;
        sumX = 0;
        sumY = 0;
        sumX2 = 0;
        for (int j = 0; j<n; j++)
        {
            sumXY += xList->at(j+1)*yList->at(j);
            sumX += xList->at(j+1);
            sumY += yList->at(j);
            sumX2 += pow(xList->at(j), 2);
        }
        a.append((n*sumXY - sumX*sumY)/(n*sumX2 - pow(sumX, 2)));
        b.append((sumY - a.at(i)*sumY)/n);
    }
    int minIndex = findMinIndex(&a);
    return b.at(minIndex);
}

