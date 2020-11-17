default: maze3D

LDFLAGS = -lGLEW $(libgl_$(shell uname -s)) -lglfw

libgl_Linux = -lGL 
libgl_Darwin = -framework OpenGL -lglfw

%: %.cpp
	g++ -I. -g $< -o $@ common/shader.cpp common/controls.cpp $(LDFLAGS)
	
clean:
	rm *.out *.o *~ maze3D
