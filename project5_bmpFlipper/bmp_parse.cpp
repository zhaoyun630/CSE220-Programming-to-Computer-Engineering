#include "bmp_parse.h"
#include <iostream>
#include <string>

using namespace std;

// Constructor
bmpParse::bmpParse()
{
	// File header
	bmp_header.fileSize = 0;
	bmp_header.offset = 0;

	// Image header
	bmp_header.headerSize = 0;
	bmp_header.width = 0;
	bmp_header.height = 0;
        bmp_header.compression = 0;
        bmp_header.compressedImageSize = 0;
        bmp_header.horizontalResolution = 0;
        bmp_header.verticalResolution = 0;
        bmp_header.numColors = 0;
        bmp_header.importantColors = 0;
	
	// bmp_image
}

bmpParse::~bmpParse()
{
	//if(data != NULL)
	//	delete [] data;

	if(infile.is_open())
		infile.close();
	
	if(outfile.is_open())
		outfile.close();	
}

char** bmpParse::alloc_iarray(int numRows, int numCols)
{
	int x;
	char** iarray;
	iarray = new char*[numRows];
	for(x=0; x<numRows; x++)
	{
		iarray[x] = new char[numCols];
	}
	return iarray;
}

void bmpParse::free_2d_array(char** iarray, int numRows)
{
	int x;
	for(x=0; x < numRows; x++)
	{
		delete [] iarray[x];
	}
	delete [] iarray;
}



int bmpParse::readfile(char* filename)
{
	int index, row, col;
	int paddingBytes, numPixels, numBytes;
	char buffer[10];
	infile.open(filename, ios::in | ios::binary);


	if(!infile.is_open())
		return -1; // Could not open file.

	// Read Header file
	
	
	// Read signature/         
	if(infile.good())
		infile.read((char*)bmp_header.signature, 2*sizeof(char));
	else
	if(!infile)
		return -2;

	// Read filesize/         
	if(infile.good())
		infile.read((char*)&bmp_header.fileSize, sizeof(int));
	else
	if(!infile)
		return -2;


	// Read reserved/         
	if(infile.good())
		infile.read((char*)&bmp_header.reserved, sizeof(int));
	else
	if(!infile)
		return -2;


	// Read offset/         
	if(infile.good())
		infile.read((char*)&bmp_header.offset, sizeof(int));
	else
	if(!infile)
		return -2;

	// Read Header size
	if(infile.good())
		infile.read((char*)&bmp_header.headerSize, sizeof(int));
	else
	if(!infile)
		return -2;

	// Read width         
	if(infile.good())
		infile.read((char*)&bmp_header.width, sizeof(int));
	else
	if(!infile)
		return -2;

	// Read height         
	if(infile.good())
		infile.read((char*)&bmp_header.height, sizeof(int));
	else
	if(!infile)
		return -2;
	// Read plance Count         
	if(infile.good())
		infile.read((char*)bmp_header.planeCount, 2*sizeof(char));
	else
	if(!infile)
		return -2;
	// Read bit per pixel         
	if(infile.good())
		infile.read((char*)bmp_header.bitPerPixel, 2*sizeof(char));
	else
	if(!infile)
		return -2;
	// Read compression        
	if(infile.good())
		infile.read((char*)&bmp_header.compression, sizeof(int));
	else
	if(!infile)
		return -2;
	// Read compressedImageSize        
	if(infile.good())
		infile.read((char*)&bmp_header.compressedImageSize, sizeof(int));
	else
	if(!infile)
		return -2;
	// Read horizontalResolution
	if(infile.good())
		infile.read((char*)&bmp_header.horizontalResolution, sizeof(int));
	else
	if(!infile)
		return -2;
	// Read verticalResolution
	if(infile.good())
		infile.read((char*)&bmp_header.verticalResolution, sizeof(int));
	else
	if(!infile)
		return -2;
	// Read numColors
	if(infile.good())
		infile.read((char*)&bmp_header.numColors, sizeof(int));
	else
	if(!infile)
		return -2;
	// Read importantColors
	if(infile.good())
		infile.read((char*)&bmp_header.importantColors, sizeof(int));
	else
	if(!infile)
		return -2;
	
	// Verify File Header block
	memcpy(buffer, bmp_header.signature, 2*sizeof(char));
	buffer[2] = '\0';
	if(strcmp(buffer, "BM") != 0)
		return -3; // File type error

	paddingBytes = (4 - bmp_header.width*3%4)%4;
	numPixels = bmp_header.width * bmp_header.height;
	numBytes = 3*numPixels + paddingBytes*bmp_header.height;

	char *pixels;
	pixels = new char[numBytes];

	// Read image data
        if(infile.good())
		infile.read((char*)pixels, numBytes*sizeof(char));
	else
		return -2; // Error reading header.

        if(!infile)
		return -8; // data read error
	infile.close();

	// write 1D array to 2D image
	image.red = alloc_iarray(bmp_header.height, bmp_header.width);
	image.green = alloc_iarray(bmp_header.height, bmp_header.width);
	image.blue = alloc_iarray(bmp_header.height, bmp_header.width);

	index = 0;
	for (row = 0; row < bmp_header.height; row++)
	{
		for (col = 0; col < bmp_header.width; col++)
		{
			image.blue[bmp_header.height -1 - row][col] = pixels[index++];
			image.green[bmp_header.height -1 - row][col] = pixels[index++];
			image.red[bmp_header.height -1 - row][col] = pixels[index++];
		}
		index += paddingBytes;
	}

	delete [] pixels;

	return 0; // Success!
}


