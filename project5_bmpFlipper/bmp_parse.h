#ifndef BMP_PARSE_H
#define BMP_PARSE_H
#include <fstream>

struct bmp_image
{
	char **red;
	char **green;
	char **blue;
} __attribute__ ((__packed__));


struct bmp_head
{
	// File header
	char signature[2];
	int fileSize;
	int reserved;
	int offset;

	// Image header
	int headerSize;
	int width;
	int height;
	char planeCount[2];
	char bitPerPixel[2];
	int compression;
	int compressedImageSize;
	int horizontalResolution;
	int verticalResolution;
	int numColors;
	int importantColors;
};

class bmpParse
{
public:
	bmpParse();
	~bmpParse();
	char** alloc_iarray(int numRows, int numCols);
	void free_2d_array(char** iarray, int numRows);
	int readfile(char* filename);
	int writefile(char* filename);
	void flipx();
	void flipy();
	void rot90();

private:
	std::ifstream infile;
	std::ofstream outfile;
	struct bmp_head bmp_header;
	struct bmp_image image;
};

#endif
