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

TEST_CASE("Rectangle2D: Point inside rectangle", "[pointInside]")
{
    Rectangle2D r(Vector2(1,1), Vector2(2,2), true);
    SECTION("Rectangle-Dentro"){
        REQUIRE(r.pointInside(Vector2(1.5,1.5)));
        REQUIRE(r.pointInside(Vector2(2,2)));
        REQUIRE(r.pointInside(Vector2(1,1)));
    }
    SECTION("Rectangle-Borda"){
        REQUIRE(r.pointInside(Vector2(1,2)));
        REQUIRE(r.pointInside(Vector2(2,1)));
    }
    SECTION("Rectangle-Fora"){
        REQUIRE(!r.pointInside(Vector2(0,0)));
        REQUIRE(!r.pointInside(Vector2(3,3)));
        REQUIRE(!r.pointInside(Vector2(1,3)));
        REQUIRE(!r.pointInside(Vector2(3,1)));
    }
}

TEST_CASE("Circle2D: Point inside circle", "[pointInside]")
{
    Circle2D c(Vector2(1,1), 1);
    SECTION("Circle-Dentro"){
        REQUIRE(c.pointInside(Vector2(1,1)));
        REQUIRE(c.pointInside(Vector2(1.5,1.5)));
        REQUIRE(c.pointInside(Vector2(1.5,1)));
    }
    SECTION("Circle-Borda"){
        REQUIRE(c.pointInside(Vector2(1,2)));
        REQUIRE(c.pointInside(Vector2(2,1)));
    }
    SECTION("Circle-Fora"){
        REQUIRE(!c.pointInside(Vector2(0,0)));
        REQUIRE(!c.pointInside(Vector2(2,2)));
        REQUIRE(!c.pointInside(Vector2(1,3)));
        REQUIRE(!c.pointInside(Vector2(3,1)));
    }
}

TEST_CASE("Polygon2D: Intersects with rectangle", "[intersects]")
{
    std::vector<Vector2> vertices;
    vertices.push_back(Vector2(1,1));
    vertices.push_back(Vector2(2,2));
    vertices.push_back(Vector2(2,4));
    vertices.push_back(Vector2(1,4));
    Polygon2D poly(vertices);

    SECTION("Triangulo-Rectangle-Dentro"){
        Rectangle2D r(Vector2(2.5,0), Vector2(1.5,3), true);
        REQUIRE(poly.intersects(r).happened);
    }
    SECTION("Triangulo-Rectangle-Fora"){
        Rectangle2D r2(Vector2(0.5,0), Vector2(-1,2), true);
        REQUIRE(!poly.intersects(r2).happened);
    }
}

TEST_CASE("Polygon2D: Intersects with circle", "[intersects]")
{
    std::vector<Vector2> vertices;
    vertices.push_back(Vector2(0.5, 1.5));
    vertices.push_back(Vector2(1, 2.5));
    vertices.push_back(Vector2(-1, 3));
    Polygon2D triangulo(vertices);
    SECTION("Poly-Circle-Dentro"){
        Circle2D c(Vector2(1,1), 1);
        REQUIRE(triangulo.intersects(c).happened);
    }
    SECTION("Poly-Circle-Fora"){
        Circle2D c(Vector2(3,1), 1);
        REQUIRE(!triangulo.intersects(c).happened);
    }
}

TEST_CASE("Rectangle2D: Intersects with rectangle", "[intersects]")
{
    Rectangle2D r(Vector2(-2,-1), Vector2(-4,-3), true);
    SECTION("Rect-Rect-Dentro"){
        Rectangle2D r2(Vector2(-3,-2), Vector2(-5,-4), true);
        REQUIRE(r.intersects(r2).happened);
    }
    SECTION("Rectangle-Rectangle-Fora"){
        Rectangle2D r3(Vector2(1,-1), Vector2(2,-2), true);
        REQUIRE(!r.intersects(r3).happened);
    }
}

TEST_CASE("Rectangle2D: Intersects with circle", "[intersects]")
{
    Rectangle2D r(Vector2(1,1), Vector2(3,2), true);
    SECTION("Rectangle-Circle-Dentro"){
        Circle2D c(Vector2(0,3), 2);
        REQUIRE(r.intersects(c).happened);
    }
    SECTION("Rectangle-Circle-Fora"){
        Circle2D c2(Vector2(2, -0.5), 1);
        REQUIRE(!r.intersects(c2).happened);
    }
}

TEST_CASE("Circle2D: Intersects with circle", "[intersects]")
{
    Circle2D c(Vector2(0,3), 2);
    SECTION("Circle-Circle-Dentro"){
        Circle2D c2(Vector2(-1, 2), 1);
        REQUIRE(c.intersects(c2).happened);
    }
    SECTION("Circle-Circle-Fora"){
        Circle2D c3(Vector2(2, -0.5), 1);
        REQUIRE(!c.intersects(c3).happened);
    }
}

TEST_CASE("Circle2D: Intersects with rectangle", "[intersects]")
{
    Circle2D c(Vector2(4, -2), 1);
    SECTION("Circle-Rect-Dentro"){
        Rectangle2D r(Vector2(2,-1), Vector2(4,-2), true);
        REQUIRE(c.intersects(r).happened);
    }
    SECTION("Circle-Rect-Fora"){
        Rectangle2D r2(Vector2(0,0), Vector2(1,1), true);
        REQUIRE(!c.intersects(r2).happened);
    }
}

