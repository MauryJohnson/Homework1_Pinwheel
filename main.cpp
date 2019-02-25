#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "buffer.cpp"
#include <bits/stdc++.h>
//#include "Triangle.h"
//#include "Texture.h"
#include <vector>
#include "Shader.h"
//<<<<<<< HEAD
#include <tgmath.h>
#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//=======
#include <iostream>

struct Point3D{
GLfloat X;
GLfloat Y;
GLfloat Z;
};

//<<<<<<< HEAD
#define PI 3.14159

//Easy definition
typedef struct Point3D Point3D;

//ArrayList for Point2D Objects
typedef std::vector<Point3D> PointVector;

namespace ShapeBuilder{

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius){
	int i;
	int triangleAmount = 20; //# of triangles used to draw circle
	
	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;
	
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y); // center of circle
		for(i = 0; i <= triangleAmount;i++) { 
			glVertex2f(
		            x + (radius * cos(i *  twicePi / triangleAmount)), 
			    y + (radius * sin(i * twicePi / triangleAmount))
			);
		}
	glEnd();
}

std::vector<GLfloat> CircleVector(GLfloat x,GLfloat y,GLfloat radius,unsigned int points){
std::vector<GLfloat> Result;
Result.reserve(20);
// (x-a)^2 + (y-b)^2 = r^2
//  x and y positions initially are a and b
// number of points specify how many points to take evenly from circle range
// x = rcos(theta), y = rsin(theta)
// 2pi is radian of circle, 2pi/points is step size for circle!!
GLfloat x_pos=0.0;
GLfloat y_pos=0.0;
GLfloat Rad = 0.0;
printf("\n CIRCLE:\n");

GLfloat ER = 2*PI;

for(unsigned int i=0; i<=points;i+=1){

//Portion point of circle to label
Rad = (ER*i)/(points*1.0);
x_pos = radius*cos(Rad);
y_pos = radius*sin(Rad);
Result.push_back(x_pos);
Result.push_back(y_pos);
}

return Result;
}


//Cone, but backwards so start from max radius 
std::vector<std::tuple<std::vector<GLfloat>,GLuint>> ConeVector(GLfloat x, GLfloat y,GLfloat zMax,GLfloat zMin){

//MUST be in order of Circle Coordinates, Z Coordinate
std::vector<std::tuple<std::vector<GLfloat>,GLuint>> Circles;
Circles.reserve(20);

//For each Z coordinate, create this circle
for(int i=0; i<=zMax; i+=1){

std::vector<GLfloat> Circle=ShapeBuilder::CircleVector(x,y,zMax-i,(i+1)*2000);

auto t = std::make_tuple(Circle,i);

Circles.push_back(t);

}

//Returning circles ordered from greatest to least
return Circles;
}

std::vector<GLuint> CircleIndices(unsigned int Len){
std::vector<GLuint> CI;
CI.reserve(100);

for(unsigned int i=0;i<Len;i+=1){
printf("\n Indice: %u",i);
CI.push_back(i);
}

return CI;
}

}

bool keys[1024];

bool Front = false;
GLfloat RotationDirection = 1.0;
GLfloat RotationSpeed = -5.0;
GLfloat MAXRS = 10000.0;
GLfloat MINRS = -10000.0;

void key_callback(GLFWwindow *window,int key,int scancode,int action,int mode)
{
    if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS)
        glfwSetWindowShouldClose(window,GL_TRUE);

    if(key==GLFW_KEY_UP && action==GLFW_PRESS)
    {
        Front = !Front;
    }

    if(key==GLFW_KEY_DOWN && action==GLFW_PRESS)
    {
        Front = !Front;
    }

    if((key==GLFW_KEY_RIGHT && action==GLFW_PRESS ) /*|| keys[key]==true*/){
	//if(RotationSpeed>MINRS)
	GLfloat R2 = RotationSpeed - 20.0f;
	if(RotationSpeed>MINRS)
	RotationSpeed = R2;

	printf("\n Decrease Rotation Speed:%f",RotationSpeed);
   	if(RotationSpeed==0){
	RotationSpeed = -0.01f;
	}

	//if(RotationSpeed<0)
	//RotationDirection = -1.0f;

	//keys[key]=true;
    }

    else if((key==GLFW_KEY_LEFT && action==GLFW_PRESS) /*|| keys[key] == true*/){
	//RotationSpeed+= RotationSpeed>=MAXRS? 0.0f:10.0f;
   	GLfloat R3 = RotationSpeed + 20.0f;
	if(RotationSpeed<MAXRS)
	RotationSpeed = R3;
	printf("\n Increase Rotation Speed:%f",RotationSpeed);        

	if(RotationSpeed==0)
	RotationSpeed==0.01f;

	//if(RotationSpeed>0)
        //RotationDirection = 1.0f;


	//keys[key] = true;
    }
    
    if(action==GLFW_PRESS) keys[key]=true;
    if(action==GLFW_RELEASE) keys[key]=false;
}

