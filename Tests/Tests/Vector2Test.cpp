#include "Vector2.h"
#include "../catch_amalgamated.hpp"

using namespace Catch;
TEST_CASE("Vector2: Default constructor initializes to zero", "[constructor]") {
  Vector2 v;
  REQUIRE(v.x == Approx(0.0f));
  REQUIRE(v.y == Approx(0.0f));
}

TEST_CASE("Vector2: Constructor with parameters initializes correctly", "[constructor]") {
  Vector2 v(5.0f, 3.0f);
  REQUIRE(v.x == Approx(5.0f));
  REQUIRE(v.y == Approx(3.0f));
}

TEST_CASE("Vector2: Normalize normalizes the vector correctly", "[normalize]") {
  Vector2 v(3.0f, 4.0f);
  v.normalize();
  REQUIRE(v.x == Approx(3.0f / 5.0f));
  REQUIRE(v.y == Approx(4.0f / 5.0f));
  // Also check that magnitude is close to 1 after normalization
  REQUIRE(v.magnitude == Approx(1.0f));
}

TEST_CASE("Vector2: Normalized returns a new normalized vector", "[normalized]") {
  Vector2 v(3.0f, 4.0f);
  Vector2 normalized = v.normalized();
  REQUIRE(normalized.x == Approx(3.0f / 5.0f));
  REQUIRE(normalized.y == Approx(4.0f / 5.0f));
  REQUIRE(v.x == Approx(3.0f)); // Original vector remains unchanged
  REQUIRE(v.y == Approx(4.0f));
}

TEST_CASE("Vector2: Flip flips x and y components", "[flip]") {
  Vector2 v(2.0f, 5.0f);
  v.flip();
  REQUIRE(v.x == Approx(5.0f));
  REQUIRE(v.y == Approx(2.0f));

  v = Vector2(10,5);
  Vector2 flipped = v.flipped();
  REQUIRE(flipped.x == Approx(5.0f));
  REQUIRE(flipped.y == Approx(10.0f));
}

TEST_CASE("Vector2: Distance calculates distance to another point correctly", "[distance]") {
  Vector2 v1(1.0f, 2.0f);
  Vector2 v2(4.0f, 6.0f);
  float distance = v1.distance(v2);
  REQUIRE(distance == Approx(std::sqrt(std::pow(4-1,2)+std::pow(6-2,2))));
}

TEST_CASE("Vector2: Addition of two vectors", "[operator+]") {
  Vector2 v1(1.0f, 2.0f);
  Vector2 v2(3.0f, 4.0f);
  Vector2 result = v1 + v2;

  REQUIRE(result.x == Approx(4.0f));
  REQUIRE(result.y == Approx(6.0f));
}

TEST_CASE("Vector2: In-place addition of two vectors", "[operator+]") {
  Vector2 v1(1.0f, 2.0f);
  Vector2 v2(3.0f, 4.0f);
  v1 += v2;

  REQUIRE(v1.x == Approx(4.0f));
  REQUIRE(v1.y == Approx(6.0f));
}

// -
TEST_CASE("Vector2: Subtraction of two vectors", "[operator-]") {
  Vector2 v1(1.0f, 2.0f);
  Vector2 v2(3.0f, 4.0f);
  Vector2 result = v1 - v2;

  REQUIRE(result.x == Approx(-2.0f));
  REQUIRE(result.y == Approx(-2.0f));
}

TEST_CASE("Vector2: In-place subtraction of two vectors", "[operator-]") {
  Vector2 v1(1.0f, 2.0f);
  Vector2 v2(3.0f, 4.0f);
  v1 -= v2;

  REQUIRE(v1.x == Approx(-2.0f));
  REQUIRE(v1.y == Approx(-2.0f));
}

// *
TEST_CASE("Vector2: Multiplication by scalar", "[operator*]") {
  Vector2 v(1.0f, 2.0f);
  Vector2 result = v * 3.0f;

  REQUIRE(result.x == Approx(3.0f));
  REQUIRE(result.y == Approx(6.0f));
}

TEST_CASE("Vector2: In-place multiplication by scalar", "[operator*]") {
  Vector2 v(1.0f, 2.0f);
  v *= 3.0f;

  REQUIRE(v.x == Approx(3.0f));
  REQUIRE(v.y == Approx(6.0f));
}

// /
TEST_CASE("Vector2: Division by scalar", "[operator/]") {
  Vector2 v(6.0f, 12.0f);
  Vector2 result = v / 3.0f;

  REQUIRE(result.x == Approx(2.0f));
  REQUIRE(result.y == Approx(4.0f));
}

TEST_CASE("Vector2: In-place division by scalar", "[operator/]") {
  Vector2 v(6.0f, 12.0f);
  v /= 3.0f;

  REQUIRE(v.x == Approx(2.0f));
  REQUIRE(v.y == Approx(4.0f));
}

// ==
TEST_CASE("Vector2: Equality operator", "[operator==]") {
  Vector2 v1(1.0f, 2.0f);
  Vector2 v2(1.0f, 2.0f);
  Vector2 v3(2.0f, 2.0f);

  REQUIRE(v1 == v2);
  REQUIRE_FALSE(v1 == v3);
}

// !=
TEST_CASE("Vector2: Inequality operator", "[operator!=]") {
  Vector2 v1(1.0f, 2.0f);
  Vector2 v2(1.0f, 2.0f);
  Vector2 v3(2.0f, 2.0f);

  REQUIRE_FALSE(v1 != v2);
  REQUIRE(v1 != v3);
}

// Similar test cases for subtraction, multiplication, division, dot product, etc.
// dot
TEST_CASE("Vector2: Dot product of two vectors", "[operator*]") {
  Vector2 v1(1.0f, 2.0f);
  Vector2 v2(3.0f, 4.0f);
  float dot = v1 * v2;

  REQUIRE(dot == Approx(11.0f));
}

TEST_CASE("Vector2: Dot product of two vectors using dot method", "[dot]") {
  Vector2 v1(1.0f, 2.0f);
  Vector2 v2(3.0f, 4.0f);
  float dot = v1.dot(v2);

  REQUIRE(dot == Approx(11.0f));
}

TEST_CASE("Vector2: Static methods return expected vectors", "[static methods]") {
    Vector2 zero = Vector2::zero();
    Vector2 one = Vector2::one();
    Vector2 left = Vector2::left();
    Vector2 right = Vector2::right();
    Vector2 up = Vector2::up();
    Vector2 down = Vector2::down();

    REQUIRE(zero.x == Approx(0.0f));
    REQUIRE(zero.y == Approx(0.0f));
    REQUIRE(one.x == Approx(1.0f));
    REQUIRE(one.y == Approx(1.0f));
    REQUIRE(left.x == Approx(-1.0f));
    REQUIRE(left.y == Approx(0.0f));
    REQUIRE(right.x == Approx(1.0f));
    REQUIRE(right.y == Approx(0.0f));
    REQUIRE(up.x == Approx(0.0f));
    REQUIRE(up.y == Approx(-1.0f));
    REQUIRE(down.x == Approx(0.0f));
    REQUIRE(down.y == Approx(1.0f));
}

TEST_CASE("Vector2: Stream output operator formats the vector correctly", "[operator<<]") {
  Vector2 v(2.5f, 7.1f);
  std::stringstream ss;
  ss << v;
  REQUIRE(ss.str() == "(2.5, 7.1)");
}
