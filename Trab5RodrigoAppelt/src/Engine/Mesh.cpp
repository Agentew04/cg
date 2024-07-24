#include "Mesh.h"

#include <vector>
#include <cmath>
#include <string>
#include <chrono>
#include <GL/glut.h>

#include "../2D/lodepng.h"
#include "../Math/Vector3.h"

#define M_PI 3.1415926535897932384626433832795
#define M_2_PI 6.283185307179586476925286766559
#define ONE_OVER_SQRT_2 1.414213562373095

using namespace Engine;

inline Vector2 sphereToUv(Vector3 vertex)
{
   float tetha = std::atan2(vertex.z, vertex.x);
   float phi = std::acos(vertex.y);
   float u = (tetha + M_PI) / (2 * M_PI);
   float v = phi / M_PI;
   return Vector2(u, v);
}

std::vector<Mesh::Face> recFace(Mesh &m, Vector3 a, Vector3 b, Vector3 c, int depth, int resolution)
{
   if (depth >= resolution)
   {
      // criar a face
      Mesh::Face f;
      a.normalize();
      b.normalize();
      c.normalize();
      m.vertexList.push_back(a);
      m.vertexList.push_back(b);
      m.vertexList.push_back(c);
      m.normalList.push_back(a);
      m.normalList.push_back(b);
      m.normalList.push_back(c);
      m.uvList.push_back(sphereToUv(a));
      m.uvList.push_back(sphereToUv(b));
      m.uvList.push_back(sphereToUv(c));

      f.vertices.push_back(m.vertexList.size() - 3);
      f.vertices.push_back(m.vertexList.size() - 2);
      f.vertices.push_back(m.vertexList.size() - 1);
      f.normals = f.vertices;
      f.uv = f.vertices;
      f.material = 0;
      // normais sao iguais aos vertices
      // vertice - normal - uv sao relacoes 1 pra 1.
      return {f};
   }
   else
   {
      // recursivo para 4 novas faces
      Vector3 ab = (a + b) * 0.5;
      Vector3 ac = (a + c) * 0.5;
      Vector3 bc = (b + c) * 0.5;

      auto f1 = recFace(m, a, ab, ac, depth + 1, resolution);
      auto f2 = recFace(m, ab, b, bc, depth + 1, resolution);
      auto f3 = recFace(m, ac, bc, c, depth + 1, resolution);
      auto f4 = recFace(m, ab, bc, ac, depth + 1, resolution);

      f1.insert(f1.end(), f2.begin(), f2.end());
      f1.insert(f1.end(), f3.begin(), f3.end());
      f1.insert(f1.end(), f4.begin(), f4.end());
      return f1;
   }
}

Mesh Mesh::tetrahedron(int resolution)
{
   Mesh m;

   Vector3 a(1, 0, -ONE_OVER_SQRT_2);
   Vector3 b(-1, 0, -ONE_OVER_SQRT_2);
   Vector3 c(0, 1, ONE_OVER_SQRT_2);
   Vector3 d(0, -1, ONE_OVER_SQRT_2);

   // evaluate the 4 faces
   auto face1 = recFace(m, a, b, c, 1, resolution);
   auto face2 = recFace(m, a, b, d, 1, resolution);
   auto face3 = recFace(m, a, c, d, 1, resolution);
   auto face4 = recFace(m, b, c, d, 1, resolution);

   m.faceList.insert(m.faceList.end(), face1.begin(), face1.end());
   m.faceList.insert(m.faceList.end(), face2.begin(), face2.end());
   m.faceList.insert(m.faceList.end(), face3.begin(), face3.end());
   m.faceList.insert(m.faceList.end(), face4.begin(), face4.end());

   return m;
}

