#include <iostream>
#include <cstdio>
#include <string>

extern "C" {
#include <termios.h>
#include <sys/ioctl.h>
}

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"


struct RGBA {
	int r, g, b, a;
};

struct Image {
	unsigned char *buf;
	int w, h, n;

	Image(std::string const &fp) {
		buf = stbi_load(fp.c_str(), &w, &h, &n, 0);
	}

	~Image() {
		stbi_image_free(buf);
	}

	void getPixel(RGBA &px, int x, int y) {
		switch (n) {
		case 1: {
			int e = buf[y+w+x];
			px.r = e;
			px.g = e;
			px.b = e;
			px.a = 255;
			break;
		}
		case 2: {
			px.r = buf[y*w+x];
			px.g = buf[y*w+x+1];
			px.b = 0;
			px.a = 255;
			break;
		}
		case 3: {
			px.r = buf[y*w+x];
			px.g = buf[y*w+x+1];
			px.b = buf[y*w+x+2];
			px.a = 255;
			break;
		}
		case 4: {
			px.r = buf[y*w+x];
			px.g = buf[y*w+x+1];
			px.b = buf[y*w+x+2];
			px.a = buf[y*w+x+3];
			break;
		}
		}
	}
};


int 
main(int argc, char* argv[]) 
{
	/* configure stdin so we can query terminal info */
	struct termios settings;
	tcgetattr(0, &settings);
	settings.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(0, TCSAFLUSH, &settings);

	/* */
	std::printf("\033[18;12H");
	std::printf("\033[6n");
	char a, b, c;
	std::cin >> c >> b >> c;
	std::cout << a << b << c << std::endl;
//	std::getline(std::cin, line);
//	std::cout << line << std::endl;
//	Image image {argv[1]};
//	
//	std::puts("\033[1;1H");
//	for (int y = 0; y < image.h; y++) {
//		for (int x = 0;  x < image.w; x++) {
//			RGBA px;
//			image.getPixel(px, x, y);
//			std::printf("\033[48;2;%d;%d;%dm ", px.r, px.g, px.b);
//		}
//		std::puts("");
//	}
}
