#if !defined(MACHINE_GL_TEXTURE_H_INCLUDED)
#define MACHINE_GL_TEXTURE_H_INCLUDED



#include "./../UtilitiesGL.h"
#include "_Images.h"
#include "_Video.h"



MACHINE_DECLARE_CLASSTYPE(Machine_GL_Texture);

struct Machine_GL_Texture {
  Machine_Texture __parent;
  GLuint id;
};

/// @brief Create an OpenGL texture from an image.
/// @param image A pointer to the image.
/// @return A pointer to the OpenGL texture.
Machine_GL_Texture* Machine_GL_Texture_create(Machine_Images_Image* image);



#endif // MACHINE_GL_TEXTURE_H_INCLUDED
