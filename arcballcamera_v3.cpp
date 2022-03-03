// Arcball camera for a Rubik cube, controlled by muouse.
// The cube's pattern is not proper. It was built from only one surface pattern of a Rubik cube.
// Every surface of the cube in this program have same pattern.
// So, do not try to solve the cube!  
// Made by ouz81.
// ouz was here. ouz is still here. I'm sitting at home right now.

#include <stdio.h>
#include <iostream>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // necessary for upload texture image.
#include "shader.h" // shader class for shaders. Inspired from learnopengl.com. Nearly copied. Nearly. Not it all.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
GLFWwindow* window;


const unsigned int SCR_WIDTH = 800; // screen width
const unsigned int SCR_HEIGHT = 600; // screen height
const float RADIUS = 1.0f; //radius of the sphere
bool flag = false; // a signal for mouse click.

struct Quaternion{
    float cosine; //cosine of half the rotation angle
    glm::vec3 axis; //unit vector scaled by sine of half the angle

};

class ArcballCamera{
public:
    
    glm::vec3 position = glm::vec3(0.0f, 0.0f, -3.0f);
    glm::vec3 startPos;
    glm::vec3 currentPos = startPos;
    glm::vec3 startPosUnitVector;
    glm::vec3 currentPosUnitVector;

    Quaternion currentQuaternion;
    Quaternion lastQuaternion = {0.0f, glm::vec3(1.0f, 0.0f, 0.0f)};
    
    float cosValue, cosValue_2;
    float theta;
    float angle = 180.0f;
    glm::vec3 rotationalAxis = glm::vec3(1.0f, 0.0f, 0.0f);                       
    glm::vec3 rotationalAxis_2;
    ArcballCamera (){};
    float z_axis(float,float);
    glm::vec3 getUnitVector(glm::vec3);
    float dotProduct();
    void rotation();
    void replace();

    
};

ArcballCamera arcCamera;
void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos){
    if(flag == true){
        //Get the screen coordinates when mouse clicks.
        arcCamera.currentPos.x = ((xpos - (SCR_WIDTH/2) ) / (SCR_WIDTH/2)) * RADIUS;
        arcCamera.currentPos.y = (((SCR_HEIGHT/2) - ypos) / (SCR_HEIGHT/2)) * RADIUS;
        arcCamera.currentPos.z = arcCamera.z_axis(arcCamera.currentPos.x, arcCamera.currentPos.y);
        arcCamera.rotation();
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    //action == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        
        double startXPos, startYPos; //screen coordinates when mouse clicks.
        glfwGetCursorPos(window, &startXPos, &startYPos);
        arcCamera.startPos.x = ((startXPos - (SCR_WIDTH/2) ) / (SCR_WIDTH/2)) * RADIUS; //convert to NDC, then assign to startPos.
        arcCamera.startPos.y = (((SCR_HEIGHT/2) - startYPos) / (SCR_HEIGHT/2)) * RADIUS;// ..same for y coordinate.
        arcCamera.startPos.z = arcCamera.z_axis(arcCamera.startPos.x, arcCamera.startPos.y);
        flag = true;
    }
     else if(action == GLFW_RELEASE){
        arcCamera.replace();
        flag = false;

        }
}

float ArcballCamera::z_axis(float x, float y){
    float z = 0; 
    if(sqrt((x * x) + (y * y)) <= RADIUS) z = (float)sqrt((RADIUS * RADIUS) - (x * x) - (y * y)); 
    return z;
}

glm::vec3 ArcballCamera::getUnitVector(glm::vec3 vectr){
    float magnitude1;
    glm::vec3 unitVector; 
    magnitude1 = (vectr.x * vectr.x) + (vectr.y * vectr.y) + (vectr.z * vectr.z); 
    magnitude1 = sqrt(magnitude1);
    if(magnitude1 == 0){
        unitVector.x = 0;
        unitVector.y = 0;
        unitVector.z = 0;
            }
    else {
    unitVector.x = vectr.x / magnitude1; //if magnitude 0, then this func give 'nan' error.
    unitVector.y = vectr.y / magnitude1;
    unitVector.z = vectr.z / magnitude1;
}
    return unitVector;    
}

