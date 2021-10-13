//
// Created by Artem Poliakov on 12.10.2021.
//

#include <cassert>
#include "RationalMatrix.h"

RationalMatrix::RationalMatrix(const size_t &_rows, const size_t &_cols)
{
    matrix = {};
    rows = 0;
    cols = 0;
    this->resize(_rows, _cols);
}
RationalMatrix::RationalMatrix(const std::vector<std::vector<Rational>> &_matrix)
{
    matrix = {};
    rows = 0;
    cols = 0;
    this->resize(_matrix.size(), _matrix[0].size());
    for(size_t i=0; i<rows; i++)
    {
        assert(_matrix[i].size()==cols);
        for(size_t j=0; j<cols; j++)
        {
            matrix[i][j] = _matrix[i][j];
        }
    }
}
void RationalMatrix::resize(const size_t &_rows, const size_t &_cols)
{
    assert(_rows!=0 && _cols!=0);
    matrix.resize(_rows);
    for(auto &row: matrix)
    {
        row.resize(_cols);
    }
    rows = _rows;
    cols = _cols;
}
void RationalMatrix::swapRows(const size_t &row1, const size_t &row2)
{
    assert(row1<rows && row2<rows);
    swap(matrix[row1], matrix[row2]);
}
std::vector<Rational>& RationalMatrix::operator ()(const size_t &row)
{
    assert(row<rows);
    return matrix[row];
}
Rational& RationalMatrix::operator ()(const size_t &row, const size_t &col)
{
    assert(row<rows && col<cols);
    return matrix[row][col];
}
RationalMatrix RationalMatrix::inverse()
{
    assert(rows==cols); //перевірка, що матриця квадратна
    RationalMatrix tmp = *this; //копіювання поточної матриці для проведення перетворень без її зміни
    RationalMatrix res(rows, cols); //створення результуючої матриці такого ж розміру (поки що нульової)
    for(size_t i=0; i<rows; i++) res(i,i)=Rational(1,1); //перетворення результуючої матриці на одиничну
    for(size_t i=0; i<rows; i++) //цикл по всім рядкам
    {
        if(tmp(i,i).numerator==0) //діагональний елемент = 0 (треба переставляти рядки)
        {
            size_t c = i+1;
            while(tmp(c,i).numerator==0) //пошук першого не нульового елемента в тому ж стовпці
            {
                i++;
                if(c==rows) return RationalMatrix(0,0); //якщо такого елемента немає - помилка (повертається порожня матриця)
            }
            this->swapRows(i,c);
        }
        tmp(i) = tmp(i)/tmp(i,i); //перетворення діагонального елемента на 1
        res(i) = res(i)/tmp(i,i);
        for(size_t j=0; i<rows; j++)
        {
            if(j==i) continue;
            tmp(j) = tmp(j)-tmp(i)*tmp(j,i);
            res(j) = res(j)-res(i)*tmp(j,i);
        }
    }
    return res;
}

std::vector<Rational> operator +(const std::vector<Rational> &row1, const Rational &k)
{
    size_t len = row1.size();
    std::vector<Rational> res(len);
    for(size_t i=0; i<len; i++)
    {
        res[i] = row1[i]+k;
    }
    return res;
}
std::vector<Rational> operator -(const std::vector<Rational> &row1, const Rational &k)
{
    return row1+(k*Rational{-1,1});
}
std::vector<Rational> operator *(const std::vector<Rational> &row1, const Rational &k)
{
    size_t len = row1.size();
    std::vector<Rational> res(len);
    for(size_t i=0; i<len; i++)
    {
        res[i] = row1[i]*k;
    }
    return res;
}
std::vector<Rational> operator /(const std::vector<Rational> &row1, const Rational &k)
{
    size_t len = row1.size();
    std::vector<Rational> res(len);
    for(size_t i=0; i<len; i++)
    {
        res[i] = row1[i]/k;
    }
    return res;
}
std::vector<Rational> operator +(const std::vector<Rational> &row1, const std::vector<Rational> &row2)
{
    size_t len = row1.size();
    assert(row2.size()==len);
    std::vector<Rational> res(len);
    for(size_t i=0; i<len; i++)
    {
        res[i] = row1[i]+row2[i];
    }
    return res;
}
std::vector<Rational> operator -(const std::vector<Rational> &row1, const std::vector<Rational> &row2)
{
    return row1+(row2*Rational{-1,1});
}
std::vector<Rational> operator *(const std::vector<Rational> &row1, const std::vector<Rational> &row2)
{
    size_t len = row1.size();
    assert(row2.size()==len);
    std::vector<Rational> res(len);
    for(size_t i=0; i<len; i++)
    {
        res[i] = row1[i]*row2[i];
    }
    return res;
}
std::vector<Rational> operator /(const std::vector<Rational> &row1, const std::vector<Rational> &row2)
{
    size_t len = row1.size();
    assert(row2.size()==len);
    std::vector<Rational> res(len);
    for(size_t i=0; i<len; i++)
    {
        res[i] = row1[i]/row2[i];
    }
    return res;
}

std::ostream& operator <<(std::ostream &ofs, const RationalMatrix &matrix)
{
    for(auto &row: matrix.matrix)
    {
        for(auto &col: row)
        {
            ofs << col << " ";
        }
        ofs << std::endl;
    }
    return ofs;
}