#include "main_window.hpp"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMenuBar>

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
    createMenuBar();

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

cv::Mat MainWindow::convertQPixmapToMat(const QPixmap &pixmap) {
    QImage img = pixmap.toImage();
    cv::Mat mat(img.height(), img.width(), CV_8UC4, img.bits(), img.bytesPerLine());
    return mat.clone();
}

QPixmap MainWindow::convertMatToQPixmap(const cv::Mat &mat) {
    QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    return QPixmap::fromImage(img.rgbSwapped());
}

void MainWindow::resizeImage(cv::Mat &img) {
    int width = 800;
    int height = (width * img.rows) / img.cols;
    cv::resize(img, img, cv::Size(width, height));
}

void MainWindow::createMenuBar() {
    createAnalysisSection();
}

void MainWindow::createAnalysisSection() {
    auto analysisMenu = menuBar()->addMenu("Analysis");
    auto buildHistogramAction = analysisMenu->addAction("Build brightness histogram");
    auto improveContrastAction = analysisMenu->addAction("Improve contrast");

    connect(buildHistogramAction, &QAction::triggered, this, &MainWindow::buildBrightnessHistogram);
    connect(improveContrastAction, &QAction::triggered, this, &MainWindow::improveContrast);
}

void MainWindow::buildBrightnessHistogram() {
    if (pixmap_.isNull()) {
        return;
    }

    cv::Mat img = convertQPixmapToMat(pixmap_);
    cv::cvtColor(img, img, cv::COLOR_BGRA2GRAY);

    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};
    cv::Mat hist;

    cv::calcHist(&img, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);

    int histW = 512;
    int histH = 512;

    cv::Mat histImage(histH, histW, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX);

    int binW = cvRound(static_cast<double>(histW) / histSize);

    for (int i = 1; i < histSize; i++) {
        cv::line(
            histImage,
            cv::Point(binW * (i - 1),
            histH - cvRound(hist.at<float>(i - 1))),
            cv::Point(binW * i,
            histH - cvRound(hist.at<float>(i))),
            cv::Scalar(0, 0, 0),
            2,
            8,
            0
        );
    }

    cv::imshow("Brightness Histogram", histImage);
    cv::waitKey(1);
}

void MainWindow::improveContrast() {
    cv::Mat img = convertQPixmapToMat(pixmap_);
    convertScaleAbs(img, img, 1.5, 0);
    resizeImage(img);

    cv::imshow("Improved contrast", img);
    cv::waitKey(1);
}
