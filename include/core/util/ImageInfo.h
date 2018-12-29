#include <FreeImage.h>

class ImageInfo {
public:
	//ImageInfo(unsigned int width, unsigned int height, BYTE* pixels) : width(width), height(height), pixels(pixels) {}
	ImageInfo(const char* filename);
	unsigned int width;
	unsigned int height;
	BYTE* pixels;
	const char * getFileName() const;
	
	bool isOk();
	void free();
	~ImageInfo();
private:
	FIBITMAP* dib;
	const char* fileName;
};