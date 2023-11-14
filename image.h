
#ifndef IMAGE_H
#define IMAGE_H

#include <string>

class Image
{
public:
    Image(const std::string fileUrl, int width, int height);

    int width() const;
    void setWidth(int width);

    int height() const;
    void setHeight(int height);

    std::string fileUrl() const;
    void setFileUrl(const std::string& fileUrl);

private:
    std::string mFileUrl;
    int mWidth;
    int mHeight;
};

#endif // IMAGE_H
