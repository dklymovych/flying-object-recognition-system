#include "main_window.hpp"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Flight object recognition system");
    resize(800, 600);

    imageLabel_ = new QLabel("Image not loaded");
    imageLabel_->setAlignment(Qt::AlignCenter);

    auto loadButton = new QPushButton("Load");
    auto saveButton = new QPushButton("Save");

    auto buttonWidget = new QWidget();
    auto buttonLayout = new QHBoxLayout(buttonWidget);
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->setAlignment(Qt::AlignLeft);

    auto mainWidget = new QWidget();
    auto mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->addWidget(imageLabel_);
    mainLayout->addWidget(buttonWidget);
    mainLayout->setStretch(0, 1);

    setCentralWidget(mainWidget);

    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadImage);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveImage);
}

MainWindow::~MainWindow() {}

void MainWindow::loadImage() {
    auto filepath = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.bmp)");

    if (filepath.isEmpty()) {
        return;
    }

    pixmap_.load(filepath);
    auto scaledPixmap = pixmap_.scaled(imageLabel_->size(), Qt::KeepAspectRatio);
    imageLabel_->setPixmap(scaledPixmap);
}

void MainWindow::saveImage() {
    auto filepath = QFileDialog::getSaveFileName(this, "Save Image", "", "PNG (*.png);;JPG (*.jpg);;BMP (*.bmp)");

    if (!filepath.isEmpty()) {
        pixmap_.save(filepath);
    }
}
