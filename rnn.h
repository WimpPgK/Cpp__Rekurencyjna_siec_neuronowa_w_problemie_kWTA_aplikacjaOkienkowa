#ifndef RNN_H
#define RNN_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <QTextEdit>

using namespace std;

class RNN
{


public:
    int *u;
    bool kWTA(double epsilon, int n, int k, int maxiter, QTextEdit *terminal_QTextEdit, bool fullPrint, bool repeat);
    void generateNumber(int n);
    void loadNumber(int* tab, int n);
    ~RNN();
};

#endif // RNN_H
