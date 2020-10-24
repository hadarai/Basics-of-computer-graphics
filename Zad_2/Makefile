default: AGL3-example

LDFLAGS = -lGLEW $(libgl_$(shell uname -s))  -lglfw 

libgl_Linux = -lGL -lepoxy
libgl_Darwin = -framework OpenGL -lepoxy

%: %.cpp
	g++ -I. $< -o $@ AGL3Window.cpp  $(LDFLAGS) 
clean:
	rm a.out *.o *~ AGL3-example


