#include <QLabel>
#include <QMessageBox>
#include <QFormLayout>
#include <QTimer>
#include <QtDebug>

#include "steam/steam_api.h"

#include "mainwindow.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    SetupUI();
    if (!SteamAPI_Init())
    {
        QMessageBox fatalError;
        fatalError.critical(nullptr, "ERROR!", "Could not init Steam API");
        exit(-1);
    }
    //create a new timer object that runs in the background
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, 
        SteamAPI_RunCallbacks); //run steamapi callbacks, thats it
    timer->start(10); //run at 100 hz
}
MainWindow::~MainWindow()
{

}
void MainWindow::OnUploadButtonClicked()
{
    m_currentItem = std::make_unique<WorkshopItem>();

    m_currentItem->SetMapName(m_lnItemTitle->text());
    m_currentItem->SetMapDescription(m_lnItemDescription->text());
    m_currentItem->SetUpdateLanguage(m_languages.GetCurrentLanguage());

    m_currentItem->ReadyForUpload();
}

void MainWindow::SetupUI()
{
    setWindowTitle(tr("Momentum Mod - Workshop Upload Tool"));
    setGeometry(100, 100, 400, 400);

    auto layout = new QFormLayout;
    m_btnUpload = new QPushButton(tr("Upload"));
    connect(m_btnUpload, &QPushButton::clicked, 
        this, &MainWindow::OnUploadButtonClicked);
    m_lnItemTitle = new QLineEdit;
    m_lnItemDescription = new QLineEdit;

    layout->addRow("Map Title", m_lnItemTitle);
    layout->addRow("Map Description", m_lnItemDescription);
    layout->addRow("Language", m_languages.GetLanguageComboBox());

    layout->addWidget(m_btnUpload);

    auto frame = new QFrame;
    frame->setLayout(layout);

    setCentralWidget(frame);
}