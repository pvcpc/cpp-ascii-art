#include <cstdio>
#include <string>

extern "C" {
#include <sys/ioctl.h>
#include <unistd.h>
}

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"


struct RGBA {
	int r, g, b, a;
};

struct View {
	int w, h;
};

struct Image {
	static constexpr int channels {4};

	unsigned char *buf;
	int w, h, n;

	Image(std::string const &fp) {
		buf = stbi_load(fp.c_str(), &w, &h, &n, channels);
	}

	~Image() {
		stbi_image_free(buf);
	}

	void getPixel(RGBA &px, int x, int y) {
		std::size_t off = 4*(y*w+x);
		px.r = buf[off];
		px.g = buf[off+1];
		px.b = buf[off+2];
		px.a = buf[off+3];
	}

	void getPixelV(RGBA &px, View const &view, int x, int y) {
		/* nearest neighbor for now */
		x = (int)(x * (float) w / view.w);
		y = (int)(y * (float) h / view.h);
		getPixel(px, x, y);
	}
};


int 
main(int argc, char* argv[]) 
{
	/* get terminal dimensions (posix) */
	struct winsize ws;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

	/* load and write image */
	Image im {argv[1]};
	View view { ws.ws_col, ws.ws_row };

	std::printf("\033[1;1H");
	for (int y = 0; y < ws.ws_row; ++y) {
		for (int x = 0; x < ws.ws_col; ++x) {
			RGBA px;
			im.getPixelV(px, view, x, y);
			std::printf("\033[48;2;%d;%d;%dm ", px.r, px.g, px.b);
		}
		std::puts("");
	}
}
