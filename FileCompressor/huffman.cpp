#include "huffman.h"

#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;


// ========================
// HUFFMAN NODE
// ========================

struct HuffmanNode
{
    unsigned char data;
    long long frequency;

    // Used to make tree construction deterministic
    int minValue;

    HuffmanNode *left;
    HuffmanNode *right;

    HuffmanNode(unsigned char data, long long frequency)
    {
        this->data = data;
        this->frequency = frequency;
        this->minValue = static_cast<int>(data);

        left = nullptr;
        right = nullptr;
    }
};


// ========================
// COMPARATOR
// ========================

struct Compare
{
    bool operator()(HuffmanNode *a, HuffmanNode *b)
    {
        // Smaller frequency gets higher priority
        if (a->frequency != b->frequency)
            return a->frequency > b->frequency;

        // If frequencies are same,
        // smaller byte value gets higher priority
        return a->minValue > b->minValue;
    }
};


// ========================
// GENERATE CODES
// ========================

void generateCodes(
    HuffmanNode *root,
    string code,
    unordered_map<unsigned char, string> &codes)
{
    if (root == nullptr)
        return;

    if (root->left == nullptr &&
        root->right == nullptr)
    {
        if (code.empty())
            code = "0";

        codes[root->data] = code;
        return;
    }

    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}


// ========================
// BUILD HUFFMAN TREE
// ========================

HuffmanNode *buildTree(
    const unordered_map<unsigned char, long long> &frequency)
{
    priority_queue<
        HuffmanNode *,
        vector<HuffmanNode *>,
        Compare>
        minHeap;

    // Insert bytes in fixed order
    for (int i = 0; i < 256; i++)
    {
        unsigned char byte =
            static_cast<unsigned char>(i);

        auto it = frequency.find(byte);

        if (it != frequency.end())
        {
            minHeap.push(
                new HuffmanNode(
                    byte,
                    it->second));
        }
    }

    if (minHeap.empty())
        return nullptr;

    while (minHeap.size() > 1)
    {
        HuffmanNode *left =
            minHeap.top();

        minHeap.pop();

        HuffmanNode *right =
            minHeap.top();

        minHeap.pop();

        HuffmanNode *parent =
            new HuffmanNode(
                0,
                left->frequency +
                    right->frequency);

        parent->left = left;
        parent->right = right;

        // Smallest byte contained in this subtree
        parent->minValue =
            min(left->minValue,
                right->minValue);

        minHeap.push(parent);
    }

    return minHeap.top();
}


// ========================
// DELETE TREE
// ========================

void deleteTree(HuffmanNode *root)
{
    if (root == nullptr)
        return;

    deleteTree(root->left);
    deleteTree(root->right);

    delete root;
}


// ========================
// CONSTRUCTOR
// ========================

Huffman::Huffman()
{
}


// ========================
// COMPRESS
// ========================

