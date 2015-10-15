all: main.exe

main.exe: obj/main.o obj/Scene.o obj/Sphere.o obj/Ray.o obj/Primitive.o obj/Cube.o obj/DirectionalLight.o obj/PointLight.o  obj/Light.o obj/Material.o
	g++ -O2 obj/main.o obj/Scene.o obj/Primitive.o obj/Sphere.o obj/Cube.o obj/Ray.o obj/DirectionalLight.o obj/PointLight.o obj/Light.o obj/Material.o -std=c++11 -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system  -o main.exe
	./main.exe

obj/main.o: src/main.cpp
	g++ -O2 -std=c++11 -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system -c src/main.cpp -o obj/main.o

obj/Primitive.o: src/Primitive.cpp
	g++ -O2 -c src/Primitive.cpp -std=c++11 -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system  -o obj/Primitive.o

obj/Sphere.o: src/Sphere.cpp
	g++ -O2 -c src/Sphere.cpp -std=c++11 -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system  -o obj/Sphere.o

obj/Cube.o: src/Cube.cpp
	g++ -O2 -c src/Cube.cpp -std=c++11 -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system  -o obj/Cube.o

obj/Scene.o: src/Scene.cpp
	g++ -O2 -c src/Scene.cpp -std=c++11 -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system  -o obj/Scene.o

obj/Ray.o: src/Ray.cpp
	g++ -O2 -c src/Ray.cpp -std=c++11 -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system  -o obj/Ray.o

obj/Light.o: src/Light.cpp
	g++ -O2 -c src/Light.cpp -std=c++11 -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system  -o obj/Light.o

obj/DirectionalLight.o: src/DirectionalLight.cpp
	g++ -O2 -c src/DirectionalLight.cpp -std=c++11 -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system  -o obj/DirectionalLight.o

obj/PointLight.o: src/PointLight.cpp
	g++ -O2 -c src/PointLight.cpp -std=c++11 -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system  -o obj/PointLight.o

obj/Material.o: src/Material.cpp
	g++ -O2 -c src/Material.cpp -std=c++11 -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system  -o obj/Material.o

clean:
	rm -rf obj/*.o *.exe
