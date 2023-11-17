#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "header/Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "header/Camera.h"
typedef unsigned int uint;
const int WindWidth = 800;
const int WindHeight = 600;

Camera camera(glm::vec3(0,0,3.f),glm::vec3(0,1,0));
float deltaTime = 0;

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
  };

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
unsigned int indices[] = {
    // 注意索引从0开始! 
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形

    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};
void framebuffer_size_callback(GLFWwindow* wind,int width,int height)
{
    glViewport(0,0,width,height);
}

float mixValue = 0;
GLFWwindow* createGlfwWindow()
{
    if(!glfwInit()) return nullptr;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    return glfwCreateWindow(WindWidth,WindHeight,"opengl",nullptr,nullptr);
}

void registerCallback(GLFWwindow* wind)
{
    glfwSetFramebufferSizeCallback(wind,framebuffer_size_callback);
}

void processInput(GLFWwindow* wind)
{
    if(glfwGetKey(wind,GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(wind,true);
    }
    if(glfwGetKey(wind,GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.01f;
    }
    if(glfwGetKey(wind,GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.01f;
    }
    if(glfwGetKey(wind,GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD,deltaTime);
    }
    if(glfwGetKey(wind,GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD,deltaTime);
    }
    if(glfwGetKey(wind,GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT,deltaTime);
    }
    if(glfwGetKey(wind,GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT,deltaTime);
    }
    mixValue = std::max(0.f,std::min(1.f,mixValue));
}

bool firstMouse = true;
double lastX;
double lastY;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    camera.ProcessMouseMovement(xoffset,yoffset);
}
float fov = 45.f;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 45.0f)
        fov = 45.0f;
}

uint genTexture2D(const std::string filename,
    int wrap_s = GL_REPEAT,int wrap_t = GL_REPEAT,
    int texture_min_filter = GL_LINEAR,int texture_mag_filter = GL_LINEAR,
    int type = GL_UNSIGNED_BYTE,
    bool genMipmap = false)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_mag_filter);
    // 加载并生成纹理
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        auto format = nrChannels == 3? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);
        // 第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
        // 第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
        // 第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
        // 第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
        // 下个参数应该总是被设为0（历史遗留的问题）。
        // 第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
        // 最后一个参数是真正的图像数据。
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    if(genMipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    return texture;
}

void setTexture2D(int textureChannel,uint texture)
{
    glActiveTexture(textureChannel);
    glBindTexture(GL_TEXTURE_2D,texture);
}

glm::mat4 getModel()
{
    glm::mat4 model = glm::mat4(1.0f);
    return glm::rotate(model,(float)glfwGetTime() *glm::radians(50.0f),glm::vec3(1.,0.,0.));
}

glm::mat4 getView()
{
    float r = 10.f;
    float camX = sin(glfwGetTime())*r;
    float camZ = cos(glfwGetTime())*r;
    return glm::lookAt(glm::vec3(camX,0,camZ),glm::vec3(0,0,0),glm::vec3(0,1,0));
}

glm::mat4 getProjection()
{
    return glm::perspective(glm::radians(fov),(float)WindWidth/(float)WindHeight,0.1f,100.f);
}

int main(int argc, char* argv[])
{
    stbi_set_flip_vertically_on_load(true);
    GLFWwindow* wind = createGlfwWindow();
    if(!wind)
    {
        std::cout<< "创建glfw窗口失败"<<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(wind);
    
    glfwSetInputMode(wind, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(wind, mouse_callback);
    glfwSetScrollCallback(wind, scroll_callback);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<< "初始化glad失败"<<std::endl;
        return -1;
    }

    glViewport(10,10,WindWidth-20,WindHeight-20);


    
    uint VAO;
    glGenVertexArrays(1,&VAO);

    glBindVertexArray(VAO);
    
    uint VBO;
    glGenBuffers(1,&VBO); //创建缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER,VBO); //指定缓冲对象类型
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW); //填充缓冲内容

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);

    // glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    // glEnableVertexAttribArray(2);
    
    uint EBO ;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    glBindVertexArray(0);

    Shader shader("./Shader/shader1.vert","./Shader/shader1.frag");

    auto texture1 = genTexture2D("./Asset/Images/container.jpg");
    auto texture2 = genTexture2D("./Asset/Images/awesomeface.png",GL_REPEAT,GL_REPEAT,GL_NEAREST,GL_NEAREST);

    shader.use();
    glUniform1i(glGetUniformLocation(shader.ID, "_MainTex"), 0);
    glUniform1i(glGetUniformLocation(shader.ID, "_MainTex1"), 1);
    shader.setVec3("objectColor",glm::vec3(1,.5,.31));
    shader.setVec3("lightColor",glm::vec3(1,1,1));
    
    uint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, lightVAO);
    // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    while(!glfwWindowShouldClose(wind))
    {
        auto time = glfwGetTime();
        processInput(wind);
        camera.UpdateProjection(fov,0.1f,100.f,(float)WindWidth/WindHeight);
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

        setTexture2D(GL_TEXTURE0,texture1);
        setTexture2D(GL_TEXTURE1,texture2);
        shader.use();
        shader.setFloat("_MixValue",mixValue);
        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i; 
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("trans", camera.GetProjectionMatrix()*camera.GetViewMatrix()*model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        
        glfwSwapBuffers(wind);

        glfwPollEvents();
        deltaTime = glfwGetTime() - time;
    }
    glfwTerminate();
    return 0;
}
