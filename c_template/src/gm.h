#ifndef GM_H_
#define GM_H_

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef GMDEF
#define GMDEF static inline
#endif // !GMDEF

GMDEF float minf(float x, float y);
GMDEF float maxf(float x, float y);
GMDEF float lerpf(float x, float y, float t);
GMDEF float clampf(float x, float min, float max);

typedef struct { float x, y; }       vec2f_t;
typedef struct { float x, y, z; }    vec3f_t;
typedef struct { float x, y, z, w; } vec4f_t;

#define RAD(x) ((x) * M_PI / 180.0f)
#define DEG(x) ((x) * 180.0f / M_PI)

#define MAX(x, y) (((x) < (y)) ? (y) : (x))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define X(v) (v).x;
#define Y(v) (v).y;
#define Z(v) (v).z;
#define W(v) (v).w;

#define XY(v)   ((vec2f_t) { (v).x, (v).y })
#define XYZ(v)  ((vec3f_t) { (v).x, (v).y, (v).z })
#define XYZW(v) ((vec4f_t) { (v).x, (v).y, (v).z, (v).w })

#define R(v)   (v).x;
#define RG(v)  ((vec2f_t) { (v).x, (v).y })
#define RGB(v) ((vec3f_t) { (v).x, (v).y, (v).z })

#define INT_TO_RGBA(x) ((x) >> 24 & 0xff) / 255.0f, ((x) >> 16 & 0xff) / 255.0f, ((x) >> 8 & 0xff) / 255.0f, ((x) >> 0 & 0xff) / 255.0f

#define vec2f_fmt "[%f, %f]"
#define vec2f_arg(v) (v).x, (v).y
#define vec2f_null ((vec2f_t) { 0.0f, 0.0f })
#define vec2f_1s   ((vec2f_t) { 1.0f, 1.0f })
GMDEF vec2f_t vec2f      (float x, float y);
GMDEF vec2f_t vec2ff     (float x);
GMDEF vec2f_t vec2f_sum  (vec2f_t a, vec2f_t b);
GMDEF vec2f_t vec2f_sub  (vec2f_t a, vec2f_t b);
GMDEF vec2f_t vec2f_mul  (vec2f_t a, vec2f_t b);
GMDEF vec2f_t vec2f_div  (vec2f_t a, vec2f_t b);
GMDEF vec2f_t vec2f_sqrt (vec2f_t a);
GMDEF vec2f_t vec2f_pow  (vec2f_t base, vec2f_t exp);
GMDEF vec2f_t vec2f_sin  (vec2f_t a);
GMDEF vec2f_t vec2f_cos  (vec2f_t a);
GMDEF vec2f_t vec2f_min  (vec2f_t a, vec2f_t b);
GMDEF vec2f_t vec2f_max  (vec2f_t a, vec2f_t b);
GMDEF vec2f_t vec2f_lerp (vec2f_t a, vec2f_t b, vec2f_t t);
GMDEF vec2f_t vec2f_floor(vec2f_t a);
GMDEF vec2f_t vec2f_ceil (vec2f_t a);
GMDEF vec2f_t vec2f_clamp(vec2f_t a, vec2f_t min, vec2f_t max);
GMDEF float   vec2f_len  (vec2f_t a);
GMDEF vec2f_t vec2f_norm (vec2f_t a);

#define vec3f_fmt "[%f, %f, %f]"
#define vec3f_arg(v) (v).x, (v).y, (v).z
#define vec3f_null ((vec3f_t) { 0.0f, 0.0f, 0.0f })
#define vec3f_1s   ((vec3f_t) { 1.0f, 1.0f, 1.0f })
GMDEF vec3f_t vec3f(float x, float y, float z);
GMDEF vec3f_t vec3ff(float x);
GMDEF vec3f_t vec3f_sum  (vec3f_t a, vec3f_t b);
GMDEF vec3f_t vec3f_sub  (vec3f_t a, vec3f_t b);
GMDEF vec3f_t vec3f_mul  (vec3f_t a, vec3f_t b);
GMDEF vec3f_t vec3f_div  (vec3f_t a, vec3f_t b);
GMDEF vec3f_t vec3f_sqrt (vec3f_t a);
GMDEF vec3f_t vec3f_pow  (vec3f_t base, vec3f_t exp);
GMDEF vec3f_t vec3f_sin  (vec3f_t a);
GMDEF vec3f_t vec3f_cos  (vec3f_t a);
GMDEF vec3f_t vec3f_min  (vec3f_t a, vec3f_t b);
GMDEF vec3f_t vec3f_max  (vec3f_t a, vec3f_t b);
GMDEF vec3f_t vec3f_lerp (vec3f_t a, vec3f_t b, vec3f_t t);
GMDEF vec3f_t vec3f_floor(vec3f_t a);
GMDEF vec3f_t vec3f_ceil (vec3f_t a);
GMDEF vec3f_t vec3f_clamp(vec3f_t a, vec3f_t min, vec3f_t max);
GMDEF float   vec3f_len  (vec3f_t a);
GMDEF vec3f_t vec3f_norm (vec3f_t a);

