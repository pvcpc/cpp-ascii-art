#include <iostream>
#include <string>
#include <tuple>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
#include "colors.h"

class LoadImage {
public:
    int x, y, n;
    unsigned char * data;
    LoadImage (char* filename) {
        int ox, oy, n;
        unsigned char * odata = stbi_load(filename, &ox, &oy, &n, 0);
        x = 500; y = (int)(x * oy / (2 * (float)ox));

        data = new unsigned char[x * y * n];
        stbir_resize_uint8(odata, ox, oy, 0, data, x, y, 0, n);
        stbi_image_free(odata);
    }

    std::tuple<unsigned char, unsigned char, unsigned char> getPixel(int x, int y) {
        unsigned char r = data[(y * this->x + x) * 3];
        unsigned char g = data[(y * this->x + x) * 3 + 1];
        unsigned char b = data[(y * this->x + x) * 3 + 2];
        return {r, g, b};
    }
};

int distanceCalculator(std::tuple<unsigned char, unsigned char, unsigned char> a, std::tuple<unsigned char, unsigned char, unsigned char> b) {
    int x = std::get<0>(a) - std::get<0>(b);
    int y = std::get<1>(a) - std::get<1>(b);
    int z = std::get<2>(a) - std::get<2>(b);

    return x*x + y*y + z*z;
}

int choose_color(std::tuple<unsigned char, unsigned char, unsigned char> rgb) {
    int minValue = INT_MAX;
    int minIndex = 0;
    for (int i = 0; i < 256; i++) {
        int currDistance = distanceCalculator(rgb, colors[i]);
        if (currDistance < minValue) {
            minValue = currDistance;
            minIndex = i;
        }
    }

    return minIndex;
}

int main (int argc, char** argv) {
    std::string codelib = "B8&WM#YXQO{}[]()I1i!pao;:,.    ";
    int count = codelib.size();
    LoadImage currImage = LoadImage(argv[1]);

    std::cout << "\033[0;0H";
    for (int y = 0; y < currImage.y; y++) {
        for (int x = 0;  x < currImage.x; x++) {
            auto rgb = currImage.getPixel(x, y);
            int r = std::get<0>(rgb);
            int g = std::get<1>(rgb);
            int b = std::get<2>(rgb);

            float gray = 0.299 * r + 0.587 * g + 0.114 * b;
            int currIndex = choose_color(rgb);

            std::cout << "\u001b[38;5;" + std::to_string(currIndex) + 'm' + codelib[(int)((count - 1) * gray / 255.0f)];
            // std::cout << codelib[(int)((count - 1) * gray / 255.0f)];
        }
        std::cout << std::endl;
    }
}
