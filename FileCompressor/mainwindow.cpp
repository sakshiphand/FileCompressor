#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "huffman.h"

#include <QFileDialog>
#include <QPushButton>
#include <QFileInfo>
#include <QMessageBox>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initial state
    ui->progressBar->setValue(0);
    ui->statusLabel->setText("Ready");


    // =====================================================
    // SELECT FILE
    // =====================================================
    connect(ui->selectFileButton, &QPushButton::clicked, this, [=]() {

        selectedFilePath = QFileDialog::getOpenFileName(
            this,
            "Select File"
            );

        if (selectedFilePath.isEmpty())
            return;

        QFileInfo fileInfo(selectedFilePath);

        ui->filePathLabel->setText(fileInfo.fileName());

        // Reset statistics
        ui->originalSizeLabel->setText("Original Size: -");
        ui->compressedSizeLabel->setText("Compressed Size: -");
        ui->decompressedSizeLabel->setText("Decompressed Size: -");
        ui->compressionRatioLabel->setText("Space Saved: -");

        ui->progressBar->setValue(0);
        ui->statusLabel->setText("File selected");
    });


    // =====================================================
    // COMPRESS
    // =====================================================
    connect(ui->compressButton, &QPushButton::clicked, this, [=]() {

        if (selectedFilePath.isEmpty())
        {
            QMessageBox::warning(
                this,
                "Warning",
                "Please select a file first."
                );

            return;
        }


        if (selectedFilePath.endsWith(".huff", Qt::CaseInsensitive))
        {
            QMessageBox::warning(
                this,
                "Invalid File",
                "Please select an original file to compress."
                );

            return;
        }


        QString outputFilePath =
            QFileDialog::getSaveFileName(
                this,
                "Save Compressed File",
                selectedFilePath + ".huff",
                "Huffman Files (*.huff)"
                );


        if (outputFilePath.isEmpty())
            return;


        if (!outputFilePath.endsWith(".huff", Qt::CaseInsensitive))
        {
            outputFilePath += ".huff";
        }


        // Start progress
        ui->progressBar->setValue(20);
        ui->statusLabel->setText("Compressing...");

        QApplication::processEvents();


        Huffman huffman;

        bool success =
            huffman.compressFile(
                selectedFilePath,
                outputFilePath
                );


        if (!success)
        {
            ui->progressBar->setValue(0);
            ui->statusLabel->setText("Compression failed");

            QMessageBox::critical(
                this,
                "Error",
                "File compression failed."
                );

            return;
        }


        // =================================================
        // COMPRESSION STATISTICS
        // =================================================

        QFileInfo originalFile(selectedFilePath);
        QFileInfo compressedFile(outputFilePath);

        qint64 originalBytes =
            originalFile.size();

        qint64 compressedBytes =
            compressedFile.size();


        double originalKB =
            originalBytes / 1024.0;

        double compressedKB =
            compressedBytes / 1024.0;


        ui->originalSizeLabel->setText(
            QString("Original Size: %1 KB")
                .arg(originalKB, 0, 'f', 2)
            );


        ui->compressedSizeLabel->setText(
            QString("Compressed Size: %1 KB")
                .arg(compressedKB, 0, 'f', 2)
            );


        ui->decompressedSizeLabel->setText(
            "Decompressed Size: -"
            );


        // =================================================
        // CHECK WHETHER SIZE DECREASED OR INCREASED
        // =================================================

        if (originalBytes > 0)
        {
            double difference =
                (static_cast<double>(originalBytes) -
                 static_cast<double>(compressedBytes))
                / static_cast<double>(originalBytes)
                * 100.0;


            // Compression reduced file size
            if (difference > 0)
            {
                ui->compressionRatioLabel->setText(
                    QString("Space Saved: %1%")
                        .arg(difference, 0, 'f', 2)
                    );
            }

            // Compression increased file size
            else if (difference < 0)
            {
                ui->compressionRatioLabel->setText(
                    QString("Size Increased: %1%")
                        .arg(-difference, 0, 'f', 2)
                    );
            }

            // Same size
            else
            {
                ui->compressionRatioLabel->setText(
                    "No Size Reduction"
                    );
            }
        }


        ui->progressBar->setValue(100);

        ui->statusLabel->setText(
            "Compression completed"
            );


        QMessageBox::information(
            this,
            "Success",
            "File compressed successfully!"
            );
    });


    // =====================================================
    // DECOMPRESS
    // =====================================================
    connect(ui->decompressButton, &QPushButton::clicked, this, [=]() {

        if (selectedFilePath.isEmpty())
        {
            QMessageBox::warning(
                this,
                "Warning",
                "Please select a .huff file first."
                );

            return;
        }


        if (!selectedFilePath.endsWith(".huff", Qt::CaseInsensitive))
        {
            QMessageBox::warning(
                this,
                "Invalid File",
                "Please select a .huff file to decompress."
                );

            return;
        }


        // Remove .huff extension
        QString originalName = selectedFilePath;

        originalName.chop(5);


        QFileInfo info(originalName);


        // Suggested decompressed filename
        QString outputName =
            info.path() +
            "/" +
            info.completeBaseName() +
            "_decompressed";


        // Restore original extension
        if (!info.suffix().isEmpty())
        {
            outputName += "." + info.suffix();
        }


        QString outputFilePath =
            QFileDialog::getSaveFileName(
                this,
                "Save Decompressed File",
                outputName
                );


        if (outputFilePath.isEmpty())
            return;


        // Start decompression
        ui->progressBar->setValue(20);

        ui->statusLabel->setText(
            "Decompressing..."
            );

        QApplication::processEvents();


        Huffman huffman;


        bool success =
            huffman.decompressFile(
                selectedFilePath,
                outputFilePath
                );


        if (!success)
        {
            ui->progressBar->setValue(0);

            ui->statusLabel->setText(
                "Decompression failed"
                );


            QMessageBox::critical(
                this,
                "Error",
                "File decompression failed."
                );

            return;
        }


        // =================================================
        // DECOMPRESSION STATISTICS
        // =================================================

        QFileInfo compressedFile(selectedFilePath);
        QFileInfo decompressedFile(outputFilePath);


        qint64 compressedBytes =
            compressedFile.size();

        qint64 decompressedBytes =
            decompressedFile.size();


        double compressedKB =
            compressedBytes / 1024.0;

        double decompressedKB =
            decompressedBytes / 1024.0;


        ui->originalSizeLabel->setText(
            QString("Original Size: %1 KB")
                .arg(decompressedKB, 0, 'f', 2)
            );


        ui->compressedSizeLabel->setText(
            QString("Compressed Size: %1 KB")
                .arg(compressedKB, 0, 'f', 2)
            );


        ui->decompressedSizeLabel->setText(
            QString("Decompressed Size: %1 KB")
                .arg(decompressedKB, 0, 'f', 2)
            );


        // =================================================
        // SIZE DIFFERENCE
        // =================================================

        if (decompressedBytes > 0)
        {
            double difference =
                (static_cast<double>(decompressedBytes) -
                 static_cast<double>(compressedBytes))
                / static_cast<double>(decompressedBytes)
                * 100.0;


            if (difference > 0)
            {
                ui->compressionRatioLabel->setText(
                    QString("Space Saved: %1%")
                        .arg(difference, 0, 'f', 2)
                    );
            }

            else if (difference < 0)
            {
                ui->compressionRatioLabel->setText(
                    QString("Size Increased: %1%")
                        .arg(-difference, 0, 'f', 2)
                    );
            }

            else
            {
                ui->compressionRatioLabel->setText(
                    "No Size Reduction"
                    );
            }
        }


        // Decompression completed
        ui->progressBar->setValue(100);

        ui->statusLabel->setText(
            "Decompression completed"
            );


        QMessageBox::information(
            this,
            "Success",
            "File decompressed successfully!"
            );
    });
}


// =========================================================
// DESTRUCTOR
// =========================================================

MainWindow::~MainWindow()
{
    delete ui;
}