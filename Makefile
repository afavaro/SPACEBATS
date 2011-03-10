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
OBJS = Main.o Shader.o Model.o Camera.o Framebuffer.o MotionBlur.o Ship.o Body.o

main: $(OBJS)
	g++ -g -Wall -o $@ $^ $(LIBS)


run: main
	LD_LIBRARY_PATH=/usr/class/cs248/lib:linux/lib64:linux/lib ./main

clean:
	rm -rf main *.o
