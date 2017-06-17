#include "window.h"
#include "ui_window.h"

#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QValidator>

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::_MainWindow)
{
    ui->setupUi(this);

    // Set mask and default values for base input
    ui->baseLatInput->setValidator(new QDoubleValidator(-90, 90, 12, this));
    ui->baseLonInput->setValidator(new QDoubleValidator(-180, 180, 12, this));
    ui->baseAltInput->setValidator(new QDoubleValidator(8900, -100, 6, this));
    ui->baseLatInput->setText("-27.570614");
    ui->baseLonInput->setText("-48.5085033");
    ui->baseAltInput->setText("71.123");

    // Make connections and set initial state
    // IBGE and Base RTK
    connect(ui->checkBaseRtk, &QCheckBox::stateChanged, [=](int state) {
        ui->baseLatInput->setEnabled(state == Qt::Checked);
        ui->baseLonInput->setEnabled(state == Qt::Checked);
        ui->baseAltInput->setEnabled(state == Qt::Checked);
        ui->basePositionButton->setEnabled(state == Qt::Checked);
        ui->checkIBGE->setCheckState(state == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    });
    connect(ui->checkIBGE, &QCheckBox::stateChanged, [=](int state) {
        ui->IBGEObsInput->setEnabled(state == Qt::Checked);
        ui->IBGENavInput->setEnabled(state == Qt::Checked);
        ui->IBGENavButton->setEnabled(state == Qt::Checked);
        ui->IBGEObsButton->setEnabled(state == Qt::Checked);
        ui->checkBaseRtk->setCheckState(state == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    });
    ui->checkIBGE->setCheckState(Qt::Checked);
    ui->checkBaseRtk->setCheckState(Qt::Checked);

    // Connect buttons
    // Hover button
    connect(ui->hoverRawButton, &QPushButton::clicked, [=](){
        QString fileName = \
            QFileDialog::getOpenFileName(this, tr("Select a .UBX file"), \
                QDir::currentPath(), \
                QStringLiteral("*.UBX"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
            ui->hoverRawInput->setText(fileName);
        }
    });

    // Base button
    connect(ui->baseRawButton, &QPushButton::clicked, [=](){
        QString fileName = \
            QFileDialog::getOpenFileName(this, tr("Select a .UBX file"), \
                QDir::currentPath(), \
                QStringLiteral("*.UBX"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
            ui->baseRawInput->setText(fileName);
        }
    });

    // IBGE Obs
    connect(ui->IBGEObsButton, &QPushButton::clicked, [=](){
        QString fileName = \
            QFileDialog::getOpenFileName(this, tr("Select a .17o file"), \
                QDir::currentPath(), \
                QStringLiteral("*.17o"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
            ui->IBGEObsInput->setText(fileName);
        }
    });

    // IBGE Nav
    connect(ui->IBGENavButton, &QPushButton::clicked, [=](){
        QString fileName = \
            QFileDialog::getOpenFileName(this, tr("Select a .17n file"), \
                QDir::currentPath(), \
                QStringLiteral("*.17n"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
            ui->IBGENavInput->setText(fileName);
        }
    });

    // Base RTK csv values
    connect(ui->basePositionButton, &QPushButton::clicked, [=](){
        QString fileName = \
            QFileDialog::getOpenFileName(this, tr("Select a .csv file"), \
                QDir::currentPath(), \
                QStringLiteral("*.csv"));
        if (fileName.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("No file selected !"));
        } else {
            //TODO
            qDebug() << "TODO";
        }
    });

    // Process button
    connect(ui->processButton, &QPushButton::clicked, [=](){
        runRTKLIB();
        if(ui->hoverRawInput->text().isEmpty() && ui->baseRawInput->text().isEmpty()) {
            qDebug() << "Add Rover and Base files !";
        }

        if(ui->checkIBGE->checkState() == Qt::Checked) {
            if(ui->IBGEObsInput->text().isEmpty() && ui->IBGENavInput->text().isEmpty()) {
                qDebug() << "Add IBGE files !";
                return;
            }
        } else {
            //TODO
            qDebug() << "TODO";
        }
    });
}

int Window::runCmd(QString cmd)
{
    QProcess *process = new QProcess();
    process->start(cmd);
    process->waitForFinished();
    ui->output->append(process->readAllStandardError());
    return process->exitCode();
}

void Window::runRTKLIB()
{
    QString path = qApp->applicationDirPath();
    QString cmd = QStringLiteral("%1/convbin --help").arg(path);
    if(runCmd(cmd) != 0) return;
    cmd = QStringLiteral("%1/rnx2rtkp --help").arg(path);
    if(runCmd(cmd) != 0) return;
    cmd = QStringLiteral("%1/pos2kml --help").arg(path);
}

Window::~Window()
{
    delete ui;
}
