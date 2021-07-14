// TODO: vector processor
//         fast inverse square root

#ifndef __VECTOR2_H__
#define __VECTOR2_H__ 1

#include <math.h>
#include "math_utils.h"

class Vector2 {
 public:

  inline Vector2();
  inline Vector2(float x, float y);
  inline Vector2(const Vector2& copy);
  inline ~Vector2();

  inline Vector2 operator+(const Vector2& b) const;
  inline Vector2 operator+(const float& b) const;
  inline Vector2& operator+=(const Vector2& b);
  inline Vector2& operator+=(const float& b);
  inline Vector2 operator-(const Vector2& b) const;
  inline Vector2 operator-(const float& b) const;
  inline Vector2& operator-();
  inline Vector2& operator-=(const Vector2& b);
  inline Vector2& operator-=(const float& b);
  inline bool operator==(const Vector2& b) const;
  inline bool operator!=(const Vector2& b) const;
  inline void operator=(const Vector2& b);
  inline void operator=(const float& b);
  inline Vector2 operator*(const float& b) const;
  inline Vector2& operator*=(const float& b);
  inline Vector2 operator/(const float& b) const;
  inline Vector2& operator/=(const float& b);

  inline float magnitude() const;
  inline void normalize();
  inline Vector2 normalized() const;
  inline void scale(const Vector2 scale);
  inline float sqrMagnitude() const;

  inline static float Distance(const Vector2& a, const Vector2& b);
  inline static float SqrDistance(const Vector2& a, const Vector2& b); // Added to compare distances without sqrt
  inline static float DotProduct(const Vector2& a, const Vector2& b);
  inline static Vector2 Lerp(const Vector2& a, const Vector2& b, float t);
  inline static Vector2 LerpUnclamped(const Vector2& a, const Vector2& b, float t);

  /*static const Vector2 kUp;
  static const Vector2 kDown;
  static const Vector2 kRight;
  static const Vector2 kLeft;
  static const Vector2 kZero;
  static const Vector2 kOne;*/

  float x_;
  float y_;
};

inline Vector2::Vector2() {}

inline Vector2::Vector2(float x, float y) {
	this->x_ = x;
	this->y_ = y;
}

inline Vector2::Vector2(const Vector2& other) {
	x_ = other.x_;
	y_ = other.y_;
}

inline Vector2::~Vector2() {}

inline Vector2 Vector2::operator+(const Vector2& b) const {
  return Vector2(x_+b.x_, y_+b.y_);
}

inline Vector2 Vector2::operator+(const float& b) const{
  return Vector2(x_+b, y_+b);
}

inline Vector2& Vector2::operator+=(const Vector2& b){
  x_ += b.x_;
  y_ += b.y_;
  return *this;
}

inline Vector2& Vector2::operator+=(const float& b) {
  x_ += b;
  y_ += b;
  return *this;
}

inline Vector2 Vector2::operator-(const Vector2& b) const {
  return Vector2(x_-b.x_, y_-b.y_);
}

inline Vector2 Vector2::operator-(const float& b) const {
  return Vector2(x_-b, y_-b);
}

inline Vector2& Vector2::operator-() {
  x_ = -x_;
  y_ = -y_;
	return *this;
}

inline Vector2& Vector2::operator-=(const Vector2& b) {
  x_ -= b.x_;
  y_ -= b.y_;
  return *this;
}

inline Vector2& Vector2::operator-=(const float& b){
  x_ -= b;
  y_ -= b;
  return *this;
}

inline bool Vector2::operator==(const Vector2& b) const {
  return x_==b.x_ && y_==b.y_;
}

inline bool Vector2::operator!=(const Vector2& b) const {
  return !(*this==b);
}

inline void Vector2::operator=(const Vector2& b) {  
  x_ = b.x_;
  y_ = b.y_;
}

inline void Vector2::operator=(const float& b) {
  x_ = b;
  y_ = b;
}

inline Vector2 Vector2::operator*(const float& b) const {
	return Vector2(x_*b, y_*b);
}

inline Vector2& Vector2::operator*=(const float& b) {
  x_ *= b;
  y_ *= b;
  return *this;
}

inline Vector2 Vector2::operator/(const float& b) const {
	return Vector2(x_/b, y_/b);
}

inline Vector2& Vector2::operator/=(const float& b) {
  x_ /= b;
  y_ /= b;
	return *this;
}

inline float Vector2::magnitude() const {
  return sqrtf(sqrMagnitude());
}

inline void Vector2::normalize() {
  float mag = MathUtils::Maxf(kSPF, magnitude());
  mag = magnitude();
  if (mag == 0.0f) mag = 0.0000001f;
  *this *= 1.0f/mag;
}

inline Vector2 Vector2::normalized() const {
  Vector2 vec;
  float mag = MathUtils::Maxf(kSPF, magnitude());
  mag = magnitude(); //arreglar esto para mag==0
  return *this * (1.0f/mag);
}

inline void Vector2::scale(const Vector2 scale){
  x_ = x_ * scale.x_;
  y_ = y_ * scale.y_;
}

inline float Vector2::sqrMagnitude() const {
  return DotProduct(*this, *this);
}

inline float Vector2::Distance(const Vector2& a, const Vector2& b) {
  return sqrtf(SqrDistance(a, b));
}

inline float Vector2::SqrDistance(const Vector2& a, const Vector2& b) {
  Vector2 vec = b-a;
  return DotProduct(vec, vec);
}
 
inline Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float t) {
  t = MathUtils::Clamp(t, 0.0f, 1.0f);
  return Vector2(LerpUnclamped(a, b, t));
}

inline Vector2 Vector2::LerpUnclamped(const Vector2& a, const Vector2& b, float t) {
	return Vector2((1 - t)*a.x_ + t * b.x_, (1 - t)*a.y_ + t * b.y_);
}

inline float Vector2::DotProduct(const Vector2& a, const Vector2& b) {
  return (a.x_*b.x_) + (a.y_*b.y_);
}

#endif 
