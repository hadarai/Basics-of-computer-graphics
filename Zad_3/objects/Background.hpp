
class Background : public AGLDrawable
{
public:
   Background() : AGLDrawable(0)
   {
      setShaders();
      setBuffers();
   }
   void setShaders()
   {
      compileShaders(R"END(

         #version 330 
         #extension GL_ARB_explicit_uniform_location : require
         #extension GL_ARB_shading_language_420pack : require
         layout(location = 0) uniform vec2 sqare_center;
         layout(location = 1) uniform int is_black;
         out vec4 vcolor;
         void main(void) {

            const vec2 vertices[4] = 
               vec2[4](sqare_center + 0.1,
                       vec2(sqare_center.x + 0.1, sqare_center.y - 0.1),
                       vec2(sqare_center.x - 0.1, sqare_center.y + 0.1),
                       sqare_center - 0.1);

            vec4 colors[4] = 
               vec4[4](vec4(0.9, 0.9, 0.9, 1.0),
                       vec4(0.9, 0.9, 0.9, 1.0),
                       vec4(0.9, 0.9, 0.9, 1.0),
                       vec4(0.9, 0.9, 0.9, 1.0));
            if(is_black == 1)
            {
               colors= vec4[4](vec4(0.1, 0.1, 0.1, 1.0),
                               vec4(0.1, 0.1, 0.1, 1.0),
                               vec4(0.1, 0.1, 0.1, 1.0),
                               vec4(0.1, 0.1, 0.1, 1.0));
            }
            
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
   void setBuffers()
   {
      bindBuffers();
   }
   void draw(float x, float y, bool is_black)
   {
      bindProgram();
      bindBuffers();
      glUniform2f(0, x, y);
      glUniform1i(1, is_black);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glDrawArrays(GL_TRIANGLES, 1, 3);
   }
};
