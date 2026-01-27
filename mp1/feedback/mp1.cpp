#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include <string>
#include <iostream>

using namespace cs225;

void rotate(std::string inputFile, std::string outputFile) {
    PNG in;
    if (!in.readFromFile(inputFile)) {
        std::cerr << "Cannot open input file: " << inputFile << std::endl;
        return;
    }
    unsigned int width  = in.width();
    unsigned int height = in.height();
    PNG out(height, width);
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            HSLAPixel* pixelIn  = in.getPixel(x, y);
            HSLAPixel* pixelOut = out.getPixel(y, width - 1 - x);
            *pixelOut = *pixelIn;
        }
    }
    out.writeToFile(outputFile);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: mp1 <input.png> <output.png>\n";
        return 1;
    }
    rotate(argv[1], argv[2]);
    return 0;
}

