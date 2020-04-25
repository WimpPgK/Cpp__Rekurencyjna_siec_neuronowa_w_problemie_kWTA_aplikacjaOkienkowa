#include "rnn.h"


bool RNN::kWTA(double epsilon, int n, int k, int maxiter, QTextEdit *terminal_QTextEdit, bool fullPrint, bool repeat)
{
    QString data;
    double sumuvec = 0;
    int zeronum = 0;
    int iternum = 0;
    terminal_QTextEdit->setText("");

    double* z = new double[n];	// wektor wartosci wyjsciowych
    double* newz = new double[n];
    for (int i = 0; i < n; i++)
    {
        z[i] = u[i];		// dla zerowej iteracji wartosci wyjsciowe poprzedniej iteracji sa sztucznie tworzone bo nie istnieja jeszcze takowe
        newz[i] = u[i];
    }


    //*******************     rekurencyjna siec neuronowa      ***********************************************
    for (int i = 0; i < maxiter; i++)
    {
         sumuvec = 0;	// suma wyjsc dla calego zbioru
         zeronum = 0;	// liczba zerowych wyjsc
         iternum = maxiter;
         for (int j = 0; j < n; j++)
         {
             sumuvec += z[j];
         }

         //wlascswa czesc uczenia sieci
         for (int j = 0; j < n; j++)
         {
             newz[j] = u[j] + z[j] - epsilon * (sumuvec - z[j]);

             if (newz[j] < 0)	// FUNKCJA AKTYWACJI ReLU
             {
                 newz[j] = 0;
                 zeronum += 1;
             }
         }

         //synchronizacja nowych wyjsc ze starymi
         for (int j = 0; j < n; j++)
         {
             z[j] = newz[j];
         }

         //wypisanie danych
         if(fullPrint)
         {
             for(int j = 0 ; j < n ; j++)
             {
                 data = "Epoka ";
                 data.append(QString::number(i));
                 data.append(" rekord ");
                 data.append(QString::number(j));
                 data.append("    x = ");
                 data.append(QString::number(u[j]));
                 data.append("    y = ");
                 data.append(QString::number(z[j]));
                 terminal_QTextEdit->append(data);
             }
         }
         else   // wyswietla tylko ostatnia iteracje
         {
             if(zeronum >= n - k || i == maxiter-1)
             {
                 for(int j = 0 ; j < n ; j++)
                 {
                     data = "Epoka ";
                     data.append(QString::number(i));
                     data.append(" rekord ");
                     data.append(QString::number(j));
                     data.append("    x = ");
                     data.append(QString::number(u[j]));
                     data.append("    y = ");
                     data.append(QString::number(z[j]));
                     terminal_QTextEdit->append(data);
                 }
              }
         }
         if (zeronum >= n - k && i != 0)
         {
             iternum = i;
             terminal_QTextEdit->append("ALGORYTM ZAKOŃCZYŁ SIĘ POWODZENIEM W EPOCE " + QString::number(i));
             return true;

         }
         else if(zeronum >= n - k && i == 0)
         {
             terminal_QTextEdit->append("ALGORYTM ZAKOŃCZYŁ SIĘ NIEPOWODZENIEM...");
             return false;
         }

    }
    terminal_QTextEdit->append("ALGORYTM ZAKOŃCZYŁ SIĘ NIEPOWODZENIEM...");
    return false;

}

void RNN::generateNumber(int n) //funkcja generujaca okreslona liczbe liczb losowych z zakresu <0;999>
{
    u = new int[n];
    for (int i = 0; i < n; i++)
    {
        u[i] = rand()%1000;
    }
}

void RNN::loadNumber(int* numberVector, int n)  // funkcja tworzaca wektor liczb w przypadku pobierania danych z pliku
// nie pobiera danych z pliku, jedynie tworzy odpowiedni wektor zmiennych
{
    u = new int[n];
    for (int i = 0; i < n; i++)
    {
        u[i] = numberVector[i];
    }
}

RNN::~RNN()
{
    delete[] u;
}
