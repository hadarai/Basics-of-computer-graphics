default: aquarium

LDFLAGS = -lGLEW $(libgl_$(shell uname -s)) -lglfw

libgl_Linux = -lGL 
libgl_Darwin = -framework OpenGL -lglfw

%: %.cpp
	g++ -I. -std=gnu++17 -g $< -o $@ common/shader.cpp common/controls.cpp common/Window.cpp $(LDFLAGS)
	
clean:
	rm *.out *.o *~ aquarium
