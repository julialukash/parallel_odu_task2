#ifndef DERIVATOR_H
#define DERIVATOR_H

#include "Interface.h"
#include <algorithm>

class ApproximateOperations
{
private:
    std::shared_ptr<NetModel> netModel;
public:
    ApproximateOperations(std::shared_ptr<NetModel> model)
    {
        netModel = model;
    }

    // note: calculates -laplass(currentValues)
    DoubleMatrix CalculateLaplass(const DoubleMatrix& currentValues)
    {
        auto laplassValues = DoubleMatrix(netModel->xPointsCount, netModel->yPointsCount);
        for (auto i = 1; i < laplassValues.rowsCount() - 1; ++i)
        {
            for (auto j = 1; j < laplassValues.colsCount() - 1; ++j)
            {
                auto xPart = (currentValues(i, j) - currentValues(i - 1, j)) / netModel->xStep(i - 1) -
                             (currentValues(i + 1, j) - currentValues(i, j)) / netModel->xStep(i);
                auto yPart = (currentValues(i, j) - currentValues(i, j - 1)) / netModel->yStep(i - 1) -
                             (currentValues(i, j + 1) - currentValues(i, j)) / netModel->yStep(i);
                laplassValues(i, j) = xPart / netModel->xAverageStep(i) + yPart / netModel->yAverageStep(j);
            }
        }
        return laplassValues;
    }

    double ScalarProduct(const DoubleMatrix& currentValues, const DoubleMatrix& otherValues)
    {
        double prodValue = 0;
        for (auto i = 1; i < currentValues.rowsCount() - 1; ++i)
        {
            for (auto j = 1; j < currentValues.colsCount() - 1; ++j)
            {
                prodValue = prodValue + netModel->xAverageStep(i) * netModel->yAverageStep(j) *
                                        currentValues(i, j) * otherValues(i, j);
            }
        }
        return prodValue;
    }

    double NormValueEq(const DoubleMatrix& currentValues)
    {
        return sqrt(ScalarProduct(currentValues, currentValues));
    }


    double NormValue(const DoubleMatrix& currentValues)
    {
        return fabs(*std::max_element(&(currentValues.matrix[0][0]),
                &(currentValues.matrix[0][0]) + currentValues.rowsCount() * currentValues.colsCount()));
    }

};

#endif // DERIVATOR_H