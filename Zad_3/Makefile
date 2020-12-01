default: labyrinth

LDFLAGS = -lGLEW $(libgl_$(shell uname -s))  -lglfw 

libgl_Linux = -lGL -lepoxy
libgl_Darwin = -framework OpenGL -lepoxy

%: %.cpp
	g++ -Wall -I. $< -o $@ AGL3Window.cpp $(LDFLAGS) 
clean:
	rm a.out *.o *~ labyrinth