bool Huffman::compressFile(
    const QString &inputFile,
    const QString &outputFile)
{
    ifstream input(
        inputFile.toStdString(),
        ios::binary);

    if (!input)
        return false;


    // Count byte frequencies
    unordered_map<unsigned char, long long>
        frequency;

    char ch;

    while (input.get(ch))
    {
        frequency[
            static_cast<unsigned char>(ch)]++;
    }

    if (frequency.empty())
        return false;


    // Build tree
    HuffmanNode *root =
        buildTree(frequency);

    if (root == nullptr)
        return false;


    // Generate Huffman codes
    unordered_map<unsigned char, string>
        codes;

    generateCodes(
        root,
        "",
        codes);


    // Return to beginning
    input.clear();
    input.seekg(0);


    ofstream output(
        outputFile.toStdString(),
        ios::binary);

    if (!output)
    {
        deleteTree(root);
        return false;
    }


    // ========================
    // WRITE HEADER
    // ========================

    int tableSize =
        static_cast<int>(
            frequency.size());

    output.write(
        reinterpret_cast<char *>(
            &tableSize),
        sizeof(tableSize));


    // Store table in fixed order
    for (int i = 0; i < 256; i++)
    {
        unsigned char byte =
            static_cast<unsigned char>(i);

        auto it =
            frequency.find(byte);

        if (it != frequency.end())
        {
            unsigned char character =
                byte;

            long long freq =
                it->second;

            output.write(
                reinterpret_cast<char *>(
                    &character),
                sizeof(character));

            output.write(
                reinterpret_cast<char *>(
                    &freq),
                sizeof(freq));
        }
    }


    // ========================
    // WRITE COMPRESSED DATA
    // ========================

    unsigned char buffer = 0;
    int bitCount = 0;


    while (input.get(ch))
    {
        unsigned char byte =
            static_cast<unsigned char>(ch);

        const string &code =
            codes[byte];


        for (char bit : code)
        {
            buffer <<= 1;

            if (bit == '1')
                buffer |= 1;

            bitCount++;


            if (bitCount == 8)
            {
                output.put(
                    static_cast<char>(
                        buffer));

                buffer = 0;
                bitCount = 0;
            }
        }
    }


    // Remaining bits
    if (bitCount > 0)
    {
        buffer <<= (8 - bitCount);

        output.put(
            static_cast<char>(
                buffer));
    }


    input.close();
    output.close();

    deleteTree(root);

    return true;
}


// ========================
// DECOMPRESS
// ========================

bool Huffman::decompressFile(
    const QString &inputFile,
    const QString &outputFile)
{
    ifstream input(
        inputFile.toStdString(),
        ios::binary);

    if (!input)
        return false;


    // ========================
    // READ HEADER
    // ========================

    int tableSize = 0;

    input.read(
        reinterpret_cast<char *>(
            &tableSize),
        sizeof(tableSize));


    if (!input ||
        tableSize <= 0 ||
        tableSize > 256)
    {
        return false;
    }


    unordered_map<unsigned char, long long>
        frequency;


    for (int i = 0;
         i < tableSize;
         i++)
    {
        unsigned char character;
        long long freq;


        input.read(
            reinterpret_cast<char *>(
                &character),
            sizeof(character));


        input.read(
            reinterpret_cast<char *>(
                &freq),
            sizeof(freq));


        if (!input || freq <= 0)
            return false;


        frequency[character] =
            freq;
    }


    // Rebuild EXACT same tree
    HuffmanNode *root =
        buildTree(frequency);

    if (root == nullptr)
        return false;


    // Calculate original file size
    long long totalCharacters = 0;

    for (const auto &item : frequency)
    {
        totalCharacters +=
            item.second;
    }


    ofstream output(
        outputFile.toStdString(),
        ios::binary);

    if (!output)
    {
        deleteTree(root);
        return false;
    }


    // ========================
    // ONE UNIQUE BYTE CASE
    // ========================

    if (root->left == nullptr &&
        root->right == nullptr)
    {
        for (long long i = 0;
             i < totalCharacters;
             i++)
        {
            output.put(
                static_cast<char>(
                    root->data));
        }

        input.close();
        output.close();

        deleteTree(root);

        return true;
    }


    // ========================
    // DECODE
    // ========================

    HuffmanNode *current =
        root;

    long long decodedCharacters =
        0;

    char byte;


    while (input.get(byte) &&
           decodedCharacters <
               totalCharacters)
    {
        unsigned char currentByte =
            static_cast<unsigned char>(
                byte);


        for (int i = 7;
             i >= 0;
             i--)
        {
            int bit =
                (currentByte >> i) & 1;


            if (bit == 0)
                current =
                    current->left;
            else
                current =
                    current->right;


            if (current == nullptr)
            {
                deleteTree(root);
                return false;
            }


            // Leaf reached
            if (current->left == nullptr &&
                current->right == nullptr)
            {
                output.put(
                    static_cast<char>(
                        current->data));


                decodedCharacters++;


                if (decodedCharacters ==
                    totalCharacters)
                {
                    break;
                }


                current = root;
            }
        }
    }


    input.close();
    output.close();

    bool success =
        decodedCharacters ==
        totalCharacters;

    deleteTree(root);

    return success;
}