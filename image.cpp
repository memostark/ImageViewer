
#include "image.h"


Image::Image(const std::string fileUrl, int width, int height) :
    mFileUrl(fileUrl),
    mWidth(width),
    mHeight(height)
{

}

int Image::width() const
{
    return mWidth;
}

void Image::setWidth(int width)
{
    mWidth = width;
}

int Image::height() const
{
    return mHeight;
}

void Image::setHeight(int height)
{
    mHeight = height;
}

std::string Image::fileUrl() const
{
    return mFileUrl;
}

void Image::setFileUrl(const std::string &fileUrl)
{
    mFileUrl = fileUrl;
}

