#include <GL/glut.h>
#include <tiffio.h>
#include "Images.h"

// Create a texture from a TIFF file using libtiff
GLuint textureFromTIFFImage(char *path, int mipmaps)
{
    // load the image
    TIFF *tiff = TIFFOpen(path, "r");
    if(!tiff)
        return 0;
    
    GLuint width, height;
    TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height);
    
    GLuint *data = (GLuint *) _TIFFmalloc(width * height * sizeof(GLuint));
    if(!data)
    {
        TIFFClose(tiff);
        return 0;
    }
    
    if(!TIFFReadRGBAImage(tiff, width, height, data, 1))
    {
        _TIFFfree(data);
        TIFFClose(tiff);
        return 0;
    }
    
    // create a texture
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    if(mipmaps)
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height,
            GL_RGBA, GL_UNSIGNED_BYTE, data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, data);
    _TIFFfree(data);
    TIFFClose(tiff);
    return texID;
}