Mesh Mesh::fromSweep(std::vector<Vector3> profile, unsigned int radialResolution)
{
   Mesh m;

   float step = M_2_PI / radialResolution;

   // vertices
   for (size_t i = 0; i < radialResolution; i++)
   {
      for (auto &v : profile)
      {
         m.vertexList.push_back(v.rotate(i * step, Vector3(0, 1, 0)));
      }
   }

   // calcular normais
   for (size_t i = 0; i < radialResolution; i++)
   {
      for (size_t j = 0; j < profile.size(); j++)
      {
         Vector3 a = m.vertexList[i * profile.size() + j];
         Vector3 b = m.vertexList[((i + 1) % radialResolution) * profile.size() + j];
         Vector3 d = m.vertexList[i * profile.size() + (j + 1) % profile.size()];

         Vector3 up = b - a;
         Vector3 right = d - a;
         Vector3 normal = up.cross(right);
         m.normalList.push_back(normal);
         float u = i / radialResolution;
         float v = j / profile.size();
         m.uvList.push_back(Vector2(u, v));
      }
   }

   float maxHeight = profile[profile.size() - 1].y;
   float minHeight = profile[0].y;
   m.vertexList.push_back(Vector3(0, minHeight, 0));
   m.vertexList.push_back(Vector3(0, maxHeight, 0));
   size_t bottomCapIndex = m.vertexList.size() - 2;
   size_t topCapIndex = m.vertexList.size() - 1;

   // para cada coluna de faces
   for (size_t i = 0; i < radialResolution; i++)
   {
      // para cada quad
      for (size_t j = 0; j < profile.size() - 1; j++)
      {
         int a = i * profile.size() + j;
         int b = i * profile.size() + j + 1;
         int c = ((i + 1) % radialResolution) * profile.size() + j + 1;
         int d = ((i + 1) % radialResolution) * profile.size() + j;

         // trig 1
         Mesh::Face f1;
         f1.material = 0;
         f1.vertices.push_back(a);
         f1.vertices.push_back(b);
         f1.vertices.push_back(c);
         f1.normals = f1.vertices;
         f1.uv = f1.vertices;
         // trig 2
         Mesh::Face f2;
         f2.material = 0;
         f2.vertices.push_back(a);
         f2.vertices.push_back(c);
         f2.vertices.push_back(d);
         f2.normals = f2.vertices;
         f2.uv = f2.vertices;

         m.faceList.push_back(f1);
         m.faceList.push_back(f2);
      }

      // top face
      Mesh::Face topFace;
      topFace.material = 0;
      topFace.vertices.push_back(topCapIndex);
      topFace.vertices.push_back(i * profile.size() + profile.size() - 1);
      topFace.vertices.push_back(((i + 1) % radialResolution) * profile.size() + profile.size() - 1);
      topFace.normals = topFace.vertices;
      topFace.uv = topFace.vertices;
      // bottom face
      Mesh::Face bottomFace;
      bottomFace.material = 0;
      bottomFace.vertices.push_back(bottomCapIndex);
      bottomFace.vertices.push_back(((i + 1) % radialResolution) * profile.size());
      bottomFace.vertices.push_back(i * profile.size());
      bottomFace.normals = bottomFace.vertices;
      bottomFace.uv = bottomFace.vertices;

      m.faceList.push_back(topFace);
      m.faceList.push_back(bottomFace);
   }
}

GLuint Mesh::loadTexture(std::string path)
{
   GLuint textureId = 0;
   std::vector<unsigned char> image; // The raw pixels
   unsigned width, height;
   auto start = std::chrono::high_resolution_clock::now();
   unsigned error = lodepng::decode(image, width, height, path);
   auto end = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> duration = end - start;

   if (error)
   {
      std::cout << "!!Error loading texture at \"" << path << "\". LodePNG Error: " << std::string(lodepng_error_text(error)) << std::endl;
      return 0;
   }

   glGenTextures(1, &textureId);
   glBindTexture(GL_TEXTURE_2D, textureId);
   glTexImage2D(
       GL_TEXTURE_2D,
       0,
       GL_RGB,
       width,  // bmp.getWidth(),
       height, // bmp.getHeight(),
       0,
       GL_RGBA,
       GL_UNSIGNED_BYTE,
       image.data() // bmp.getImage()
   );

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   return textureId;
}
