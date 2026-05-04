#include "matrix.h"
#include <iostream>
#include <string.h>
// #include <err.h>

//  конструкторы

// конструктор матрицы размера n ́m со значениями 0.0
matrix::matrix(int n, int m) : nRow(n), nColum(m)
{
    if (n <= 0 || m <= 0)
    {
        throw matrix::error("Size of matrix error");
    }
    data = new double[n * m];
    for (int i = 0; i < n * m; i++)
    {
        data[i] = 0.0;
    }
}

// матрица 1 ́1 с этим элементом
matrix::matrix(double val) : nRow(1), nColum(1)
{
    data = new double[1];
    data[0] = val;
}

// матрица - строка из массива длины m
matrix::matrix(double *arr, int m) : nRow(1), nColum(m)
{
    if (m <= 0)
    {
        throw matrix::error("Size of matrix error");
    }
    data = new double[m];
    for (int i = 0; i < m; i++)
    {
        data[i] = arr[i];
    }
}

// матрица - столбец из массива длины n
matrix::matrix(int n, double *arr) : nRow(n), nColum(1)
{
    if (n <= 0)
    {
        throw matrix::error("Size of matrix error");
    }
    data = new double[n];
    for (int i = 0; i < n; i++)
    {
        data[i] = arr[i];
    }
}

// из строкового представления
//{{1,2,3},{1,2,3}}
matrix::matrix(char *str)
{
    char *l = str;
    int len = 0;
    while (*l != '\0')
    {
        len++;
        l++;
    }
    if (len <= 4 || str[0] != '{' || str[1] != '{' || str[len - 1] != '}' || str[len - 2] != '}')
    {
        throw matrix::error("Size of matrix error");
    }
    // число строк
    int cR = 1;
    for (int i = 0; i < len - 1; i++)
    {
        if (str[i] == '}' && str[i + 1] == ',')
        {
            i++;
            cR++;
        }
    }
    nRow = cR;
    // число строк счиатем как кол-во запятых в первой строке, для этого для начала определяем длину первой строки
    int cC = 1;
    int indFirstR = 0;
    for (int i = 2; i < len; i++)
    {
        if (str[i] == '}')
        {
            indFirstR = i;
            break;
        }
    }
    if (indFirstR == 0)
    {
        throw matrix::error("Size of matrix error");
    }
    for (int i = 2; i < indFirstR; i++)
    {
        if (str[i] == ',')
        {
            cC++;
        }
    }
    nColum = cC;
    // память для массива
    data = new double[nRow * nColum];
    // парсим строку для заполнения массива
    char *p = str;
    p += 2;
    int index = 0; // индекс в data
    // движемяс по строкам потом по столбцам, заполняем как m[i][j] есть data[i * nColum + j]
    for (int i = 0; i < nRow; i++)
    {
        for (int j = 0; j < nColum; j++)
        {
            while (*p == ' ' || *p == ',' || *p == '{' || *p == '}')
            {
                p++;
            }
            bool neg = false;
            if (*p == '-')
            {
                neg = true;
                p++;
            }
            else if (*p == '+')
            {
                p++;
            }
            double numeric = 0.0;
            // парсим целую часть
            while (*p >= '0' && *p <= '9')
            {
                numeric = numeric * 10 + (*p - '0');
                p++;
            }
            // парсим дробную если есть
            if (*p == '.')
            {
                p++;
                double fract = 0.0;
                double constant = 10.0;
                while (*p >= '0' && *p <= '9')
                {
                    fract = fract * 10 + (*p - '0');
                    constant *= 10;
                    p++;
                }
                numeric = numeric + (fract / constant);
            }
            if (neg)
            {
                numeric *= -1;
            }
            data[index++] = numeric;
        }
        while (*p == ' ' || *p == ',' || *p == '{' || *p == '}')
        {
            p++;
        }
    }
}

// конструктор копирования
matrix::matrix(const matrix &other) : nRow(other.nRow), nColum(other.nColum)
{
    data = new double[nRow * nColum];
    for (int i = 0; i < nRow * nColum; i++)
    {
        data[i] = other.data[i];
    }
}

// переопределяем оператор присваивания
matrix &matrix ::operator=(const matrix &other)
{
    if (this == &other)
    {
        return *this;
    }
    delete[] data;
    nRow = other.nRow;
    nColum = other.nColum;
    data = new double[nRow * nColum];
    for (int i = 0; i < nRow * nColum; i++)
    {
        data[i] = other.data[i];
    }

    return *this;
}

matrix::~matrix()
{
    delete[] data;
}

// статические методы
matrix matrix::identity(int n)
{
    if (n <= 0)
    {
        throw matrix::error("Size of matrix error");
    }
    matrix m(n, n);
    for (int i = 0; i < n; i++)
    {
        m.set(i, i, 1.0);
    }
    return m;
}

matrix matrix::diagonal(double *vals, int n)
{
    if (n <= 0 || vals == nullptr)
    {
        throw matrix::error("Size of matrix error");
    }
    matrix m(n, n);
    for (int i = 0; i < n; i++)
    {
        m.set(i, i, vals[i]);
    }
    return m;
}

int matrix::rows() const
{
    return nRow;
}

int matrix::colums() const
{
    return nColum;
}

void matrix::set(int i, int j, double val)
{
    if (i < 0 || j < 0 || i >= nRow || j >= nColum)
    {
        throw matrix::error("Size of matrix error");
    }
    data[i * nColum + j] = val;
}

double matrix::get(int i, int j) const
{
    if (i < 0 || j < 0 || i >= nRow || j >= nColum)
    {
        throw matrix::error("Size of matrix error");
    }
    return data[i * nColum + j];
}

matrix matrix::operator[](int index) const
{
    // если имеет более одной строки то рассматриваем индекс как номер строки
    if (nRow > 1)
    {
        if (index < 0 || index >= nRow)
        {
            throw matrix::error("Size of matrix error");
        }
        matrix m(1, nColum);
        for (int i = 0; i < nColum; i++)
        {
            m.set(0, i, data[index * nColum + i]);
        }
        return m;
    }
    else if (nRow == 1)
    {
        if (index < 0 || index >= nColum)
        {
            throw matrix::error("Size of matrix error");
        }
        matrix m(data[index]);
        return m;
    }
    else
    {
        throw matrix::error("Size of matrix error");
    }
}

matrix matrix::operator*(int val) const
{
    matrix m(nRow, nColum);
    for (int i = 0; i < nRow; i++)
    {
        for (int j = 0; j < nColum; j++)
        {
            m.set(i, j, get(i, j) * val);
        }
    }
    return m;
}

matrix &matrix::operator*=(int val)
{
    for (int i = 0; i < nRow; i++)
    {
        for (int j = 0; j < nColum; j++)
        {
            set(i, j, get(i, j) * val);
        }
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const matrix &mtrx)
{
    for (int i = 0; i < mtrx.rows(); i++)
    {
        for (int j = 0; j < mtrx.colums(); j++)
        {
            os << mtrx.get(i, j) << " ";
        }
        os << "\n";
    }
    return os;
}