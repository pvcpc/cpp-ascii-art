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

static void
displayFrame(
	cv::Mat const &im,
	View const &view
) {
	std::printf("\033[1;1H");
	for (int y = 0; y < view.h; ++y) {
		for (int x = 0; x < view.w; ++x) {
			RGB px;
			getPixelV(px, im, view, x, y);
			std::printf("\033[48;2;%d;%d;%dm ", px.r, px.g, px.b);
		}
		std::puts("");
	}
	std::fflush(stdout);
}

int 
main(int argc, char* argv[]) 
{
	if (argc <= 1) {
		std::fputs("no input image/video file specified.\n", stderr);
		std::fputs("usage: ascii [FILE]\n", stderr);
		return 1;
	}

	/* use a large stdout buffer for the framebuffer */
	std::setvbuf(stdout, NULL, _IOFBF, 1 << 30); /* 1GB buffer */

	/* get terminal size */
	struct winsize ws;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	View view {ws.ws_col, ws.ws_row};

	/* images */
	std::string file {argv[1]};
	if (cv::haveImageReader(file)) {
		cv::Mat im = cv::imread(argv[1]);
		displayFrame(im, view);
		std::getchar();
	}
	/* videos */
	else {
		cv::VideoCapture vc;
		if (vc.open(file)) {
			cv::Mat im;
			while (vc.read(im)) {
				displayFrame(im, view);
			}
		}
		else {
			std::fprintf(stderr, "cannot open video %s.\n", file);
			return 2;
		}
	}

	return 0;
}
