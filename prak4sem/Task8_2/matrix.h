#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

class matrix
{
private:
    int nRow;     // число строк
    int nColum;   // число столбцов
    double *data; // храним матрицу как  одномерный масив, индексы будет счиатть как m[i][j] есть data[i * nColum + j]
public:
    // класс исключений
    class error : public std::runtime_error
    {
    public:
        error(const std::string &msg) : std::runtime_error(msg) {}
    };
    // констуркторы
    matrix(int n, int m);       // конструктор матрицы размера n ́m со значениями 0.0
    matrix(double val);         // матрица 1 ́1 с этим элементом
    matrix(double *arr, int m); // матрица - строка из массива длины m
    matrix(int n, double *arr); // матрица - столбец из массива длины n
    matrix(char *str);          // из строкового представления(см.выше)
    // констуктор копирования и операция присваивания
    matrix(const matrix &other);
    matrix &operator=(const matrix &other);
    //  деструктор
    ~matrix();
    // статические методы
    static matrix identity(int n);               // возвращает единичную матрицу размера n
    static matrix diagonal(double *vals, int n); // возвращает диагональную матрицу размера n с заданными элементами по главной диагонали
    // методы доступа
    int rows() const;                   // число строк
    int colums() const;                 // число столбцов
    void set(int i, int j, double val); // присвоить значение элементу[i][j]
    double get(int i, int j) const;     // значение элемента[i][j]
    // оператор []
    matrix operator[](int index) const;
    // операторы скалярного умножения
    matrix operator*(int val) const;
    matrix &operator*=(int val);
    // оператор вывод
    friend std::ostream &operator<<(std::ostream &os, const matrix &mtrx);
};

#endif
