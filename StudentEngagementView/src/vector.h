#pragma once
#ifndef STUDENT_ENGAGEMENT_SRC_VECTOR_H
#define STUDENT_ENGAGEMENT_SRC_VECTOR_H

namespace ste {

struct Vector2 {
  float x, y;


  Vector2(float X, float Y) : x(X), y(Y) {}
  Vector2(float T) : x(T), y(T) {}
};
struct IVector2 {
  int x, y;


  IVector2(int X, int Y) : x(X), y(Y) {}
  IVector2(int T) : x(T), y(T) {}
};


struct Vector3 {
  float x, y, z;

  Vector3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
  Vector3(float T) : x(T), y(T), z(T) {}
};

struct Vector4 {
  float x, y, z, w;

  Vector4(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W) {}
  Vector4(float T) : x(T), y(T), z(T), w(T) {}
};

typedef Vector2 Point2;
typedef Vector3 Color;



}// namespace ste

#endif