#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <opencv2/opencv.hpp>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPixmap pixmap_;
    QLabel *imageLabel_;

    void createMenuBar();
    void createAnalysisSection();

    cv::Mat convertQPixmapToMat(const QPixmap &pixmap);
    QPixmap convertMatToQPixmap(const cv::Mat &mat);
    void resizeImage(cv::Mat &img);

private slots:
    void loadImage();
    void saveImage();

    void buildBrightnessHistogram();
    void improveContrast();
};