int bmpParse::writefile(char *filename)
{
	int row, col, paddingBytes, index;
	char padNum = 0;
	outfile.open(filename, ios::out | ios::binary);
	if(!outfile.is_open())
		return -1; // Could not open file error
	
//	outfile.write((char*)&bmp_header, sizeof(struct bmp_head));
	// Write Header file
	
	
	// Write signature/         
		outfile.write((char*)bmp_header.signature, 2*sizeof(char));
	// Write filesize/         
		outfile.write((char*)&bmp_header.fileSize, sizeof(int));
	// Write reserved/         
		outfile.write((char*)&bmp_header.reserved, sizeof(int));
	// Write offset/         
		outfile.write((char*)&bmp_header.offset, sizeof(int));
	// Write Header size
		outfile.write((char*)&bmp_header.headerSize, sizeof(int));
	// Write width         
		outfile.write((char*)&bmp_header.width, sizeof(int));
	// Write height         
		outfile.write((char*)&bmp_header.height, sizeof(int));
	// Write plance Count         
		outfile.write((char*)bmp_header.planeCount, 2*sizeof(char));
	// Write bit per pixel         
		outfile.write((char*)bmp_header.bitPerPixel, 2*sizeof(char));
	// Write compression        
		outfile.write((char*)&bmp_header.compression, sizeof(int));
	// Write compressedImageSize        
		outfile.write((char*)&bmp_header.compressedImageSize, sizeof(int));
	// Write horizontalResolution
		outfile.write((char*)&bmp_header.horizontalResolution, sizeof(int));
	// Write verticalResolution
		outfile.write((char*)&bmp_header.verticalResolution, sizeof(int));
	// Write numColors
		outfile.write((char*)&bmp_header.numColors, sizeof(int));
	// Write importantColors
		outfile.write((char*)&bmp_header.importantColors, sizeof(int));

	paddingBytes = (4 - bmp_header.width*3%4)%4;
        for (row = 0; row < bmp_header.height; row++)
	{
		for (col = 0; col < bmp_header.width; col++)
		{
			outfile.write((char*)&image.blue[bmp_header.height -1 - row][col], sizeof(char));
			outfile.write((char*)&image.green[bmp_header.height -1 - row][col], sizeof(char));
			outfile.write((char*)&image.red[bmp_header.height -1 - row][col], sizeof(char));
		}
		// padding zeros to make the width a multiple of 4
		for(index = 0; index < paddingBytes; index++)
		{
			outfile.write((char*)&padNum,sizeof(char));
		}
	}

	outfile.close();
	free_2d_array(image.red,bmp_header.height);
	free_2d_array(image.green,bmp_header.height);
	free_2d_array(image.blue,bmp_header.height);
	return 0;
}


void bmpParse::flipx()
{
	// allocate an temporary image
	struct bmp_image imageTmp;
        imageTmp.red = alloc_iarray(bmp_header.height, bmp_header.width);
	imageTmp.green = alloc_iarray(bmp_header.height, bmp_header.width);
	imageTmp.blue = alloc_iarray(bmp_header.height, bmp_header.width);
        
	int idx=0;
        int to_idx=0;

        // Copy ith row to (height - i)th row
	for(idx = 0;idx<bmp_header.height;idx++)
	{
		to_idx = bmp_header.height - 1 - idx;
		memcpy(imageTmp.blue[to_idx],image.blue[idx],bmp_header.width);
		memcpy(imageTmp.green[to_idx],image.green[idx],bmp_header.width);
       		memcpy(imageTmp.red[to_idx],image.red[idx],bmp_header.width);
	}
	
	// Write the flipped image data to original one
	for(idx = 0;idx<bmp_header.height;idx++)
	{
                memcpy(image.blue[idx],imageTmp.blue[idx],bmp_header.width);
                memcpy(image.green[idx],imageTmp.green[idx],bmp_header.width);
                memcpy(image.red[idx],imageTmp.red[idx],bmp_header.width);

	}

        // Delete temporary image data
        free_2d_array(imageTmp.red,bmp_header.height);
        free_2d_array(imageTmp.green,bmp_header.height);
        free_2d_array(imageTmp.blue,bmp_header.height);

}

