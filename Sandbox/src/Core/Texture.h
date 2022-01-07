#pragma once

#include <unordered_map>

#include <GL/glew.h>

class Texture {
public:
  ~Texture();

  void SetTexParameter(int parameter, int value);

  void LoadFromFile(const char* path                                    ,
                    unsigned int target     = Texture::defaultTarget    ,
                    int flipTexture         = Texture::defaultFlip      ,
                    unsigned int dataFormat = Texture::defaultDataFormat,
                    unsigned int dataType   = Texture::defaultDataType  );

  void Bind() const;
  void UnBind() const;

  void Dispose();

private:
  static constexpr unsigned int defaultTarget     = GL_TEXTURE_2D;
  static constexpr int          defaultFlip       = GL_TRUE;
  static constexpr unsigned int defaultDataFormat = GL_RGB;
  static constexpr unsigned int defaultDataType   = GL_UNSIGNED_BYTE;

  unsigned int m_ID     = 0;
  unsigned int m_Target = 0;
  int m_Width           = 0;
  int m_Height          = 0;
  int m_BitDepth        = 0;

  std::unordered_map<int, int> m_TexParameters{};
};