#define vec4f_fmt "[%f, %f, %f, %f]"
#define vec4f_arg(v) (v).x, (v).y, (v).z, (v).w
#define vec4f_null ((vec4f_t) { 0.0f, 0.0f, 0.0f, 0.0f })
#define vec4f_1s   ((vec4f_t) { 1.0f, 1.0f, 1.0f, 1.0f })
GMDEF vec4f_t vec4f(float x, float y, float z, float w);
GMDEF vec4f_t vec4ff(float x);
GMDEF vec4f_t vec4f_sum(vec4f_t a, vec4f_t b);
GMDEF vec4f_t vec4f_sub(vec4f_t a, vec4f_t b);
GMDEF vec4f_t vec4f_mul(vec4f_t a, vec4f_t b);
GMDEF vec4f_t vec4f_div(vec4f_t a, vec4f_t b);
GMDEF vec4f_t vec4f_sqrt(vec4f_t a);
GMDEF vec4f_t vec4f_pow(vec4f_t base, vec4f_t exp);
GMDEF vec4f_t vec4f_sin(vec4f_t a);
GMDEF vec4f_t vec4f_cos(vec4f_t a);
GMDEF vec4f_t vec4f_min(vec4f_t a, vec4f_t b);
GMDEF vec4f_t vec4f_max(vec4f_t a, vec4f_t b);
GMDEF vec4f_t vec4f_lerp(vec4f_t a, vec4f_t b, vec4f_t t);
GMDEF vec4f_t vec4f_floor(vec4f_t a);
GMDEF vec4f_t vec4f_ceil(vec4f_t a);
GMDEF vec4f_t vec4f_clamp(vec4f_t a, vec4f_t min, vec4f_t max);
GMDEF float   vec4f_len(vec4f_t a);
GMDEF vec4f_t vec4f_norm(vec4f_t a);

#endif // !GM_H_

