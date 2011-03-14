OS:=$(shell uname)

OBJS = Main.o Shader.o Model.o Camera.o Framebuffer.o MotionBlur.o Ship.o Body.o BodyEmitter.o HUD.o HUDComponent.o Scoreboard.o Gate.o

ifeq ($(OS), Darwin)
	CXXFLAGS = -g -Wall -Ilinux/include -I/usr/local/include/bullet
	LIBS = -Llinux/lib64 -Llinux/lib \
		-lBulletDynamics -lBulletCollision -lLinearMath \
		-framework sfml-network \
		-framework sfml-window \
		-framework sfml-graphics \
		-framework sfml-system \
		-framework sfml-audio \
		-lassimp \
		-framework OpenGL \
		-framework GLUT	

main: $(OBJS)
	g++ -g -Wall -o $@ $^ $(LIBS)

run: main
	LD_LIBRARY_PATH=linux/lib64 ./main

else

	CXXFLAGS = -g -Wall -Ilinux/include -I/usr/class/cs248/include -I/usr/local/include/bullet
	LIBS = -Llinux/lib64 -Llinux/lib \
		-lBulletDynamics -lBulletCollision -lLinearMath \
		-lsfml-network \
		-lsfml-window \
		-lsfml-graphics \
		-lsfml-system \
		-lassimp \
		-lGLU \
		-lGLEW

main: $(OBJS)
	g++ -g -Wall -o $@ $^ $(LIBS)

run: main
	LD_LIBRARY_PATH=/usr/class/cs248/lib:linux/lib64:linux/lib ./main
	
endif


clean:
	rm -rf main *.o