int main()
{
    glfwInit();
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);

    GLFWwindow *window=glfwCreateWindow(800,600,"Learn OpenGL",nullptr,nullptr);
    if(window==nullptr)
    {
        std::cout<<"Failed to create GLFW window!"<<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental=GL_TRUE;
    if(glewInit()!=GLEW_OK)
    {
        std::cout<<"Failed to initialize GLEW!"<<std::endl;
        return -1;
    }

    int width,height;
    glfwGetFramebufferSize(window,&width,&height);
    glViewport(0,0,width,height);

    glm::mat4 projection(1.0f);
    projection=glm::perspective(glm::radians(45.0f),(float)width/height,0.1f,100.0f);

    glfwSetKeyCallback(window,key_callback);
       
    //Triangle1 
    GLuint indices1[]={
        0,1,2 
    };

    GLfloat vertices[]={
          0.4f,  0.0f, 0.0f,
          0.75f,  0.5f, 0.0f,
          0.05f,  0.0f, 0.0f,
          //0.75f,  0.0f, 0.5f
    };
    /////////
  
    //Triangle 1 and Triangle 2 Crease 
    GLuint indices8[]={
	0,1,2
    };

    GLfloat vertices8[]={
	0.0f,0.0f,0.0f,
	0.0f,0.40f,0.0f,
	0.40f,0.0f,0.0f
    };

    //T1 T2 Crease 2
     GLuint indices12[]={
        0,1,2
    };

    GLfloat vertices12[]={
        0.75f,0.5f,0.0f,
        0.0f,0.4f,0.0f,
        0.40f,0.0f,0.0f
    };

    //BASE 2
    GLuint indices7[]={
	0,1,2
    };
    GLfloat vertices7[]={
         -0.025f,0.0f,0.0f,
         0.025f,-1.0f,0.0f,
         0.025f,0.0f,0.0f
         //0.0f,-1.0f,0.10f
    };

    //Triangle2
    GLuint indices2[]={
       0,1,2
    };
 
    GLfloat vertices2[]={
        0.0f,0.05f,0.0f,
	0.0f,0.40f,0.0f,
	-0.5f,0.75f,0.0f,
	0.0f,0.75f,0.5f
    } ;
    /////////

     // Slice T2 - T3
    GLuint indices9[]={
       0,1,2
    };

    GLfloat vertices9[]={
        0.0f,0.0f,0.0f,
        0.0f,0.40f,0.0f,
        -0.4f,0.0f,0.0f
    } ;

    //T2 T3 Crease 2
     GLuint indices13[]={
        0,1,2
    };

    GLfloat vertices13[]={
        -0.5f,0.75f,0.0f,
        0.0f,0.4f,0.0f,
        -0.40f,0.0f,0.0f
    };

    //Triangle3
    GLuint indices3[]={
       0,1,2
    };

    GLfloat vertices3[]={
         -0.05f,0.0f,0.0f,
        -0.40f,0.0f,0.0f,
//<<<<<<< HEAD
        -0.75f,-0.5f,0.0f,
	-0.75f,0.0f,0.5f
    } ;
    /////////

    // Slice T3 - T4
    GLuint indices10[]={
       0,1,2
    };

    GLfloat vertices10[]={
        0.0f,0.0f,0.0f,
        0.0f,-0.40f,0.0f,
        -0.4f,0.0f,0.0f
    };

     //T3 T4 Crease 2
     GLuint indices14[]={
        0,1,2
    };

    GLfloat vertices14[]={
        -0.75f,-0.5f,0.0f,
        0.0f,-0.4f,0.0f,
        -0.40f,0.0f,0.0f
    };
 
    //Triangle4
    GLuint indices4[]={
       0,1,2
    };

    GLfloat vertices4[]={
         0.0f,-0.05f,0.0f,
         0.0f,-0.40f,0.0f,
         0.5f,-0.75f,0.0f,
	 0.0f,-0.75f,0.05f
    };
   /////////
  
    // Slice T4 - T1
    GLuint indices11[]={
       0,1,2
    };

    GLfloat vertices11[]={
        0.0f, -0.4f, 0.0,
	0.0f,0.0f,0.0f,
	0.4f,0.0f,0.0f
    } ;

    //T4 T1 Crease 2
     GLuint indices15[]={
        0,1,2
    };

    GLfloat vertices15[]={
        0.5f,-0.75f,0.0f,
        0.0f,-0.4f,0.0f,
        0.40f,0.0f,0.0f
    };

   //Triangle5, BASE
    GLuint indices6[]={
       0,1,2
    };

    GLfloat vertices6[]={
         -0.025f,0.0f,0.0f,
         -0.025f,-1.0f,0.0f,
         0.025f,-1.0f,0.0f//,
	 //0.0f,-1.0f,0.10f
    };
   /////////
   
   GLuint VAO[16],EBO[16]/*,VBO,*//*EBO*/;
   glGenBuffers(16,EBO);
   glGenVertexArrays(16,VAO);

   //For first one
   glBindVertexArray(VAO[0]);
   
   glBuffer VBO(GL_ARRAY_BUFFER);
   VBO.setdata(vertices,12,sizeof(GLfloat),GL_STATIC_DRAW);
   VBO.bind(0,3);
   
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices1),indices1,GL_STATIC_DRAW); 
  
   //WRAP UP VERTEX ATTRIBS
   //Set ALLLLL Vtex attrib pointers, MAKES IT FASTER??
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
   glEnableVertexAttribArray(0);
   
   // unbind the vertex array object   
   glBindVertexArray(0);
   //WRAP UP VERTEX ATTRIBS 

   //
    
   //For third one
   glBindVertexArray(/* CHANGER 1 */VAO[2]/* CHANGER 1 */);

   glBuffer/* CHANGER 1.1 */ VBO3(GL_ARRAY_BUFFER);
   /* CHANGER 1.2 */VBO3.setdata(/* CHANGER 2 */vertices3/* CHANGER 2 */,12,sizeof(GLfloat),GL_STATIC_DRAW);
   /* CHANGER 2.1 */VBO3.bind(0,3);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,/* CHANGER 3 */EBO[2]/* CHANGER 3 */);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(/* CHANGER 4 */indices3),/* CHANGER 4 */indices3,GL_STATIC_DRAW);

   //WRAP UP VERTEX ATTRIBS
   //Set ALLLLL Vtex attrib pointers, MAKES IT FASTER??
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
   glEnableVertexAttribArray(0);

   // unbind the vertex array object
   glBindVertexArray(0);
   //WRAP UP VERTEX ATTRIBS 
   //

    //Triangle5
   glBindVertexArray(/* CHANGER 1 */VAO[5]/* CHANGER 1 */);

   glBuffer/* CHANGER 1.1 */ VBO6(GL_ARRAY_BUFFER);
   /* CHANGER 1.2 */VBO6.setdata(/* CHANGER 2 */vertices6/* CHANGER 2 */,12,sizeof(GLfloat),GL_STATIC_DRAW);
   /* CHANGER 2.1 */VBO6.bind(0,3);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,/* CHANGER 3 */EBO[5]/* CHANGER 3 */);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(/* CHANGER 4 */indices6),/* CHANGER 4 */indices6,GL_STATIC_DRAW);

   //WRAP UP VERTEX ATTRIBS
   //Set ALLLLL Vtex attrib pointers, MAKES IT FASTER??
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
   glEnableVertexAttribArray(0);

   // unbind the vertex array object
   glBindVertexArray(0);
   //WRAP UP VERTEX ATTRIBS 

   glBindVertexArray(/* CHANGER 1 */VAO[7]/* CHANGER 1 */);

   glBuffer/* CHANGER 1.1 */ VBO7(GL_ARRAY_BUFFER);
   /* CHANGER 1.2 */VBO7.setdata(/* CHANGER 2 */vertices7/* CHANGER 2 */,12,sizeof(GLfloat),GL_STATIC_DRAW);
   /* CHANGER 2.1 */VBO7.bind(0,3);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,/* CHANGER 3 */EBO[7]/* CHANGER 3 */);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(/* CHANGER 4 */indices7),/* CHANGER 4 */indices7,GL_STATIC_DRAW);

   //WRAP UP VERTEX ATTRIBS
   //Set ALLLLL Vtex attrib pointers, MAKES IT FASTER??
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
   glEnableVertexAttribArray(0);

   // unbind the vertex array object
   glBindVertexArray(0);

   //For second one
   glBindVertexArray(VAO[1]);

   glBuffer VBO2(GL_ARRAY_BUFFER);
   VBO2.setdata(vertices2,12,sizeof(GLfloat),GL_STATIC_DRAW);   
   VBO2.bind(0,3);
   
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO[1]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices2),indices2,GL_STATIC_DRAW);
    
   //WRAP UP VERTEX ATTRIBS
   //Set ALLLLL Vtex attrib pointers, MAKES IT FASTER??
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
   glEnableVertexAttribArray(0);
   