#ifdef GM_IMPLEMENTATION
GMDEF float minf(float x, float y) 
{
  return x < y ? x : y;
}
GMDEF float maxf(float x, float y) 
{
  return x < y ? y : x;
}
inline float lerpf(float x, float y, float t)
{
  return x + (y - x) * t;
}
GMDEF float clampf(float x, float min, float max) 
{
  return minf(maxf(x, min), max);
}
GMDEF vec2f_t vec2f(float x, float y)
{
  return (vec2f_t) { x, y };
}
GMDEF vec2f_t vec2ff(float x)
{
  return (vec2f_t) { x, x };
}
GMDEF vec2f_t vec2f_sum(vec2f_t a, vec2f_t b)
{
  return (vec2f_t) { a.x + b.x, a.y + b.y };
}
GMDEF vec2f_t vec2f_sub(vec2f_t a, vec2f_t b)
{
  return (vec2f_t) { a.x - b.x, a.y - b.y };
}
GMDEF vec2f_t vec2f_mul(vec2f_t a, vec2f_t b)
{
  return (vec2f_t) { a.x * b.x, a.y * b.y };
}
GMDEF vec2f_t vec2f_div(vec2f_t a, vec2f_t b)
{
  return (vec2f_t) { a.x / b.x, a.y / b.y };
}
GMDEF vec2f_t vec2f_sqrt(vec2f_t a)
{
  return (vec2f_t) { sqrtf(a.x), sqrtf(a.y) };
}
GMDEF vec2f_t vec2f_pow(vec2f_t base, vec2f_t exp)
{
  return (vec2f_t) { powf(base.x, exp.x), powf(base.y, exp.y) };
}
GMDEF vec2f_t vec2f_sin(vec2f_t a)
{
  return (vec2f_t) { sinf(a.x), sinf(a.y) };
}
GMDEF vec2f_t vec2f_cos(vec2f_t a)
{
  return (vec2f_t) { cosf(a.x), cosf(a.y) };
}
GMDEF vec2f_t vec2f_min(vec2f_t a, vec2f_t b)
{
  return (vec2f_t) { minf(a.x, b.x), minf(a.y, b.y) };
}
GMDEF vec2f_t vec2f_max(vec2f_t a, vec2f_t b)
{
  return (vec2f_t) { maxf(a.x, b.x), maxf(a.y, b.y) };
}
GMDEF vec2f_t vec2f_lerp(vec2f_t a, vec2f_t b, vec2f_t t)
{
  return (vec2f_t) { lerpf(a.x, b.x, t.x), lerpf(a.y, b.y, t.y) };
}
GMDEF vec2f_t vec2f_floor(vec2f_t a)
{
  return (vec2f_t) { floorf(a.x), floorf(a.y) };
}
GMDEF vec2f_t vec2f_ceil(vec2f_t a)
{
  return (vec2f_t) { ceilf(a.x), ceilf(a.y) };
}
GMDEF vec2f_t vec2f_clamp(vec2f_t a, vec2f_t min, vec2f_t max)
{
  return (vec2f_t) { clampf(a.x, min.x, max.x), clampf(a.y, min.y, max.y) };
}
GMDEF float vec2f_len(vec2f_t a)
{
  return sqrtf(a.x * a.x + a.y * a.y);
}
GMDEF vec2f_t vec2f_norm(vec2f_t a)
{
  float len = vec2f_len(a);
  return (vec2f_t) { a.x / len, a.y / len };
}
GMDEF vec3f_t vec3f(float x, float y, float z)
{
  return (vec3f_t) { x, y, z };
}
GMDEF vec3f_t vec3ff(float x)
{
  return (vec3f_t) { x, x, x };
}
GMDEF vec3f_t vec3f_sum(vec3f_t a, vec3f_t b)
{
  return (vec3f_t) { a.x + b.x, a.y + b.y, a.z + b.z };
}
GMDEF vec3f_t vec3f_sub(vec3f_t a, vec3f_t b)
{
  return (vec3f_t) { a.x - b.x, a.y - b.y, a.z - b.z };
}
GMDEF vec3f_t vec3f_mul(vec3f_t a, vec3f_t b)
{
  return (vec3f_t) { a.x * b.x, a.y * b.y, a.z * b.z };
}
GMDEF vec3f_t vec3f_div(vec3f_t a, vec3f_t b)
{
  return (vec3f_t) { a.x / b.x, a.y / b.y, a.z / b.z };
}
GMDEF vec3f_t vec3f_sqrt(vec3f_t a)
{
  return (vec3f_t) { sqrtf(a.x), sqrtf(a.y), sqrtf(a.z) };
}
GMDEF vec3f_t vec3f_pow(vec3f_t base, vec3f_t exp)
{
  return (vec3f_t) { powf(base.x, exp.x), powf(base.y, exp.y), powf(base.z, exp.z) };
}
GMDEF vec3f_t vec3f_sin(vec3f_t a)
{
  return (vec3f_t) { sinf(a.x), sinf(a.y), sinf(a.z) };
}
GMDEF vec3f_t vec3f_cos(vec3f_t a)
{
  return (vec3f_t) { cosf(a.x), cosf(a.y), cosf(a.z) };
}
GMDEF vec3f_t vec3f_min(vec3f_t a, vec3f_t b)
{
  return (vec3f_t) { minf(a.x, b.x), minf(a.y, b.y), minf(a.z, b.z) };
}
GMDEF vec3f_t vec3f_max(vec3f_t a, vec3f_t b)
{
  return (vec3f_t) { maxf(a.x, b.x), maxf(a.y, b.y), maxf(a.z, b.z) };
}
GMDEF vec3f_t vec3f_lerp(vec3f_t a, vec3f_t b, vec3f_t t)
{
  return (vec3f_t) { lerpf(a.x, b.x, t.x), lerpf(a.y, b.y, t.y), lerpf(a.z, b.z, t.z) };
}
GMDEF vec3f_t vec3f_floor(vec3f_t a)
{
  return (vec3f_t) { floorf(a.x), floorf(a.y), floorf(a.z) };
}
GMDEF vec3f_t vec3f_ceil(vec3f_t a)
{
  return (vec3f_t) { ceilf(a.x), ceilf(a.y), ceilf(a.z) };
}
GMDEF vec3f_t vec3f_clamp(vec3f_t a, vec3f_t min, vec3f_t max)
{
  return (vec3f_t) { clampf(a.x, min.x, max.x), clampf(a.y, min.y, max.y), clampf(a.z, min.z, max.z) };
}
GMDEF float vec3f_len(vec3f_t a)
{
  return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}
