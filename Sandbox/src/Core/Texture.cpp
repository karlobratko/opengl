#include "Texture.h"

#include <sstream>
#include <algorithm>

#include "Core/Core.h"

#include "Vendor/stb_image/stb_image.h"

const std::unordered_map<int, int> Texture::defaultTexParameters{
  { GL_TEXTURE_WRAP_S, GL_REPEAT },
  { GL_TEXTURE_WRAP_T, GL_REPEAT },
  { GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR },
  { GL_TEXTURE_MAG_FILTER, GL_LINEAR },
};

Texture::~Texture()
{
  Dispose();
}

void Texture::SetTexParameter(int parameter, int value)
{
  if (m_TexParameters.find(parameter) == std::end(m_TexParameters)) 
  {
    m_TexParameters.emplace(parameter, value);
  }
  else
  {
    m_TexParameters.at(parameter) = value;
  }
}

void Texture::LoadFromFile(const char* path, unsigned int target, int flipTexture, unsigned int dataType)
{
  m_Target = target;

  CALL(glGenTextures(1, &m_ID));
  Bind();

  std::unordered_map<int, int> tempTexParams{ Texture::defaultTexParameters };
  m_TexParameters.merge(tempTexParams);
  std::for_each(
    std::begin(m_TexParameters),
    std::end(m_TexParameters),
    [&](const std::pair<int, int>& param) -> void
    {
      CALL(glTexParameteri(m_Target, param.first, param.second));
    }
  );


  stbi_set_flip_vertically_on_load(flipTexture);
  unsigned char* data = stbi_load(path, &m_Width, &m_Height, &m_BitDepth, 0);

  if (data)
  {
    unsigned int format;
    switch (m_BitDepth)
    {
    case 1:
      format = GL_RED;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    default:
      throw std::exception{ "[ERROR] Could not load texture, error while trying to determine data format." };
    }

    CALL(glTexImage2D(m_Target, 0, format, m_Width, m_Height, 0, format, dataType, data));
    CALL(glGenerateMipmap(m_Target));
    stbi_image_free(data);
  }
  else
  {
    std::ostringstream outStream{};
    outStream << "[ERROR] Could not load texture, path: " << path;
    throw std::exception{ outStream.str().c_str() };
  }

  UnBind();
}

void Texture::Bind() const
{
  CALL(glBindTexture(m_Target, m_ID));
}

void Texture::UnBind() const
{
  CALL(glBindTexture(m_Target, 0));
}

void Texture::Dispose()
{
  if (m_ID) 
  {
    CALL(glDeleteTextures(1, &m_ID));
    m_ID       = 0;
    m_Width    = 0;
    m_Height   = 0;
    m_BitDepth = 0;
    m_TexParameters.clear();
  }
}