void bmpParse::flipy()
{

        // allocate an temporary image
        struct bmp_image imageTmp;
        imageTmp.red = alloc_iarray(bmp_header.height, bmp_header.width);
        imageTmp.green = alloc_iarray(bmp_header.height, bmp_header.width);
        imageTmp.blue = alloc_iarray(bmp_header.height, bmp_header.width);
  
        int idx1=0;
	int idx2=0;
	int to_idx2=0;
        // Copy ith column to (width - i)th row
        for(idx1 = 0;idx1<bmp_header.height;idx1++)
        {
		for(idx2 = 0; idx2<bmp_header.width;idx2++)
		{
			to_idx2 = bmp_header.width - 1 -idx2;
                	memcpy(&imageTmp.blue[idx1][idx2],&image.blue[idx1][to_idx2],sizeof(char));
                	memcpy(&imageTmp.green[idx1][idx2],&image.green[idx1][to_idx2],sizeof(char));
                	memcpy(&imageTmp.red[idx1][idx2],&image.red[idx1][to_idx2],sizeof(char));
		}
        }

        // Write the flipped image data to original one
        for(idx1 = 0;idx1<bmp_header.height;idx1++)
        {
                for(idx2 = 0; idx2<bmp_header.width;idx2++)
                {
                        memcpy(&image.blue[idx1][idx2],&imageTmp.blue[idx1][idx2],sizeof(char));
                        memcpy(&image.green[idx1][idx2],&imageTmp.green[idx1][idx2],sizeof(char));
                        memcpy(&image.red[idx1][idx2],&imageTmp.red[idx1][idx2],sizeof(char));
                }
        }

        // Delete temporary image data

        free_2d_array(imageTmp.red,bmp_header.height);
        free_2d_array(imageTmp.green,bmp_header.height);
        free_2d_array(imageTmp.blue,bmp_header.height);

}


void bmpParse::rot90()
{
        // allocate an temporary image
        struct bmp_image imageTmp;
        imageTmp.red = alloc_iarray(bmp_header.width, bmp_header.height);
        imageTmp.green = alloc_iarray(bmp_header.width, bmp_header.height);
        imageTmp.blue = alloc_iarray(bmp_header.width, bmp_header.height);
  
        int idx1=0;
	int to_idx1=0;
        int idx2=0;
        int to_idx2=0;
	int width = 0;
	int height = 0;

	// Now rotate the image by 90 degrees and save to temporary image
       for(idx1 = 0;idx1<bmp_header.height;idx1++)
        {
		to_idx1 = bmp_header.height - 1 - idx1;
                for(idx2 = 0; idx2<bmp_header.width;idx2++)
                {
                        memcpy(&imageTmp.blue[idx2][to_idx1],&image.blue[idx1][idx2],sizeof(char));
                        memcpy(&imageTmp.green[idx2][to_idx1],&image.green[idx1][idx2],sizeof(char));
                        memcpy(&imageTmp.red[idx2][to_idx1],&image.red[idx1][idx2],sizeof(char));
                }
        }

	// Delete original image.
        free_2d_array(image.red,bmp_header.height);
	free_2d_array(image.green,bmp_header.height);
	free_2d_array(image.blue,bmp_header.height);

	// create a new image with new size.
        image.red = alloc_iarray(bmp_header.width, bmp_header.height);
	image.green = alloc_iarray(bmp_header.width, bmp_header.height);
	image.blue = alloc_iarray(bmp_header.width, bmp_header.height);



	// Write the flipped image data to original one
        for(idx1 = 0;idx1<bmp_header.width;idx1++)
        {
                for(idx2 = 0; idx2<bmp_header.height;idx2++)
                {
                        memcpy(&image.blue[idx1][idx2],&imageTmp.blue[idx1][idx2],sizeof(char));
                        memcpy(&image.green[idx1][idx2],&imageTmp.green[idx1][idx2],sizeof(char));
                        memcpy(&image.red[idx1][idx2],&imageTmp.red[idx1][idx2],sizeof(char));
                }
        }

	// update header file.
	width = bmp_header.height;
	height = bmp_header.width;
	*(&bmp_header.width) = width;
	*(&bmp_header.height) = height;
        
	
	// Delete temporary image data
        free_2d_array(imageTmp.red,bmp_header.height);
        free_2d_array(imageTmp.green,bmp_header.height);
        free_2d_array(imageTmp.blue,bmp_header.height);

}






