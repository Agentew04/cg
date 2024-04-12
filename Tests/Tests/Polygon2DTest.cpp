#include "../Math/Polygon2D.h"
#include "../catch_amalgamated.hpp"

using namespace Catch;

TEST_CASE("Polygon2D: Default constructor initializes to empty", "[constructor]")
{
    Polygon2D p;
    REQUIRE(p.vertices.size() == 0);
}

TEST_CASE("Polygon2D: Constructor with parameters initializes correctly", "[constructor]")
{
    std::vector<Vector2> vertices;
    vertices.push_back(Vector2(1.0f, 2.0f));
    vertices.push_back(Vector2(3.0f, 4.0f));
    Polygon2D p(vertices);
    REQUIRE(p.vertices.size() == 2);
    REQUIRE(p.vertices[0].x == Approx(1.0f));
    REQUIRE(p.vertices[0].y == Approx(2.0f));
    REQUIRE(p.vertices[1].x == Approx(3.0f));
    REQUIRE(p.vertices[1].y == Approx(4.0f));
}

TEST_CASE("Polygon2D: Copy constructor copies correctly", "[constructor]")
{
    std::vector<Vector2> vertices;
    vertices.push_back(Vector2(1.0f, 2.0f));
    vertices.push_back(Vector2(3.0f, 4.0f));
    Polygon2D p(vertices);
    Polygon2D p2(p);
    REQUIRE(p2.vertices.size() == 2);
    REQUIRE(p2.vertices[0].x == Approx(1.0f));
    REQUIRE(p2.vertices[0].y == Approx(2.0f));
    REQUIRE(p2.vertices[1].x == Approx(3.0f));
    REQUIRE(p2.vertices[1].y == Approx(4.0f));
}

TEST_CASE("Polygon2D: Point inside polygon", "[pointInside]")
{
    std::vector<Vector2> vertices;
    vertices.push_back(Vector2(3,1));
    vertices.push_back(Vector2(4,3));
    vertices.push_back(Vector2(2,2));
    Polygon2D triangulo(vertices);
    SECTION("Triangulo-Dentro"){
        REQUIRE(triangulo.pointInside(Vector2(3.5,2.2)));
        REQUIRE(triangulo.pointInside(Vector2(3, 2.5)));
        REQUIRE(triangulo.pointInside(Vector2(2.2,2)));
    }
    SECTION("Triangulo-Borda"){
        //REQUIRE(!triangulo.pointInside(Vector2(3,1)));
        //REQUIRE(!triangulo.pointInside(Vector2(4,3)));
        //REQUIRE(!triangulo.pointInside(Vector2(2,2)));
        //REQUIRE(!triangulo.pointInside(Vector2(2.5,1.5)));
        //REQUIRE(!triangulo.pointInside(Vector2(3,2.5)));
    }
    SECTION("Triangulo-Fora"){
        REQUIRE(!triangulo.pointInside(Vector2(1,1)));
        REQUIRE(!triangulo.pointInside(Vector2(5,5)));
        REQUIRE(!triangulo.pointInside(Vector2(3,4)));
        REQUIRE(!triangulo.pointInside(Vector2(4,1)));
    }
}