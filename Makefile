pinwheel: main.cpp
	g++ -O3 main.cpp -o pinwheel -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -ldl -lXxf86vm -lXinerama -lXcursor -lrt -lm -std=c++11

clean:
	rm pinwheel
