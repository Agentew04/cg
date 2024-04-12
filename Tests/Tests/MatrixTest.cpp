#include "../Math/Matrix.h"
#include "../catch_amalgamated.hpp"

using namespace Catch;
TEST_CASE("Construtor e Destrutor", "[Matrix]")
{
    SECTION("Construtor de matriz quadrada")
    {
        Matrix m(3);
        REQUIRE(m.dim() == 3);
    }

    SECTION("Construtor de cópia")
    {
        Matrix m1(3);
        m1.set(0, 0, 1.0f);
        m1.set(1, 1, 2.0f);
        m1.set(2, 2, 3.0f);

        Matrix m2(m1);

        REQUIRE(m2.dim() == 3);
        REQUIRE(m2.at(0, 0) == Approx(1.0f));
        REQUIRE(m2.at(1, 1) == Approx(2.0f));
        REQUIRE(m2.at(2, 2) == Approx(3.0f));
    }

    SECTION("Construtor com dados")
    {
        float data[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
        Matrix m(2, data);

        REQUIRE(m.dim() == 2);
        REQUIRE(m.at(0, 0) == Approx(1.0f));
        REQUIRE(m.at(0, 1) == Approx(2.0f));
        REQUIRE(m.at(1, 0) == Approx(3.0f));
        REQUIRE(m.at(1, 1) == Approx(4.0f));
    }

    SECTION("Destrutor")
    {
        // A memória alocada pelo construtor deve ser liberada pelo destrutor.
        // Não há uma maneira direta de testar isso em um teste unitário.
    }
}

TEST_CASE("Acesso a elementos", "[Matrix]")
{
    Matrix m(3);

    SECTION("Obter elemento")
    {
        m.set(1, 1, 5.0f);
        REQUIRE(m.at(1, 1) == Approx(5.0f));
    }

    SECTION("Definir elemento")
    {
        m.set(0, 0, 1.0f);
        m.set(1, 1, 2.0f);
        m.set(2, 2, 3.0f);

        REQUIRE(m.at(0, 0) == Approx(1.0f));
        REQUIRE(m.at(1, 1) == Approx(2.0f));
        REQUIRE(m.at(2, 2) == Approx(3.0f));
    }
}

TEST_CASE("Operações matemáticas", "[Matrix]")
{
    Matrix m1(2), m2(2);

    SECTION("Soma")
    {
        m1.set(0, 0, 1.0f);
        m1.set(0, 1, 2.0f);
        m1.set(1, 0, 3.0f);
        m1.set(1, 1, 4.0f);

        m2.set(0, 0, 5.0f);
        m2.set(0, 1, 6.0f);
        m2.set(1, 0, 7.0f);
        m2.set(1, 1, 8.0f);

        Matrix m3 = m1 + m2;

        REQUIRE(m3.at(0, 0) == Approx(6.0f));
        REQUIRE(m3.at(0, 1) == Approx(8.0f));
        REQUIRE(m3.at(1, 0) == Approx(10.0f));
        REQUIRE(m3.at(1, 1) == Approx(12.0f));
    }

    SECTION("Subtração")
    {
        m1.set(0, 0, 1.0f);
        m1.set(0, 1, 2.0f);
        m1.set(1, 0, 3.0f);
        m1.set(1, 1, 4.0f);

        m2.set(0, 0, 5.0f);
        m2.set(0, 1, 6.0f);
        m2.set(1, 0, 7.0f);
        m2.set(1, 1, 8.0f);

        Matrix m3 = m1 - m2;

        REQUIRE(m3.at(0, 0) == Approx(-4.0f));
        REQUIRE(m3.at(0, 1) == Approx(-4.0f));
        REQUIRE(m3.at(1, 0) == Approx(-4.0f));
        REQUIRE(m3.at(1, 1) == Approx(-4.0f));
    }

    SECTION("Multiplicação por escalar")
    {
        Matrix m(2);
        m.set(0, 0, 1.0f);
        m.set(0, 1, 2.0f);
        m.set(1, 0, 3.0f);
        m.set(1, 1, 4.0f);

        float a = 2.0f;
        Matrix m2 = m * a;

        REQUIRE(m2.at(0, 0) == Approx(2.0f));
        REQUIRE(m2.at(0, 1) == Approx(4.0f));
        REQUIRE(m2.at(1, 0) == Approx(6.0f));
        REQUIRE(m2.at(1, 1) == Approx(8.0f));
    }

    SECTION("Multiplicação de matrizes")
    {
        Matrix m1(2), m2(2);

        m1.set(0, 0, 1.0f);
        m1.set(0, 1, 2.0f);
        m1.set(1, 0, 3.0f);
        m1.set(1, 1, 4.0f);

        m2.set(0, 0, 5.0f);
        m2.set(0, 1, 6.0f);
        m2.set(1, 0, 7.0f);
        m2.set(1, 1, 8.0f);

        Matrix m3 = m1 * m2;

        REQUIRE(m3.at(0, 0) == Approx(19.0f));
        REQUIRE(m3.at(0, 1) == Approx(22.0f));
        REQUIRE(m3.at(1, 0) == Approx(43.0f));
        REQUIRE(m3.at(1, 1) == Approx(50.0f));
    }

    SECTION("Multiplicação por vetor")
    {
        float data[] = {
            1.0f, 2.0f,
            3.0f, 4.0f};
        Matrix m(2, data);
        Vector2 v(1, 2);
        Vector2 v2 = m * v;
        std::cout << v << "*" << m << "=" << v2 << std::endl;
        REQUIRE(v2.x == Approx(5.0f));
        REQUIRE(v2.y == Approx(11.0f));

        float data2[] = {
            1.0f, 2.0f, 1.0f,
            3.0f, 4.0f, 6.0f,
            8.0f, 1.0f, 0.0f};
        Matrix m2(3, data2);
        Vector3 v3(1, 2, 1);
        Vector3 v4 = m2 * v3;
        REQUIRE(v4.x == Approx(6.0f));
        REQUIRE(v4.y == Approx(17.0f));
        REQUIRE(v4.z == Approx(10.0f));

        float data3[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f};
        Matrix m3(3, data3);
        Vector3 v5(1, 2, 3);
        Vector3 v6 = m3 * v5;
        REQUIRE(v6.x == Approx(1.0f));
        REQUIRE(v6.y == Approx(2.0f));
        REQUIRE(v6.z == Approx(3.0f));

        float data4[] = {
            1.0f, 2.0f, 1.0f, 4.0f,
            3.0f, 4.0f, 6.0f, 1.0f,
            8.0f, 1.0f, 0.0f, 2.0f,
            1.0f, 6.0f, 0.0f, 1.0f};
        Matrix m4(4, data4);
        std::vector<float> v7 = {1.0f, 2.0f, 1.0f, 4.0f};
        std::vector<float> v8 = m4 * v7;
        REQUIRE(v8[0] == Approx(22.0f));
        REQUIRE(v8[1] == Approx(21.0f));
        REQUIRE(v8[2] == Approx(18.0f));
        REQUIRE(v8[3] == Approx(17.0f));
    }
}

TEST_CASE("Funções estáticas", "[Matrix]")
{
    SECTION("Matriz identidade")
    {
        Matrix m = Matrix::identity(3);

        REQUIRE(m.at(0, 0) == Approx(1.0f));
        REQUIRE(m.at(0, 1) == Approx(0.0f));
        REQUIRE(m.at(0, 2) == Approx(0.0f));

        REQUIRE(m.at(1, 0) == Approx(0.0f));
        REQUIRE(m.at(1, 1) == Approx(1.0f));
        REQUIRE(m.at(1, 2) == Approx(0.0f));

        REQUIRE(m.at(2, 0) == Approx(0.0f));
        REQUIRE(m.at(2, 1) == Approx(0.0f));
        REQUIRE(m.at(2, 2) == Approx(1.0f));
    }

    // SECTION("Matriz rotacao")
    // {
    //     Matrix m = Matrix::rotation2D(3.14159265359 / 2.0f);

    //     REQUIRE(m.at(0, 0) == Approx(0.0f));
    //     REQUIRE(m.at(0, 1) == Approx(-1.0f));
    //     REQUIRE(m.at(1, 0) == Approx(1.0f));
    //     REQUIRE(m.at(1, 1) == Approx(0.0f));

    //     Matrix m2 = Matrix::rotation2D(3.14159265359 * 2);

    //     REQUIRE(m2.at(0, 0) == Approx(1.0f));
    //     REQUIRE(m2.at(0, 1) == Approx(0.0f));
    //     REQUIRE(m2.at(1, 0) == Approx(0.0f));
    //     REQUIRE(m2.at(1, 1) == Approx(1.0f));

    //     Matrix m3 = Matrix::rotation2DHomo(3.14159265359 / 2.0f);

    //     REQUIRE(m2.at(0, 0) == Approx(0.0f));
    //     REQUIRE(m2.at(0, 1) == Approx(-1.0f));
    //     REQUIRE(m2.at(0, 2) == Approx(0.0f));
    //     REQUIRE(m2.at(1, 0) == Approx(1.0f));
    //     REQUIRE(m2.at(1, 1) == Approx(0.0f));
    //     REQUIRE(m2.at(1, 2) == Approx(0.0f));
    //     REQUIRE(m2.at(2, 0) == Approx(0.0f));
    //     REQUIRE(m2.at(2, 1) == Approx(0.0f));
    //     REQUIRE(m2.at(2, 2) == Approx(1.0f));
    // }

    SECTION("Matriz Translacao")
    {
        Matrix m = Matrix::translation2D(1.0f, 2.0f);

        REQUIRE(m.at(0, 0) == Approx(1.0f));
        REQUIRE(m.at(0, 1) == Approx(0.0f));
        REQUIRE(m.at(0, 2) == Approx(1.0f));

        REQUIRE(m.at(1, 0) == Approx(0.0f));
        REQUIRE(m.at(1, 1) == Approx(1.0f));
        REQUIRE(m.at(1, 2) == Approx(2.0f));

        REQUIRE(m.at(2, 0) == Approx(0.0f));
        REQUIRE(m.at(2, 1) == Approx(0.0f));
        REQUIRE(m.at(2, 2) == Approx(1.0f));
    }

    SECTION("Matriz Escala")
    {
        Matrix m = Matrix::scale2D(2.0f, 3.0f);

        REQUIRE(m.at(0, 0) == Approx(2.0f));
        REQUIRE(m.at(0, 1) == Approx(0.0f));
        REQUIRE(m.at(1, 0) == Approx(0.0f));
        REQUIRE(m.at(1, 1) == Approx(3.0f));
    }
}

TEST_CASE("Outros", "[Matrix]")
{
    SECTION("Dimensão")
    {
        Matrix m(3);
        REQUIRE(m.dim() == 3);
    }

    SECTION("Transposta")
    {
        float data[] = {
            1.0f, 2.0f, 3.0f,
            4.0f, 5.0f, 6.0f,
            7.0f, 8.0f, 9.0f};
        Matrix m(3, data);
        Matrix m2 = m.transpose();

        REQUIRE(m2.at(0, 0) == Approx(1.0f));
        REQUIRE(m2.at(0, 1) == Approx(4.0f));
        REQUIRE(m2.at(0, 2) == Approx(7.0f));
        REQUIRE(m2.at(1, 0) == Approx(2.0f));
        REQUIRE(m2.at(1, 1) == Approx(5.0f));
        REQUIRE(m2.at(1, 2) == Approx(8.0f));
        REQUIRE(m2.at(2, 0) == Approx(3.0f));
        REQUIRE(m2.at(2, 1) == Approx(6.0f));
        REQUIRE(m2.at(2, 2) == Approx(9.0f));
    }

    // SECTION("Inversa") {
    //     float data[] = {
    //         1.0f, 2.0f, 3.0f,
    //         0.0f, 1.0f, 4.0f,
    //         5.0f, 6.0f, 0.0f
    //     };
    //     Matrix m(3, data);

    //     Matrix m2 = m.inverse();

    //     REQUIRE(m2.at(0, 0) == Approx(-24.0f));
    //     REQUIRE(m2.at(0, 1) == Approx(18.0f));
    //     REQUIRE(m2.at(0, 2) == Approx(5.0f));
    //     REQUIRE(m2.at(1, 0) == Approx(20.0f));
    //     REQUIRE(m2.at(1, 1) == Approx(-15.0f));
    //     REQUIRE(m2.at(1, 2) == Approx(-4.0f));
    //     REQUIRE(m2.at(2, 0) == Approx(-5.0f));
    //     REQUIRE(m2.at(2, 1) == Approx(4.0f));
    //     REQUIRE(m2.at(2, 2) == Approx(-1.0f));
    // }
}
