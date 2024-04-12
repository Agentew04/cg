#include "Vector3.h"
#include "../catch_amalgamated.hpp"

using namespace Catch;

TEST_CASE("Vector3 Constructor") {
  Vector3 v;
  REQUIRE(v.x == Approx(0.0));
  REQUIRE(v.y == Approx(0.0));
  REQUIRE(v.z == Approx(0.0));
}

TEST_CASE("Vector3 Constructor with one parameter") {
  Vector3 v(1);
  REQUIRE(v.x == Approx(1.0));
  REQUIRE(v.y == Approx(1.0));
  REQUIRE(v.z == Approx(1.0));
}

TEST_CASE("Vector3 Constructor with two parameters") {
  Vector3 v(1, 2);
  REQUIRE(v.x == Approx(1.0));
  REQUIRE(v.y == Approx(2.0));
  REQUIRE(v.z == Approx(0.0));
}

TEST_CASE("Vector3 Constructor with three parameters") {
  Vector3 v(1, 2, 3);
  REQUIRE(v.x == Approx(1.0));
  REQUIRE(v.y == Approx(2.0));
  REQUIRE(v.z == Approx(3.0));
}

TEST_CASE("Vector3 Normalize") {
  Vector3 v(1, 2, 3);
  v.normalize();
  REQUIRE(v.x == Approx(0.26726124));
  REQUIRE(v.y == Approx(0.53452248));
  REQUIRE(v.z == Approx(0.80178373));
}

TEST_CASE("Vector3 Normalized") {
  Vector3 v(1, 2, 3);
  Vector3 normalized = v.normalized();
  REQUIRE(normalized.x == Approx(0.26726124));
  REQUIRE(normalized.y == Approx(0.53452248));
  REQUIRE(normalized.z == Approx(0.80178373));

  REQUIRE(v.x == Approx(1.0));
  REQUIRE(v.y == Approx(2.0));
  REQUIRE(v.z == Approx(3.0));

  REQUIRE(normalized.magnitude == Approx(1.0));

  Vector3 v2(0,0,0);
  Vector3 normalized2 = v2.normalized();
  REQUIRE(normalized2.x == Approx(0.0));
  REQUIRE(normalized2.y == Approx(0.0));
  REQUIRE(normalized2.z == Approx(0.0));
  REQUIRE(normalized2.magnitude == Approx(0.0));
}

TEST_CASE("Vector3 Distance") {
  Vector3 v(1, 2, 3);
  Vector3 v2(4, 6, 8);
  REQUIRE(v.distance(v2) == Approx(std::sqrt(std::pow(4-1,2)+std::pow(6-2,2)+std::pow(8-3,2))));

  v = Vector3(0,0,0);
  v2 = Vector3(0,5,0);
  REQUIRE(v.distance(v2) == Approx(5.0));
}

TEST_CASE("Vector3 Magnitude") {
  Vector3 v(3, 4, 0);
  REQUIRE(v.magnitude == Approx(5));

  Vector3 v2(1, 2, 3);
  REQUIRE(v2.magnitude == Approx(3.7416573867739413));
}

TEST_CASE("Vector3 Operator Plus") {
  Vector3 v1(1, 2, 3);
  Vector3 v2(4, 5, 6);
  Vector3 v3 = v1 + v2;
  REQUIRE(v3.x == Approx(5));
  REQUIRE(v3.y == Approx(7));
  REQUIRE(v3.z == Approx(9));

  v2 = Vector3(0,-1,-2);
  v3 = v1 + v2;
  REQUIRE(v3.x == Approx(1));
  REQUIRE(v3.y == Approx(1));
  REQUIRE(v3.z == Approx(1));
}

TEST_CASE("Vector3 Operator PlusEqual") {
  Vector3 v1(1, 2, 3);
  Vector3 v2(4, 5, 6);
  v1 += v2;
  REQUIRE(v1.x == 5);
  REQUIRE(v1.y == 7);
  REQUIRE(v1.z == 9);
}

TEST_CASE("Vector3 Operator Minus") {
  Vector3 v1(1, 2, 3);
  Vector3 v2(4, 5, 6);
  Vector3 v3 = v1 - v2;
  REQUIRE(v3.x == -3);
  REQUIRE(v3.y == -3);
  REQUIRE(v3.z == -3);
}

TEST_CASE("Vector3 Operator MinusEqual") {
  Vector3 v1(1, 2, 3);
  Vector3 v2(4, 5, 6);
  v1 -= v2;
  REQUIRE(v1.x == -3);
  REQUIRE(v1.y == -3);
  REQUIRE(v1.z == -3);
}

