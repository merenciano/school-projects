#ifndef __MATHUTILS_H__
#define __MATHUTILS_H__ 1

#define MAX(X,Y) ((X) > (Y) ? : (X) : (Y))

#include <stdlib.h>
#include <math.h>

const float kSPF = 0x00800000;

class MathUtils {
public:
  inline static float Clamp(float value, float minVal, float maxVal);
  inline static float Fisqrt(float value);
  inline static float Determinant(const float* mat, int n);
  inline static float Maxf(float a, float b);
  inline static float Minf(float a, float b);
  inline static float Absf(float value);
  inline static int Absi(int value);
  inline static float Lerpf(float a, float b, float t);

private:
  MathUtils();
  MathUtils(const MathUtils&) {}
  ~MathUtils();
};
inline MathUtils::MathUtils() {}
inline MathUtils::~MathUtils(){}

inline float MathUtils::Clamp(float value, float minVal, float maxVal) {
	if (value > maxVal) return maxVal;
	if (value > minVal) return value;
	return minVal;
}

inline float MathUtils::Fisqrt(float value) {
  float xhalf = 0.5f * value;
  int i = *(int*)&value;
  i = 0x5f3759df - (i >> 1);
  value = *(float*)&i;
  value = value*(1.5f - xhalf*value*value);
  return value;
}

inline float MathUtils::Maxf(float a, float b) {
  if (a > b) return a;
  else return b;
}

inline float MathUtils::Minf(float a, float b) {
  if (a < b) return a;
  else return b;
}

// Puedo haberme liado con i y j por querer hacerla transpuesta
// si no funciona correctamente probar cambiandolas en los
// indices del array
inline float MathUtils::Determinant(const float* mat, int n) {
  float det = 0.0f;
  float* submat;

  if (n == 2)
    return ((mat[0] * mat[3]) - (mat[1] * mat[2]));
  else {
    submat = (float*)malloc((n - 1) * (n - 1) * sizeof(float));
    for (int x = 0; x < n; x++) {
      int subi = 0;
      for (int i = 1; i < n; i++) {
        int subj = 0;
        for (int j = 0; j < n; j++) {
          if (j != x) {
            submat[(n - 1)*subi + subj] = mat[n*i + j];
            subj++;
          }
        }
        subi++;
      }
      det = det + (powf(-1.0f, (float)x) * mat[x] * Determinant(submat, n - 1));
    }
  }
  free(submat);
  return det;
}

inline float MathUtils::Absf(float value)
{
  if (value > 0.0f) return value;
  return -value;
}

inline int MathUtils::Absi(int value)
{
  if (value > 0) return value;
  return -value;
}

inline float MathUtils::Lerpf(float a, float b, float t)
{
  return (1.0f - t)*a + t * b;
}

#endif