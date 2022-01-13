#pragma once

#include <vector>
#include <initializer_list>

#include <GL/glew.h>

#include "Core/Core.h"

template <typename T>
class Buffer {
public:
  Buffer();
  Buffer(std::vector<T> buf);

  ~Buffer();

  void SetData(std::size_t size, T* data, unsigned int target = Buffer::defaultTarget, unsigned int usage = Buffer::defaultUsage);
  void SetData(std::vector<T> buf);

  void GetData(size_t size, T* data, std::ptrdiff_t offset = Buffer::defaultOffset) const;
  void GetData(std::vector<T>& buf) const;

  inline void GetSize() const {
    return m_Size;
  }

  void Bind() const;
  void UnBind() const;

  void Dispose();

private:
  static constexpr unsigned int   defaultTarget = GL_ARRAY_BUFFER;
  static constexpr unsigned int   defaultUsage  = GL_STATIC_DRAW;
  static constexpr std::ptrdiff_t defaultOffset = 0;

  unsigned int m_ID     = 0;
  unsigned int m_Target = 0;
  std::size_t  m_Size   = 0;
};

template<typename T>
Buffer<T>::Buffer()
{
  CALL(glGenBuffers(1, &m_ID));
}

template<typename T>
Buffer<T>::Buffer(std::vector<T> buf) : Buffer()
{
  SetData(buf);
}

template<typename T>
Buffer<T>::~Buffer()
{
  Dispose();
}

template<typename T>
void Buffer<T>::SetData(std::size_t size, T* data, unsigned int target, unsigned int usage)
{
  m_Target = target;
  m_Size = size;

  Bind();
  CALL(glBufferData(m_Target, m_Size * sizeof(T), data, usage));
  UnBind();
}

template<typename T>
void Buffer<T>::SetData(std::vector<T> buf)
{
  SetData(buf.size(), buf.data());
}

template<typename T>
void Buffer<T>::GetData(size_t size, T* data, std::ptrdiff_t offset) const
{
  Bind();
  glGetBufferSubData(m_Target, offset, size * sizeof(T), data);
  UnBind();
}

template<typename T>
void Buffer<T>::GetData(std::vector<T>& buf) const
{
  GetData(buf.size(), buf.data());
}

template<typename T>
void Buffer<T>::Bind() const
{
  CALL(glBindBuffer(m_Target, m_ID));
}

template<typename T>
void Buffer<T>::UnBind() const
{
  CALL(glBindBuffer(m_Target, 0));
}

template<typename T>
void Buffer<T>::Dispose()
{
  if (m_ID)
  {
    CALL(glDeleteBuffers(1, &m_ID));
    m_ID = 0;
    m_Target = 0;
    m_Size = 0;
  }
}