GMDEF vec3f_t vec3f_norm(vec3f_t a)
{
  float len = vec3f_len(a);
  return (vec3f_t) { a.x / len, a.y / len, a.z / len };
}
GMDEF vec4f_t vec4f(float x, float y, float z, float w)
{
  return (vec4f_t) { x, y, z, w };
}
GMDEF vec4f_t vec4ff(float x)
{
  return (vec4f_t) { x, x, x, x };
}
GMDEF vec4f_t vec4f_sum(vec4f_t a, vec4f_t b)
{
  return (vec4f_t) { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}
GMDEF vec4f_t vec4f_sub(vec4f_t a, vec4f_t b)
{
  return (vec4f_t) { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}
GMDEF vec4f_t vec4f_mul(vec4f_t a, vec4f_t b)
{
  return (vec4f_t) { a.x* b.x, a.y* b.y, a.z* b.z, a.w * b.w };
}
GMDEF vec4f_t vec4f_div(vec4f_t a, vec4f_t b)
{
  return (vec4f_t) { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
}
GMDEF vec4f_t vec4f_sqrt(vec4f_t a)
{
  return (vec4f_t) { sqrtf(a.x), sqrtf(a.y), sqrtf(a.z), sqrtf(a.w) };
}
GMDEF vec4f_t vec4f_pow(vec4f_t base, vec4f_t exp)
{
  return (vec4f_t) { powf(base.x, exp.x), powf(base.y, exp.y), powf(base.z, exp.z), powf(base.w, exp.w) };
}
GMDEF vec4f_t vec4f_sin(vec4f_t a)
{
  return (vec4f_t) { sinf(a.x), sinf(a.y), sinf(a.z), sinf(a.w) };
}
GMDEF vec4f_t vec4f_cos(vec4f_t a)
{
  return (vec4f_t) { cosf(a.x), cosf(a.y), cosf(a.z), cosf(a.w) };
}
GMDEF vec4f_t vec4f_min(vec4f_t a, vec4f_t b)
{
  return (vec4f_t) { minf(a.x, b.x), minf(a.y, b.y), minf(a.z, b.z), minf(a.w, b.w) };
}
GMDEF vec4f_t vec4f_max(vec4f_t a, vec4f_t b)
{
  return (vec4f_t) { maxf(a.x, b.x), maxf(a.y, b.y), maxf(a.z, b.z), maxf(a.w, b.w) };
}
GMDEF vec4f_t vec4f_lerp(vec4f_t a, vec4f_t b, vec4f_t t)
{
  return (vec4f_t) { lerpf(a.x, b.x, t.x), lerpf(a.y, b.y, t.y), lerpf(a.z, b.z, t.z), lerpf(a.w, b.w, t.w) };
}
GMDEF vec4f_t vec4f_floor(vec4f_t a)
{
  return (vec4f_t) { floorf(a.x), floorf(a.y), floorf(a.z), floorf(a.w) };
}
GMDEF vec4f_t vec4f_ceil(vec4f_t a)
{
  return (vec4f_t) { ceilf(a.x), ceilf(a.y), ceilf(a.z), ceilf(a.w) };
}
GMDEF vec4f_t vec4f_clamp(vec4f_t a, vec4f_t min, vec4f_t max)
{
  return (vec4f_t) { clampf(a.x, min.x, max.x), clampf(a.y, min.y, max.y), clampf(a.z, min.z, max.z), clampf(a.w, min.w, max.w) };
}
GMDEF float vec4f_len(vec4f_t a)
{
  return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}
GMDEF vec4f_t vec4f_norm(vec4f_t a)
{
  float len = vec4f_len(a);
  return (vec4f_t) { a.x / len, a.y / len, a.z / len, a.w / len };
}
#endif