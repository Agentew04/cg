catch: catch_amalgamated.cpp
	g++ -c catch_amalgamated.cpp -o catch.o

vector3.o: Math/Vector3.cpp Math/Vector3.h
	g++ -c Math/Vector3.cpp -o vector3.o

vector3test.o: Tests/Vector3Test.cpp Math/Vector3.h
	g++ -c Tests/Vector3Test.cpp -o vector3test.o

vector2.o: Math/Vector2.cpp Math/Vector2.h
	g++ -c Math/Vector2.cpp -o vector2.o

vector2test.o: Tests/Vector2Test.cpp Math/Vector2.h
	g++ -c Tests/Vector2Test.cpp -o vector2test.o

polygon2d.o: Math/Polygon2D.cpp Math/Polygon2D.h
	g++ -c Math/Polygon2D.cpp -o polygon2d.o

polygon2dtest.o: Tests/Polygon2DTest.cpp Math/Polygon2D.h
	g++ -c Tests/Polygon2DTest.cpp -o polygon2dtest.o

matrix.o: Math/Matrix.cpp Math/Matrix.h
	g++ -c Math/Matrix.cpp -o matrix.o

matrixtest.o: Tests/MatrixTest.cpp Math/Matrix.h
	g++ -c Tests/MatrixTest.cpp -o matrixtest.o

vector3: vector3.o vector3test.o
	g++ catch.o\
		vector3.o vector3test.o\
		-o vector3test.exe
	./vector3test.exe

vector2: vector2.o vector2test.o
	g++ catch.o\
		vector2.o vector2test.o\
		-o vector2test.exe
	./vector2test.exe

polygon2d: polygon2d.o polygon2dtest.o vector2.o
	g++ catch.o\
		polygon2d.o polygon2dtest.o\
		vector2.o\
		-o polygon2dtest.exe
	./polygon2dtest.exe

matrix: matrix.o matrixtest.o vector2.o vector3.o
	g++ catch.o\
		matrix.o matrixtest.o\
		vector2.o vector3.o\
		-o matrixtest.exe
	./matrixtest.exe


all: vector3 vector2 polygon2d matrix
	