#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rnn.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionSave_as_triggered()   // obsluga przycisku save as
{
        // Opens a dialog for saving a file
        QString fileName = QFileDialog::getSaveFileName(this, "Save as");

        // An object for reading and writing files
        QFile file(fileName);

        // Try to open a file with write only options
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
            return;
        }

        // Store the currentFile name
        currentFile = fileName;

        // Set the title for the window to the file name
        setWindowTitle(fileName);

        // Interface for writing text
        QTextStream out(&file);

        // Copy text in the textEdit widget and convert to plain text
        QString text = ui->terminal_QTextEdit->toPlainText();

        // Output to file
        out << text;

        // Close the file
        file.close();
}

void MainWindow::on_actionLoad_triggered()  // obsługa przycisku load
{
        // Opens a dialog that allows you to select a file to open
        QString fileName = QFileDialog::getOpenFileName(this, "Open the file");//
        // An object for reading and writing files
        QFile file(fileName);

        // Store the currentFile name
        currentFile = fileName;

        // Try to open the file as a read only file if possible or display a
        // warning dialog box
        if (!file.open(QIODevice::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
            return;
        }

        // Set the title for the window to the file name
        setWindowTitle(fileName);

        // Interface for reading text
        QTextStream in(&file);

        // Copy text in the string
        QString text;

        int size = 0;
        while("END" != (text = in.readLine()))
        {
            //cout << text.split(" ")[0].toInt() << endl;
            size++;
        }
        in.seek(0);
        tab = new int[size];
        for(int i = 0 ; i < size ; i++)
        {
            text = in.readLine();
            tab[i] = text.split(" ")[0].toInt();
            cout << tab[i] << endl;
        }


        ui->n_textEdit->setText(QString::number(size));
        flagLoadFromFile = true;
        ui->terminal_QTextEdit->append("Plik został wczytany.");

 }


void MainWindow::on_start_QPushButton_clicked() // obsługa przycisku start
{

    ui->actionSave_as->setEnabled(true);    // uaktywnia opcje zapisu do pliku

    // odczyt pol tekstowych z interfejsu
    QString napis01 = ui->epsilon_textEdit->toPlainText();
    QString napis02 = ui->n_textEdit->toPlainText();
    QString napis03 = ui->k_textEdit->toPlainText();
    QString napis04 = ui->maxiter_textEdit->toPlainText();
    bool fullPrint ;
    bool repeat;

    if(ui->radioButton->isChecked())    // jesli zaznaczona opcja wyswietl wszystkie epoki
    {
        fullPrint = true;
    }
    else
    {
        fullPrint = false;
    }

    if(ui->repeat_checkBox->isChecked())    // jesli zaznaczona opcja w razie niepowodzenia zmien parametry procesu
    {
        repeat = true;
    }
    else
    {
        repeat = false;
    }

    // zapis odczytanych pol tekstowych do zmiennych odpowiednich typow
    double epsilon = napis01.split(" ")[0].toDouble();
    int n = napis02.split(" ")[0].toInt();
    int k = napis03.split(" ")[0].toInt();
    int maxiter = napis04.split(" ")[0].toInt();

    //utworzenie obiektu rnn - naszej sieci rekurencyjnej
    RNN rnn;

    if(flagLoadFromFile==false) // jesli nie pobieramy danych z pliku to generujemy dane w funkcji
    {
        rnn.generateNumber(n);
    }
    else
    {
        rnn.loadNumber(tab,n);
    }
    bool working = false;
    QString data = "";


    //petla ktora zapewnia powtarzanie procesu uczenia sieci w przypadku jesli zaznaczona
    //jest opcja "w razie niepowodzenia zmien parametry procesu"
    for(int i = 0 ; i < 100 ; i++)
    {
        working = rnn.kWTA(epsilon, n, k, maxiter, ui->terminal_QTextEdit, fullPrint, repeat);
        if(working == true)
        {
            data.append("Epsilon = ");
            data.append(QString::number(epsilon));
            ui->terminal_QTextEdit->append(data);
            break;
        }
        else if(working == false && repeat == true)     // zmiana parametrow procesu
        {

            epsilon = 1.0 / (n + k);
            epsilon = epsilon + epsilon*(float(rand())/RAND_MAX - 0.5); // losowa zmiana epsilon
            maxiter += 25;
            working = rnn.kWTA(epsilon, n, k, maxiter, ui->terminal_QTextEdit, fullPrint, repeat);
        }
        else
        {
            break;
        }

    }

}


void MainWindow::on_pushButton_clicked()
{
    QString napis02 = ui->n_textEdit->toPlainText();
    QString napis03 = ui->k_textEdit->toPlainText();
    int n = napis02.split(" ")[0].toInt();
    int k = napis03.split(" ")[0].toInt();
    double epsilon = 1.0 / (n + k);
    ui->epsilon_textEdit->setText(QString::number(epsilon));
}