TEST_CASE("Vector3 Operator Multiply Scalar") {
  Vector3 v(1, 2, 3);
  Vector3 v2 = v * 2;
  REQUIRE(v2.x == 2);
  REQUIRE(v2.y == 4);
  REQUIRE(v2.z == 6);
}

TEST_CASE("Vector3 Operator MultiplyEqual Scalar") {
  Vector3 v(1, 2, 3);
  v *= 2;
  REQUIRE(v.x == 2);
  REQUIRE(v.y == 4);
  REQUIRE(v.z == 6);
}

TEST_CASE("Vector3 Operator Divide Scalar") {
  Vector3 v(2, 4, 6);
  Vector3 v2 = v / 2;
  REQUIRE(v2.x == 1);
  REQUIRE(v2.y == 2);
  REQUIRE(v2.z == 3);
}

TEST_CASE("Vector3 Operator DivideEqual Scalar") {
  Vector3 v(2, 4, 6);
  v /= 2;
  REQUIRE(v.x == 1);
  REQUIRE(v.y == 2);
  REQUIRE(v.z == 3);
}

TEST_CASE("Vector3 Operator Dot") {
  Vector3 v1(1, 2, 3);
  Vector3 v2(4, 5, 6);
  float dot = v1 * v2;
  REQUIRE(dot == 32);
}

TEST_CASE("Vector3 Dot") {
  Vector3 v1(1, 2, 3);
  Vector3 v2(4, 5, 6);
  float dot = v1.dot(v2);
  REQUIRE(dot == 32);
}

TEST_CASE("Vector3 Operator Cross") {
  Vector3 v1(1, 2, 3);
  Vector3 v2(4, 5, 6);
  Vector3 v3 = v1 ^ v2;
  REQUIRE(v3.x == -3);
  REQUIRE(v3.y == 6);
  REQUIRE(v3.z == -3);
}

TEST_CASE("Vector3 Cross") {
  Vector3 v1(1, 2, 3);
  Vector3 v2(4, 5, 6);
  Vector3 v3 = v1.cross(v2);
  REQUIRE(v3.x == -3);
  REQUIRE(v3.y == 6);
  REQUIRE(v3.z == -3);
}

TEST_CASE("Vector3 Operator CrossEqual") {
  Vector3 v1(1, 2, 3);
  Vector3 v2(4, 5, 6);
  v1 ^= v2;
  REQUIRE(v1.x == -3);
  REQUIRE(v1.y == 6);
  REQUIRE(v1.z == -3);
}

TEST_CASE("Vector3 Multiply") {
  Vector3 v1(1, 2, 3);
  Vector3 v2(4, 5, 6);
  Vector3 v3 = v1.multiply(v2);
  REQUIRE(v3.x == 4);
  REQUIRE(v3.y == 10);
  REQUIRE(v3.z == 18);
}

TEST_CASE("Vector3 Operator <<") {
  Vector3 v(1, 2, -3);
  std::ostringstream os;
  os << v;
  REQUIRE(os.str() == "(1, 2, -3)");
}

TEST_CASE("Vector3 Operator Equal") {
  Vector3 v1(0, -2, 4);
  Vector3 v2(0, -2, 4);
  REQUIRE(v1 == v2);
}

TEST_CASE("Vector3 Operator Not Equal") {
  Vector3 v1(0, -2, 4);
  Vector3 v2(0, -2, 5);
  REQUIRE(v1 != v2);
}

TEST_CASE("Vector3 Constants") {
  Vector3 v = Vector3::zero();
  REQUIRE(v.x == 0);
  REQUIRE(v.y == 0);
  REQUIRE(v.z == 0);

  v = Vector3::one();
  REQUIRE(v.x == 1);
  REQUIRE(v.y == 1);
  REQUIRE(v.z == 1);

  v = Vector3::left();
  REQUIRE(v.x == -1);
  REQUIRE(v.y == 0);
  REQUIRE(v.z == 0);

  v = Vector3::right();
  REQUIRE(v.x == 1);
  REQUIRE(v.y == 0);
  REQUIRE(v.z == 0);

  v = Vector3::up();
  REQUIRE(v.x == 0);
  REQUIRE(v.y == -1);
  REQUIRE(v.z == 0);

  v = Vector3::down();
  REQUIRE(v.x == 0);
  REQUIRE(v.y == 1);
  REQUIRE(v.z == 0);

  v = Vector3::forward();
  REQUIRE(v.x == 0);
  REQUIRE(v.y == 0);
  REQUIRE(v.z == -1);

  v = Vector3::back();
  REQUIRE(v.x == 0);
  REQUIRE(v.y == 0);
  REQUIRE(v.z == 1);
}
