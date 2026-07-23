#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <QString>

class Huffman
{
public:
    Huffman();

    bool compressFile(
        const QString &inputFile,
        const QString &outputFile
        );

    bool decompressFile(
        const QString &inputFile,
        const QString &outputFile
        );
};

#endif // HUFFMAN_H