#include <cstdio>
#include <string>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>

extern "C" 
{
#include <sys/ioctl.h>
#include <unistd.h>
}


struct RGB 
{
	int r, g, b;
};

/* swizzling: how RGB data is extracted, ex. BGR -> RGB */
using Swiz = RGB;
static constexpr Swiz
	SWIZ_RGB = {0, 1, 2},
	SWIZ_BGR = {2, 1, 0};

struct View
{
	int w, h;
};

static void 
getPixel(
	RGB &px, 
	cv::Mat const &im,
	int x, 
	int y,
	Swiz const &sw = SWIZ_BGR
) {
	std::size_t off = im.channels() * (y * im.cols + x);
	px.r = im.data[off+sw.r];
	px.g = im.data[off+sw.g];
	px.b = im.data[off+sw.b];
}

static void 
getPixelV(
	RGB &px, 
	cv::Mat const &im,
	View const &view, 
	int x, 
	int y, 
	Swiz const &sw = SWIZ_BGR
) {
	/* nearest neighbor for now */
	x = (int)(x * (float) im.cols / view.w);
	y = (int)(y * (float) im.rows / view.h);
	getPixel(px, im, x, y, sw);
}


int 
main(int argc, char* argv[]) 
{
	/* */
	struct winsize ws;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

	/* */
	cv::Mat im = cv::imread(argv[1]);
	View view {ws.ws_col, ws.ws_row};
	std::printf("\033[0;0H");
	for (int y = 0; y < ws.ws_row; ++y) {
		for (int x = 0; x < ws.ws_col; ++x) {
			RGB px;
			getPixelV(px, im, view, x, y);
			std::printf("\033[48;2;%d;%d;%dm ", px.r, px.g, px.b);
		}
		std::puts("");
	}
}
