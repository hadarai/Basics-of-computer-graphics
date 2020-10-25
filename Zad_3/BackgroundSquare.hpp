
class BackgroundSqare : public AGLDrawable
{
public:
   BackgroundSqare() : AGLDrawable(0)
   {
      setShaders();
   }
   void setShaders()
   {
      compileShaders(R"END(

         #version 330 
         out vec4 vcolor;
         void main(void) {

            const vec2 vertices[4] = // to sa wspolrzedne wierzcholkow trojkata
               vec2[4](vec2(-0.9, -0.9),
                       vec2( 0.9, -0.9),                       
                       vec2(-0.9,  0.9),
                       vec2( 0.9,  0.9)); 

            const vec4 colors[4] = // a to jakies wspolrzene kolorow?
               vec4[4](vec4(0.0, 0.0, 0.0, 1.0),
                       vec4(1.0, 0.0, 0.0, 1.0),
                       vec4(0.0, 0.0, 1.0, 1.0),
                       vec4(0.0, 1.0, 0.0, 1.0));

            vcolor      = colors[gl_VertexID];
            gl_Position = vec4(vertices[gl_VertexID], 0.5, 1.0); 
         }

      )END",
                     R"END(

         #version 330 
         in  vec4 vcolor;
         out vec4 color;

         void main(void) {
            color = vcolor;
         } 

      )END");
   }
   void draw()
   {
      bindProgram();
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glDrawArrays(GL_TRIANGLES, 1, 3);
   }
};