//<<<<<<< HEAD
   // unbind the vertex array object
   glBindVertexArray(0);
   //WRAP UP VERTEX ATTRIBS 
   //
   
   //For fourth one
   glBindVertexArray(/* CHANGER 1 */VAO[3]/* CHANGER 1 */);

   glBuffer/* CHANGER 1.1 */ VBO4(GL_ARRAY_BUFFER);
   /* CHANGER 1.2 */VBO4.setdata(/* CHANGER 2 */vertices4/* CHANGER 2 */,12,sizeof(GLfloat),GL_STATIC_DRAW);
   /* CHANGER 2.1 */VBO4.bind(0,3);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,/* CHANGER 3 */EBO[3]/* CHANGER 3 */);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(/* CHANGER 4 */indices4),/* CHANGER 4 */indices4,GL_STATIC_DRAW);

   //WRAP UP VERTEX ATTRIBS
   //Set ALLLLL Vtex attrib pointers, MAKES IT FASTER??
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
   glEnableVertexAttribArray(0);

   // unbind the vertex array object
   glBindVertexArray(0);
   //WRAP UP VERTEX ATTRIBS 
  
   glBindVertexArray(VAO[4]);

   std::vector<GLfloat> CV = ShapeBuilder::CircleVector(0.0f,0.0f,0.05,5000);
   
   glBuffer VBO9(GL_ARRAY_BUFFER);
   
   VBO9.setdata(CV.data(),CV.size(),sizeof(GLfloat),GL_STATIC_DRAW);
   VBO9.bind(0,3);
   //glBindVertexArray(VAO[4]);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,/* CHANGER 3 */EBO[4]/* CHANGER 3 */);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,CV.size()*sizeof(GLfloat),/* CHANGER 4 */CV.data(),GL_STATIC_DRAW);

   //WRAP UP VERTEX ATTRIBS
   //Set ALLLLL Vtex attrib pointers, MAKES IT FASTER??
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
   glEnableVertexAttribArray(0);

   // unbind the vertex array object
   glBindVertexArray(0);

   glBindVertexArray(VAO[6]);

   std::vector<GLfloat> CV2 = ShapeBuilder::CircleVector(0.0f,0.0f,0.060,5000);
   
   glBuffer VBO8(GL_ARRAY_BUFFER);
   VBO8.setdata(CV2.data(),CV2.size(),sizeof(GLfloat),GL_STATIC_DRAW);
   VBO8.bind(0,3);
   //glBindVertexArray(VAO[4]);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,/* CHANGER 3 */EBO[6]/* CHANGER 3 */);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,CV2.size()*sizeof(GLfloat),/* CHANGER 4 */CV2.data(),GL_STATIC_DRAW);

   //WRAP UP VERTEX ATTRIBS
   //Set ALLLLL Vtex attrib pointers, MAKES IT FASTER??
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
   glEnableVertexAttribArray(0);

   // unbind the vertex array object
   glBindVertexArray(0);

   //Triangle 1 + 2 crease
   glBindVertexArray(/* CHANGER 1 */VAO[8]/* CHANGER 1 */);

   glBuffer/* CHANGER 1.1 */ VBO10(GL_ARRAY_BUFFER);
   /* CHANGER 1.2 */VBO10.setdata(/* CHANGER 2 */vertices8/* CHANGER 2 */,12,sizeof(GLfloat),GL_STATIC_DRAW);
   /* CHANGER 2.1 */VBO10.bind(0,3);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,/* CHANGER 3 */EBO[8]/* CHANGER 3 */);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(/* CHANGER 4 */indices8),/* CHANGER 4 */indices8,GL_STATIC_DRAW);

   //WRAP UP VERTEX ATTRIBS
   //Set ALLLLL Vtex attrib pointers, MAKES IT FASTER??
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
   glEnableVertexAttribArray(0);

   // unbind the vertex array object
   glBindVertexArray(0);

   //Triangle 2 + 3 crease
   glBindVertexArray(/* CHANGER 1 */VAO[9]/* CHANGER 1 */);

   glBuffer/* CHANGER 1.1 */ VBO11(GL_ARRAY_BUFFER);
   /* CHANGER 1.2 */VBO11.setdata(/* CHANGER 2 */vertices9/* CHANGER 2 */,12,sizeof(GLfloat),GL_STATIC_DRAW);
   /* CHANGER 2.1 */VBO11.bind(0,3);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,/* CHANGER 3 */EBO[9]/* CHANGER 3 */);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(/* CHANGER 4 */indices9),/* CHANGER 4 */indices9,GL_STATIC_DRAW);

   //WRAP UP VERTEX ATTRIBS
   //Set ALLLLL Vtex attrib pointers, MAKES IT FASTER??
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
   glEnableVertexAttribArray(0);

   // unbind the vertex array object
   glBindVertexArray(0);

   //Triangle 3 + 4 crease
   glBindVertexArray(/* CHANGER 1 */VAO[10]/* CHANGER 1 */);

   glBuffer/* CHANGER 1.1 */ VBO12(GL_ARRAY_BUFFER);
   /* CHANGER 1.2 */VBO12.setdata(/* CHANGER 2 */vertices10/* CHANGER 2 */,12,sizeof(GLfloat),GL_STATIC_DRAW);
   /* CHANGER 2.1 */VBO12.bind(0,3);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,/* CHANGER 3 */EBO[10]/* CHANGER 3 */);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(/* CHANGER 4 */indices10),/* CHANGER 4 */indices10,GL_STATIC_DRAW);

   //WRAP UP VERTEX ATTRIBS
   //Set ALLLLL Vtex attrib pointers, MAKES IT FASTER??
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
   glEnableVertexAttribArray(0);

   // unbind the vertex array object
   glBindVertexArray(0);

   //Triangle 4+1 crease
    glBindVertexArray(/* CHANGER 1 */VAO[11]/* CHANGER 1 */);

   glBuffer/* CHANGER 1.1 */ VBO13(GL_ARRAY_BUFFER);
   /* CHANGER 1.2 */VBO13.setdata(/* CHANGER 2 */vertices11/* CHANGER 2 */,12,sizeof(GLfloat),GL_STATIC_DRAW);
   /* CHANGER 2.1 */VBO13.bind(0,3);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,/* CHANGER 3 */EBO[11]/* CHANGER 3 */);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(/* CHANGER 4 */indices11),/* CHANGER 4 */indices11,GL_STATIC_DRAW);

   //WRAP UP VERTEX ATTRIBS
   //Set ALLLLL Vtex attrib pointers, MAKES IT FASTER??
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
   glEnableVertexAttribArray(0);

   // unbind the vertex array object
   glBindVertexArray(0);

   //Triangle 1+2 crease 2
    glBindVertexArray(/* CHANGER 1 */VAO[12]/* CHANGER 1 */);

   glBuffer/* CHANGER 1.1 */ VBO14(GL_ARRAY_BUFFER);
   /* CHANGER 1.2 */VBO14.setdata(/* CHANGER 2 */vertices12/* CHANGER 2 */,12,sizeof(GLfloat),GL_STATIC_DRAW);
   /* CHANGER 2.1 */VBO14.bind(0,3);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,/* CHANGER 3 */EBO[12]/* CHANGER 3 */);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(/* CHANGER 4 */indices12),/* CHANGER 4 */indices12,GL_STATIC_DRAW);

   //WRAP UP VERTEX ATTRIBS
   //Set ALLLLL Vtex attrib pointers, MAKES IT FASTER??
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
   glEnableVertexAttribArray(0);

   // unbind the vertex array object
   glBindVertexArray(0);

   //Triangle 2+3 crease 2
   glBindVertexArray(/* CHANGER 1 */VAO[13]/* CHANGER 1 */);

   glBuffer/* CHANGER 1.1 */ VBO15(GL_ARRAY_BUFFER);
   /* CHANGER 1.2 */VBO15.setdata(/* CHANGER 2 */vertices13/* CHANGER 2 */,12,sizeof(GLfloat),GL_STATIC_DRAW);
   /* CHANGER 2.1 */VBO15.bind(0,3);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,/* CHANGER 3 */EBO[13]/* CHANGER 3 */);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(/* CHANGER 4 */indices13),/* CHANGER 4 */indices13,GL_STATIC_DRAW);

   //WRAP UP VERTEX ATTRIBS
   //Set ALLLLL Vtex attrib pointers, MAKES IT FASTER??
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
   glEnableVertexAttribArray(0);

   // unbind the vertex array object
   glBindVertexArray(0);

   //Triangle 3+4 crease 2

   glBindVertexArray(/* CHANGER 1 */VAO[14]/* CHANGER 1 */);

   glBuffer/* CHANGER 1.1 */ VBO16(GL_ARRAY_BUFFER);
   /* CHANGER 1.2 */VBO16.setdata(/* CHANGER 2 */vertices14/* CHANGER 2 */,12,sizeof(GLfloat),GL_STATIC_DRAW);
   /* CHANGER 2.1 */VBO16.bind(0,3);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,/* CHANGER 3 */EBO[14]/* CHANGER 3 */);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(/* CHANGER 4 */indices14),/* CHANGER 4 */indices14,GL_STATIC_DRAW);

   //WRAP UP VERTEX ATTRIBS
   //Set ALLLLL Vtex attrib pointers, MAKES IT FASTER??
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
   glEnableVertexAttribArray(0);

   // unbind the vertex array object
   glBindVertexArray(0);

   //triangle 4+1 crease 2
   glBindVertexArray(/* CHANGER 1 */VAO[15]/* CHANGER 1 */);

   glBuffer/* CHANGER 1.1 */ VBO17(GL_ARRAY_BUFFER);
   /* CHANGER 1.2 */VBO17.setdata(/* CHANGER 2 */vertices15/* CHANGER 2 */,12,sizeof(GLfloat),GL_STATIC_DRAW);
   /* CHANGER 2.1 */VBO17.bind(0,3);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,/* CHANGER 3 */EBO[15]/* CHANGER 3 */);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(/* CHANGER 4 */indices15),/* CHANGER 4 */indices15,GL_STATIC_DRAW);

   //WRAP UP VERTEX ATTRIBS
   //Set ALLLLL Vtex attrib pointers, MAKES IT FASTER??
   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
   glEnableVertexAttribArray(0);

   // unbind the vertex array object
   glBindVertexArray(0);


   //V SHADER//
   //Vertex shader, fragment shader
   Shader VFShader("transform.vs","shader.frag"); 
   Shader VFShader2("transform.vs","shader2.frag");
   Shader VFShader3("transform.vs","shader.frag");
   Shader VFShader4("transform.vs","shader2.frag");
   //Shader VFShader5("circleshader.vs","circleshader.frag");
   Shader VFShader5("transform.vs","shader.frag");
   
   Shader VFShader6("transform.vs","shaderR.frag");
   Shader VFShader9("transform.vs","shaderR.frag");   

   Shader VFShader7("transform.vs","circleshader.frag");
   Shader VFShader8("transform.vs","circleshader2.frag");
   //V SHADER// 
  
   //For creases 
   Shader VFShader10("transform.vs","shaders1.frag");
   //Creases
   Shader VFShader11("transform.vs","shaders2.frag");  
   Shader VFShader12("transform.vs","shaders3.frag");

   glm::mat4 view(1.0f);

   view=glm::translate(view,glm::vec3(0.0f,0.0f,-3.0f));

   //glEnable(GL_DEPTH_TEST);

   while(!glfwWindowShouldClose(window)){
    glfwPollEvents();
    
    glClearColor(.2f,.3f,.3f,.1f);
//<<<<<<< HEAD
    glClear(GL_COLOR_BUFFER_BIT);
    
    GLfloat current_frame = glfwGetTime();
    //delta_time=current_frame-last_frame;
    //last_frame=current_frame;

   if(!Front){

    //DRAW BASE 1
    VFShader6.Use();
    glBindVertexArray(VAO[5]);
    //TRANSFORMATION BASE 

    glm::mat4 /* CHANGE */ tb(1.0f);
    /* CHANGE */tb=translate(/* CHANGE */ tb,glm::vec3(0.0f,-0.0f,0.0f));

//    /* CHANGE */tb=rotate(/* CHANGE */ tb,glm::radians((GLfloat)glfwGetTime()*50.0f),glm::vec3(0.0f,0.0f,1.0f));

    GLuint /* CHANGE */tl=glGetUniformLocation(/* CHANGE */ VFShader6.program,"transform");

    glUniformMatrix4fv(/* CHANGE */ tl,1,GL_FALSE,glm::value_ptr(/* CHANGE  */tb));
    //TRANSFORMATION PRIMER BASE

    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    //Draw BASE 2
    VFShader9.Use();
    glBindVertexArray(VAO[7]);
    //TRANSFORMATION BASE 

    glm::mat4 /* CHANGE */ tb9(1.0f);
    /* CHANGE */tb9=translate(/* CHANGE */ tb9,glm::vec3(0.0f,-0.0f,0.0f));

//    /* CHANGE */tb=rotate(/* CHANGE */ tb,glm::radians((GLfloat)glfwGetTime()*50.0f),glm::vec3(0.0f,0.0f,1.0f));

    GLuint /* CHANGE */tl9=glGetUniformLocation(/* CHANGE */ VFShader9.program,"transform");

    glUniformMatrix4fv(/* CHANGE */ tl9,1,GL_FALSE,glm::value_ptr(/* CHANGE  */tb9));
    //TRANSFORMATION PRIMER BASE

    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    //END DRAW BASE

   }

    //Draw Crease 1
    VFShader10.Use();
    
    glm::mat4 /* CHANGE */ trans10(1.0f);
    /* CHANGE */trans10=translate(/* CHANGE */ trans10,glm::vec3(0.0f,-0.0f,0.0f));
    /* CHANGE */trans10=rotate(/* CHANGE */ trans10,glm::radians((GLfloat)glfwGetTime()*RotationSpeed),glm::vec3(0.0f,0.0f,RotationDirection));

    GLuint /* CHANGE */transform_location10=glGetUniformLocation(/* CHANGE */ VFShader10.program,"transform");
    
    glUniformMatrix4fv(/* CHANGE */ transform_location10,1,GL_FALSE,glm::value_ptr(/* CHANGE  */trans10));
    //TRANSFORMATION PRIMER2

/*
=======
>>>>>>> ec5c0f0cbdf5aa1c4a9ae258e148848cc8849274
*/  glBindVertexArray(VAO[8]);
    //glDrawArrays(GL_TRIANGLES,0,4);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    //glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    //END DRAW CREASE 1

    //Draw Crease 11
    VFShader12.Use();

    glm::mat4 /* CHANGE */ trans15(1.0f);
    /* CHANGE */trans15=translate(/* CHANGE */ trans15,glm::vec3(0.0f,-0.0f,0.0f));
    /* CHANGE */trans15=rotate(/* CHANGE */ trans15,glm::radians((GLfloat)glfwGetTime()*RotationSpeed),glm::vec3(0.0f,0.0f,RotationDirection));

    GLuint /* CHANGE */transform_location15=glGetUniformLocation(/* CHANGE */ VFShader12.program,"transform");

    glUniformMatrix4fv(/* CHANGE */ transform_location15,1,GL_FALSE,glm::value_ptr(/* CHANGE  */trans15));
    //TRANSFORMATION PRIMER2

/*
=======
>>>>>>> ec5c0f0cbdf5aa1c4a9ae258e148848cc8849274
*/  glBindVertexArray(VAO[12]);
    //glDrawArrays(GL_TRIANGLES,0,4);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    //glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    //End DRAW CREASE 11

    //Draw Crease 2

    VFShader11.Use();

    glm::mat4 /* CHANGE */ trans11(1.0f);
    /* CHANGE */trans11=translate(/* CHANGE */ trans11,glm::vec3(0.0f,-0.0f,0.0f));
    /* CHANGE */trans11=rotate(/* CHANGE */ trans11,glm::radians((GLfloat)glfwGetTime()*RotationSpeed),glm::vec3(0.0f,0.0f,RotationDirection));

    GLuint /* CHANGE */transform_location11=glGetUniformLocation(/* CHANGE */ VFShader11.program,"transform");

    glUniformMatrix4fv(/* CHANGE */ transform_location11,1,GL_FALSE,glm::value_ptr(/* CHANGE  */trans11));
    //TRANSFORMATION PRIMER2

/*
=======
>>>>>>> ec5c0f0cbdf5aa1c4a9ae258e148848cc8849274
*/  glBindVertexArray(VAO[9]);
    //glDrawArrays(GL_TRIANGLES,0,4);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    //glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    //END DRAW CREASE 2

    //Draw Crease 22

    VFShader11.Use();

    glm::mat4 /* CHANGE */ trans16(1.0f);
    /* CHANGE */trans16=translate(/* CHANGE */ trans16,glm::vec3(0.0f,-0.0f,0.0f));
    /* CHANGE */trans16=rotate(/* CHANGE */ trans16,glm::radians((GLfloat)glfwGetTime()*RotationSpeed),glm::vec3(0.0f,0.0f,RotationDirection));

    GLuint /* CHANGE */transform_location16=glGetUniformLocation(/* CHANGE */ VFShader11.program,"transform");

    glUniformMatrix4fv(/* CHANGE */ transform_location16,1,GL_FALSE,glm::value_ptr(/* CHANGE  */trans16));
    //TRANSFORMATION PRIMER2

/*
=======
>>>>>>> ec5c0f0cbdf5aa1c4a9ae258e148848cc8849274
*/  glBindVertexArray(VAO[13]);
    //glDrawArrays(GL_TRIANGLES,0,4);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    //glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);   
 
    //END DRAW CREASE 22

    //Draw Crease 3

    VFShader10.Use();

    glm::mat4 /* CHANGE */ trans13(1.0f);
    /* CHANGE */trans13=translate(/* CHANGE */ trans13,glm::vec3(0.0f,-0.0f,0.0f));
    /* CHANGE */trans13=rotate(/* CHANGE */ trans13,glm::radians((GLfloat)glfwGetTime()*RotationSpeed),glm::vec3(0.0f,0.0f,RotationDirection));

    GLuint /* CHANGE */transform_location13=glGetUniformLocation(/* CHANGE */ VFShader10.program,"transform");

    glUniformMatrix4fv(/* CHANGE */ transform_location13,1,GL_FALSE,glm::value_ptr(/* CHANGE  */trans13));
    //TRANSFORMATION PRIMER2

/*
=======
>>>>>>> ec5c0f0cbdf5aa1c4a9ae258e148848cc8849274
*/  glBindVertexArray(VAO[10]);
    //glDrawArrays(GL_TRIANGLES,0,4);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    //glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    //END DRAW CREASE 3

    //Draw Crease 33

    VFShader12.Use();

    glm::mat4 /* CHANGE */ trans17(1.0f);
    /* CHANGE */trans17=translate(/* CHANGE */ trans17,glm::vec3(0.0f,-0.0f,0.0f));
    /* CHANGE */trans17=rotate(/* CHANGE */ trans17,glm::radians((GLfloat)glfwGetTime()*RotationSpeed),glm::vec3(0.0f,0.0f,RotationDirection));

    GLuint /* CHANGE */transform_location17=glGetUniformLocation(/* CHANGE */ VFShader12.program,"transform");

    glUniformMatrix4fv(/* CHANGE */ transform_location17,1,GL_FALSE,glm::value_ptr(/* CHANGE  */trans17));
    //TRANSFORMATION PRIMER2

/*
=======
>>>>>>> ec5c0f0cbdf5aa1c4a9ae258e148848cc8849274
*/  glBindVertexArray(VAO[14]);
    //glDrawArrays(GL_TRIANGLES,0,4);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    //glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    //END DRAW CREASE 33

    //Draw Crease 4

    VFShader11.Use();

    glm::mat4 /* CHANGE */ trans14(1.0f);
    /* CHANGE */trans14=translate(/* CHANGE */ trans14,glm::vec3(0.0f,-0.0f,0.0f));
    /* CHANGE */trans14=rotate(/* CHANGE */ trans14,glm::radians((GLfloat)glfwGetTime()*RotationSpeed),glm::vec3(0.0f,0.0f,RotationDirection));

    GLuint /* CHANGE */transform_location14=glGetUniformLocation(/* CHANGE */ VFShader11.program,"transform");

    glUniformMatrix4fv(/* CHANGE */ transform_location14,1,GL_FALSE,glm::value_ptr(/* CHANGE  */trans14));
    //TRANSFORMATION PRIMER2

     glBindVertexArray(VAO[11]);
    //glDrawArrays(GL_TRIANGLES,0,4);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    //glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);


    //END DRAW CREASE 4

    //Draw Crease 44

    VFShader11.Use();

    glm::mat4 /* CHANGE */ trans18(1.0f);
    /* CHANGE */trans18=translate(/* CHANGE */ trans18,glm::vec3(0.0f,-0.0f,0.0f));
    /* CHANGE */trans18=rotate(/* CHANGE */ trans18,glm::radians((GLfloat)glfwGetTime()*RotationSpeed),glm::vec3(0.0f,0.0f,RotationDirection));

    GLuint /* CHANGE */transform_location18=glGetUniformLocation(/* CHANGE */ VFShader11.program,"transform");

    glUniformMatrix4fv(/* CHANGE */ transform_location18,1,GL_FALSE,glm::value_ptr(/* CHANGE  */trans18));
    //TRANSFORMATION PRIMER2

/*
=======
>>>>>>> ec5c0f0cbdf5aa1c4a9ae258e148848cc8849274
*/  glBindVertexArray(VAO[15]);
    //glDrawArrays(GL_TRIANGLES,0,4);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    //glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    //END DRAW CREASE 44

   //DRAW TRIANGLE 1
    
   //SHADER ACTIVE
    VFShader4.Use();

    GLuint view_location=glGetUniformLocation(VFShader4.program,"view");
    glUniformMatrix4fv(view_location,1,GL_FALSE,glm::value_ptr(view));

    //TRANSFORMATION PRIMER1    
    glm::mat4 trans(1.0f);
    trans=translate(trans,glm::vec3(0.0f,-0.0f,0.0f));
    trans=rotate(trans,glm::radians((GLfloat)glfwGetTime()*RotationSpeed),glm::vec3(0.0f,0.0f,RotationDirection));
   
    GLuint transform_location=glGetUniformLocation(VFShader.program,"transform");
    glUniformMatrix4fv(transform_location,1,GL_FALSE,glm::value_ptr(trans));
    //TRANSFORMATION PRIMER1

    glBindVertexArray(VAO[3]);    
    //glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glDrawArrays(GL_TRIANGLES,0,12);
    //glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    // END DRAW TRIANGLE 1

    //DRAW Triangle 3
    VFShader2.Use();

    //TRANSFORMATION PRIMER2    
    glm::mat4 /* CHANGE */ trans2(1.0f);
    /* CHANGE */trans2=translate(/* CHANGE */ trans2,glm::vec3(0.0f,-0.0f,0.0f));
    /* CHANGE */trans2=rotate(/* CHANGE */ trans2,glm::radians((GLfloat)glfwGetTime()*RotationSpeed),glm::vec3(0.0f,0.0f,RotationDirection));

    GLuint /* CHANGE */transform_location2=glGetUniformLocation(/* CHANGE */ VFShader2.program,"transform");
    
    glUniformMatrix4fv(/* CHANGE */ transform_location2,1,GL_FALSE,glm::value_ptr(/* CHANGE  */trans2));
    //TRANSFORMATION PRIMER2

/*
=======
>>>>>>> ec5c0f0cbdf5aa1c4a9ae258e148848cc8849274
*/  glBindVertexArray(VAO[1]);
    //glDrawArrays(GL_TRIANGLES,0,4);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    //glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    // END DRAW TRIANGLE 3


    // DRAW TRIANGLE 4
    VFShader.Use();
    //DRAW3
    
    //TRANSFORMATION PRIMER3
    glm::mat4 /* CHANGE */ trans3(1.0f);
    /* CHANGE */trans3=translate(/* CHANGE */ trans3,glm::vec3(0.0f,-0.0f,0.0f));
    /* CHANGE */trans3=rotate(/* CHANGE */ trans3,glm::radians((GLfloat)glfwGetTime()*RotationSpeed),glm::vec3(0.0f,0.0f,RotationDirection));

    GLuint /* CHANGE */transform_location3=glGetUniformLocation(/* CHANGE */ VFShader3.program,"transform");

    glUniformMatrix4fv(/* CHANGE */ transform_location3,1,GL_FALSE,glm::value_ptr(/* CHANGE  */trans3));
    //TRANSFORMATION PRIMER3

    glBindVertexArray(VAO[0]);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    // ENd DRAW TRIANGLE 4


    //DRAW TRIANGLE 2
    VFShader.Use();

    glm::mat4 /* CHANGE */ trans12(1.0f);
    /* CHANGE */trans12=translate(/* CHANGE */ trans12,glm::vec3(0.0f,-0.0f,0.0f));
    /* CHANGE */trans12=rotate(/* CHANGE */ trans12,glm::radians((GLfloat)glfwGetTime()*RotationSpeed),glm::vec3(0.0f,0.0f,RotationDirection));
    GLuint /* CHANGE */transform_location12=glGetUniformLocation(/* CHANGE */ VFShader10.program,"transform");
    glUniformMatrix4fv(/* CHANGE */ transform_location12,1,GL_FALSE,glm::value_ptr(/* CHANGE  */trans12));
    //TRANSFORMATION PRIMER2   
    glBindVertexArray(VAO[2]);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
    
    //END DRAW TRIANGLE 2 

    if(Front){

    //DRAW BASE 1
    VFShader6.Use();
    glBindVertexArray(VAO[5]);
    //TRANSFORMATION BASE 

    glm::mat4 /* CHANGE */ tb(1.0f);
    /* CHANGE */tb=translate(/* CHANGE */ tb,glm::vec3(0.0f,-0.0f,0.0f));

//    /* CHANGE */tb=rotate(/* CHANGE */ tb,glm::radians((GLfloat)glfwGetTime()*50.0f),glm::vec3(0.0f,0.0f,1.0f));

    GLuint /* CHANGE */tl=glGetUniformLocation(/* CHANGE */ VFShader6.program,"transform");

    glUniformMatrix4fv(/* CHANGE */ tl,1,GL_FALSE,glm::value_ptr(/* CHANGE  */tb));
    //TRANSFORMATION PRIMER BASE

    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    //Draw BASE 2
    VFShader9.Use();
    glBindVertexArray(VAO[7]);
    //TRANSFORMATION BASE 

    glm::mat4 /* CHANGE */ tb9(1.0f);
    /* CHANGE */tb9=translate(/* CHANGE */ tb9,glm::vec3(0.0f,-0.0f,0.0f));

//    /* CHANGE */tb=rotate(/* CHANGE */ tb,glm::radians((GLfloat)glfwGetTime()*50.0f),glm::vec3(0.0f,0.0f,1.0f));

    GLuint /* CHANGE */tl9=glGetUniformLocation(/* CHANGE */ VFShader9.program,"transform");

    glUniformMatrix4fv(/* CHANGE */ tl9,1,GL_FALSE,glm::value_ptr(/* CHANGE  */tb9));
    //TRANSFORMATION PRIMER BASE

    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    //END DRAW BASE

    }

    //DRAW CIRCLES
    
    VFShader8.Use();
    glBindVertexArray(VAO[6]);
    //TRANSFORMATION PRIMER2    

    glm::mat4 /* CHANGE */ tb3(1.0f);
    /* CHANGE */tb3=translate(/* CHANGE */ tb3,glm::vec3(0.0f,-0.0f,0.0f));
    /* CHANGE */tb3=rotate(/* CHANGE */ tb3,glm::radians((GLfloat)glfwGetTime()*RotationSpeed),glm::vec3(0.0f,0.0f,RotationDirection));

    GLuint /* CHANGE */tl3=glGetUniformLocation(/* CHANGE */ VFShader8.program,"transform");

    glUniformMatrix4fv(/* CHANGE */ tl3,1,GL_FALSE,glm::value_ptr(/* CHANGE  */tb3));
    //TRANSFORMATION PRIMER2

    glDrawArrays(GL_LINES,0,CV.size());
    glBindVertexArray(0);
    
    VFShader7.Use();
    glBindVertexArray(VAO[4]);
    //TRANSFORMATION PRIMER2    

    glm::mat4 /* CHANGE */ tb2(1.0f);
    /* CHANGE */tb2=translate(/* CHANGE */ tb2,glm::vec3(0.0f,-0.0f,0.0f));
    /* CHANGE */tb2=rotate(/* CHANGE */ tb2,glm::radians((GLfloat)glfwGetTime()*50.0f),glm::vec3(0.0f,0.0f,-1.0f));

    GLuint /* CHANGE */tl2=glGetUniformLocation(/* CHANGE */ VFShader7.program,"transform");

    glUniformMatrix4fv(/* CHANGE */ tl2,1,GL_FALSE,glm::value_ptr(/* CHANGE  */tb2));
    //TRANSFORMATION PRIMER2

    glDrawArrays(GL_LINES,0,CV.size());
    glBindVertexArray(0);
    
    //END DRAW CIRCLES

    glfwSwapBuffers(window);
   }

    // deallocate all resources    
    glDeleteVertexArrays(6,VAO);
    VBO.~glBuffer();
    VBO2.~glBuffer();
    VBO3.~glBuffer();
    VBO4.~glBuffer();
    VBO6.~glBuffer();
    VBO7.~glBuffer();
    VBO8.~glBuffer();
    VBO9.~glBuffer();
    VBO10.~glBuffer();
    VBO11.~glBuffer();
    VBO12.~glBuffer();
    VBO13.~glBuffer();
    VBO14.~glBuffer();
    VBO15.~glBuffer();
    VBO16.~glBuffer();
    VBO17.~glBuffer();
    CV.clear();
    CV2.clear();
   // terminate GLFW
    glfwTerminate();

    return 0;
}