float ArcballCamera::dotProduct(){
    float result = (startPosUnitVector.x * currentPosUnitVector.x) + (startPosUnitVector.y * currentPosUnitVector.y) + (startPosUnitVector.z * currentPosUnitVector.z);
    return result;
}

void ArcballCamera::rotation(){
    startPosUnitVector = getUnitVector(startPos);
    currentPosUnitVector = getUnitVector(currentPos);
    currentQuaternion.axis = glm::cross(startPos, currentPos);
    currentQuaternion.axis = getUnitVector(currentQuaternion.axis);
    
    cosValue = dotProduct(); //q0 is cosine of the angle here.
    if(cosValue > 1) cosValue = 1; // when dot product gives '1' as result, it doesn't equal to 1 actually. It equals to just like 1.00000000001 . 
    theta = (acos(cosValue) * 180 / 3.1416); //theta is the angle now.
    currentQuaternion.cosine = cos((theta / 2) * 3.1416 / 180); //currentQuaternion.cosine is cos of half the angle now.

    currentQuaternion.axis.x = currentQuaternion.axis.x * sin((theta / 2) * 3.1416 / 180);
    currentQuaternion.axis.y = currentQuaternion.axis.y * sin((theta / 2) * 3.1416 / 180);
    currentQuaternion.axis.z = currentQuaternion.axis.z * sin((theta / 2) * 3.1416 / 180);
    
    cosValue_2 = (currentQuaternion.cosine * lastQuaternion.cosine)
                         - glm::dot(currentQuaternion.axis, lastQuaternion.axis);
    
    
    glm::vec3 temporaryVector;

    temporaryVector = glm::cross(currentQuaternion.axis, lastQuaternion.axis);
    

    rotationalAxis_2.x = (currentQuaternion.cosine * lastQuaternion.axis.x) + 
                            (lastQuaternion.cosine * currentQuaternion.axis.x ) +
                            temporaryVector.x;

    rotationalAxis_2.y = (currentQuaternion.cosine * lastQuaternion.axis.y) + 
                            (lastQuaternion.cosine * currentQuaternion.axis.y ) +
                            temporaryVector.y;

    rotationalAxis_2.z = (currentQuaternion.cosine * lastQuaternion.axis.z) + 
                            (lastQuaternion.cosine * currentQuaternion.axis.z ) +
                            temporaryVector.z;
    
    angle = (acos(cosValue_2) * 180 / 3.1416) * 2;

    rotationalAxis.x = rotationalAxis_2.x / sin((angle / 2) * 3.1416 / 180);
    rotationalAxis.y = rotationalAxis_2.y / sin((angle / 2) * 3.1416 / 180);
    rotationalAxis.z = rotationalAxis_2.z / sin((angle / 2) * 3.1416 / 180);
}

void ArcballCamera::replace(){
    lastQuaternion.cosine = cosValue_2;
    lastQuaternion.axis = rotationalAxis_2;
}

int main(){

	/*******GLFW and GLEW INITIALIZATION******/
	if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( SCR_WIDTH, SCR_HEIGHT, "ARCBALL CAMERA", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    //enable depth test
    glEnable(GL_DEPTH_TEST);

    //Shaders
    Shader cubeShader("rubik.vs", "rubik.fs");
    unsigned int cubeProgram = cubeShader.programID();
    

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(cubeVAO);

    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    //loading texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("rubik.jpg", &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else std::cout<<"NO IMAGE"<<std::endl;
    stbi_image_free(data);
    
    do{ 
    	glClearColor(1.0f, 0.8f, 0.1f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(cubeProgram);        
    	glBindTexture(GL_TEXTURE_2D, texture);
    	glUniform1i(glGetUniformLocation(cubeProgram, "rubiktexture"), 0);

        glm::mat4 model = glm::mat4(1.0f);
        
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::mat4(1.0f);// this command must be in the loop. Otherwise, the object moves if there is a glm::rotate func in the lop.    
        view = glm::translate(view, arcCamera.position);// this, too.  
        view = glm::rotate(view, glm::radians(arcCamera.angle), arcCamera.rotationalAxis);

        glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "view"),  1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "model"),  1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    }
