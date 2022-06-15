#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Make function for checking Temperature every 1 sec
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(check_temperature()));
    timer->start(1000);

    // Creates new QChart dynamically
    series = new QLineSeries();
    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Temperature usage in %");

    //Creates View for QChart
    chartView = new QChartView(chart);

    // Adds QChartView to the gridLayout so it scales dynamically
    ui->gridLayout->addWidget(chartView, 1, 0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::check_temperature() {
    try {
        
        /* Get temperature */
        int temperature = readFile();

        series->append(iter, temperature);
        iter++;
        if (series->count() > 10) series->remove(0);
        chart->removeSeries(series);
        chart->addSeries(series);
        chart->createDefaultAxes();

        ui->label->setText("Temperature: " + QString::number(temperature) + "%");

    } catch (QFileDevice::FileError) {
        QMessageBox::critical(this, "Can't read file", "Please install Linux, ty! :D");
        this->close();
    }

}

/* Read the content of the temperature driver file */
int MainWindow::readFile() {
    // Define the input file stream
    std::ifstream temp_file("/opt/Temperature/bin/dht11_temp");
    int temperature;
    temp_file >> temperature;
    return temperature;
}
