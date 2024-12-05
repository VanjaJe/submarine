#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define CRES 30
#define TRAIL_LENGTH 2000  
#define CRES2 100  

float points[CRES2 * 2];

float trail[TRAIL_LENGTH * 4] = {0};
int trailIndex = 0; 

bool isSolarTurnedOn = true;


unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath);
void showNumber(int number, int* digits);
void loadTexture(unsigned* textures, int num, unsigned int shader);
unsigned loadWarningTexture(unsigned digitShader, bool warning);
unsigned loadNameTexture(unsigned digitShader);
void updateHand(float angle, float* hand, float r);
unsigned loadButtonTexture(unsigned digitShader);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
bool isMouseOverRectangle(float mouseX, float mouseY, float left, float right, float bottom, float top);
unsigned loadMetalTexture(unsigned digitShader);


int main(void)
{

    if (!glfwInit())
    {
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    unsigned int wWidth = 900;
    unsigned int wHeight = 471;
    const char wTitle[] = "[Generic Title]";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    unsigned int unifiedShader = createShader("basic.vert", "basic.frag");
    unsigned int sonarShader = createShader("sonar.vert", "sonar.frag");
    unsigned int digitShader = createShader("digit.vert", "digit.frag");
    unsigned int warningShader = createShader("warning.vert", "warning.frag");
    unsigned int lightShader = createShader("light.vert", "light.frag");
    unsigned int pointerShader = createShader("pointer.vert", "pointer.frag");


    float vertices[] =
    {
        -0.65, 0.15,     0.0, 0.0, 0.0,
        0.65, 0.15,      0.0, 0.0, 0.0, 
        -0.65, 0.45,     0.0, 0.0, 0.0,
        0.65, 0.45,      0.0, 0.0, 0.0, 

        -0.65, -0.05,    0.0, 0.0, 0.0, 
        0.65, -0.05,     0.0, 0.0, 0.0,
        -0.65, -0.35,    0.0, 0.0, 0.0, 
        0.65, -0.35,     0.0, 0.0, 0.0 
    };

    float dangerVertices[] =
    {
        -0.62, -0.07,  0.0, 0.0,
         0.60, -0.07,  1.0, 0.0,
        -0.62,  0.2,  0.0, 1.0,
         0.60,  0.20,  1.0, 1.0,
    };

    float digitsVertices1[] = {
        // Prvi kvadrat (cifra stotina)
        -0.35, -0.5,   0.0, 1.0, 
        -0.25, -0.5,   1.0, 1.0,  
        -0.35, -0.6,   0.0, 0.0, 
        -0.25, -0.6,   1.0, 0.0, 

        // Drugi kvadrat (cifra desetica)
        -0.20, -0.5,   0.0, 1.0,
        -0.10, -0.5,   1.0, 1.0,
        -0.20, -0.6,   0.0, 0.0,
        -0.10, -0.6,   1.0, 0.0,

        // Treci kvadrat (cifra jedinica)
        -0.05, -0.5,   0.0, 1.0,
        0.05,  -0.5,   1.0, 1.0,
        -0.05, -0.6,   0.0, 0.0,
        0.05,  -0.6,   1.0, 0.0,
    };

    float digitsVertices2[] = {
        -0.35, 0.65,   0.0, 1.0,
        -0.25, 0.65,   1.0, 1.0,
        -0.35, 0.55,   0.0, 0.0,
        -0.25, 0.55,   1.0, 0.0, 

        -0.20, 0.65,   0.0, 1.0, 
        -0.10, 0.65,   1.0, 1.0,
        -0.20, 0.55,   0.0, 0.0, 
        -0.10, 0.55,   1.0, 0.0,

        -0.05, 0.65,   0.0, 1.0, 
        0.05,  0.65,   1.0, 1.0,
        -0.05, 0.55,   0.0, 0.0,  
        0.05,  0.55,   1.0, 0.0,
    };

    float progressVertices[] =
    {
        -0.65,  0.15,     0.0, 1.0, 0.0, 
        -0.65,  0.45,     0.0, 1.0, 0.0, 
        -0.65, 0.15,      0.0, 1.0, 0.0, 
        -0.65, 0.45,      0.0, 1.0, 0.0,

        -0.65, -0.05,     0.0, 1.0, 0.0,
        -0.65, -0.35,     0.0, 1.0, 0.0, 
        -0.65, -0.05,     0.0, 1.0, 0.0,
        -0.65, -0.35,     0.0, 1.0, 0.0
    };

    float nameVertices[] =
    {
        -1.0,  -1.0,  0.0,  0.0,  
        -0.07,  -1.0,  1.0,  0.0, 
        -1.0,  -0.85,  0.0,  1.0,
        -0.07,  -0.85,  1.0,  1.0 
    };

    float buttonVertices[] =
    {
        0.58, 0.70,  0.0, 0.0,
        0.75, 0.70,  1.0, 0.0,
        0.58, 0.85,  0.0, 1.0,
        0.75, 0.85,  1.0, 1.0,
    };

    float backgroundVertices[] = {
        -1.0f, -1.0f,    0.0f, 0.0f, // Levo dole
         1.0f, -1.0f,    1.0f, 0.0f, // Desno dole
        -1.0f,  1.0f,    0.0f, 1.0f, // Levo gore
         1.0f,  1.0f,    1.0f, 1.0f  // Desno gore
    };


    unsigned textures[10];

    for (int i = 0; i < 10; i++) {
        loadTexture(textures, i, digitShader);
    }

    unsigned int VAO[2];
    glGenVertexArrays(2, VAO);
    unsigned int VBO[2];
    glGenBuffers(2, VBO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int progressVAO;
    glGenVertexArrays(1, &progressVAO);
    unsigned int progressVBO;
    glGenBuffers(1, &progressVBO);

    glBindVertexArray(progressVAO);
    glBindBuffer(GL_ARRAY_BUFFER, progressVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int dangerVAO;
    glGenVertexArrays(1, &dangerVAO);
    unsigned int dangerVBO;
    glGenBuffers(1, &dangerVBO);

    glBindVertexArray(dangerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, dangerVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dangerVertices), dangerVertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int buttonVAO;
    glGenVertexArrays(1, &buttonVAO);
    unsigned int buttonVBO;
    glGenBuffers(1, &buttonVBO);

    glBindVertexArray(buttonVAO);
    glBindBuffer(GL_ARRAY_BUFFER, buttonVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buttonVertices), buttonVertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned buttonTexture = loadButtonTexture(digitShader);

    unsigned int backgroundVAO;
    glGenVertexArrays(1, &backgroundVAO);
    unsigned int backgroundVBO;
    glGenBuffers(1, &backgroundVBO);

    glBindVertexArray(backgroundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVertices), backgroundVertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned backgroundTexture = loadMetalTexture(digitShader);


    unsigned warningTexture = loadWarningTexture(digitShader, true);
    unsigned notWarningTexture = loadWarningTexture(digitShader, false);

    unsigned int digitsVAO[2], digitsVBO[2];
    glGenVertexArrays(2, digitsVAO);
    glGenBuffers(2, digitsVBO);

    glBindVertexArray(digitsVAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, digitsVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(digitsVertices1), digitsVertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(digitsVAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, digitsVBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(digitsVertices2), digitsVertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int nameVAO;
    glGenVertexArrays(1, &nameVAO);
    unsigned int nameVBO;
    glGenBuffers(1, &nameVBO);

    glBindVertexArray(nameVAO);
    glBindBuffer(GL_ARRAY_BUFFER, nameVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nameVertices), nameVertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned nameTexture = loadNameTexture(digitShader);


    float circle[(CRES + 2) * 2];
    float r = 0.8;

    circle[0] = 0;
    circle[1] = 0;
    int i;
    for (i = 0; i <= CRES; i++)
    {
        circle[2 + 2 * i] = r * cos((3.141592 / 180) * (i * 360 / CRES));
        circle[2 + 2 * i + 1] = r * sin((3.141592 / 180) * (i * 360 / CRES));
    }

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float pointer[4]; 
    float angle = 45; 
    pointer[0] = 0.0f; 
    pointer[1] = 0.0f;
    pointer[2] = r * cos((3.141592 / 180) * angle);
    pointer[3] = r * sin((3.141592 / 180) * angle);

    unsigned int pointerVAO, pointerVBO;
    glGenVertexArrays(1, &pointerVAO);
    glGenBuffers(1, &pointerVBO);

    glBindVertexArray(pointerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pointerVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointer), pointer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    float light[(CRES + 2) * 2];
    float r2 = 0.06;

    float dx = -0.8f; 
    float dy = 0.3f;

    light[0] = dx; 
    light[1] = dy; 

    for (int i = 0; i <= CRES; i++) {
        light[2 + 2 * i] = dx + r2 * cos((3.141592 / 180) * (i * 360 / CRES)); 
        light[2 + 2 * i + 1] = dy + r2 * sin((3.141592 / 180) * (i * 360 / CRES)); 
    }

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    unsigned int lightVBO;
    glGenBuffers(1, &lightVBO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(light), light, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float progress1 = 1.0f;
    float progress2 = 0.0f;

    float time = 0.0f;
    int digits[4] = {};
    int digits2[4] = {};
    float lastTime = 0.0f;
    float deltaTime = 0.0f;
    float timeAccumulator = 0.0f;

    unsigned int trailVAO, trailVBO;
    glGenVertexArrays(1, &trailVAO);
    glGenBuffers(1, &trailVBO);

    glBindVertexArray(trailVAO);
    glBindBuffer(GL_ARRAY_BUFFER, trailVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trail), trail, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    for (int i = 0; i < 10; i++) {
        float angle = ((float)rand() / RAND_MAX) * 2 * 3.14159;
        float radius = sqrt(((float)rand() / RAND_MAX) * r * r);

        points[i * 2] = radius * cos(angle);   
        points[i * 2 + 1] = radius * sin(angle);
    }


    unsigned int pointsVAO, pointsVBO;
    glGenVertexArrays(1, &pointsVAO);
    glGenBuffers(1, &pointsVBO);

    glBindVertexArray(pointsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
   
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    const double target_time = 1.0 / 60.0;
    double last_frame_time = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double current_frame_time = glfwGetTime();
        double delta_time = current_frame_time - last_frame_time;

        if (delta_time >= target_time) {


            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, GL_TRUE);
            }

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            {
                progress2 -= 0.0009f;
                if (progress2 < 0.0f) progress2 = 0.0f;

                showNumber(std::round(progress2 * 1000 / 4), digits);
            }

            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            {
                progress2 += 0.0009f;
                if (progress2 > 1.0f) progress2 = 1.0f;

                showNumber(std::round(progress2 * 1000 / 4), digits);
            }

            glClearColor(0.4706, 0.4667, 0.4667, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);


            glViewport(0, 0, wWidth / 2, wHeight);

            glBindVertexArray(backgroundVAO);
            glUseProgram(digitShader);

            glBindTexture(GL_TEXTURE_2D, backgroundTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glUseProgram(unifiedShader);

            glBindVertexArray(VAO[0]);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);


            if (progress2 > 0.0) {
                progress1 -= 0.0001f;
            }

            if (progress2 == 0.0) {
                progress1 += 0.0001f;
            }

            if (progress1 > 1.0f) progress1 = 1.0f;
            if (progress1 < 0.0f) progress1 = 0.0f;

            //std::cout << progress1 << "  ";
            showNumber(std::round(progress1 * 100), digits2);

            progressVertices[10] = -0.65 + 1.3f * progress1;
            progressVertices[15] = -0.65 + 1.3f * progress1;

            progressVertices[30] = -0.65 + 1.3f * progress2;
            progressVertices[35] = -0.65 + 1.3f * progress2;

            glBindVertexArray(progressVAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // Prvi progress bar
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 4); // Drugi progress bar
            glBindVertexArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, progressVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(progressVertices), progressVertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);


            glBindVertexArray(lightVAO);
            glUseProgram(lightShader);
            unsigned int color = glGetUniformLocation(lightShader, "color");
            //glBindVertexArray(0);

            //glUseProgram(lightShader);

            //int circleCenterLocation = glGetUniformLocation(lightShader, "circleCenter");
            //int circleRadiusLocation = glGetUniformLocation(lightShader, "circleRadius");
            //int circleColorLocation = glGetUniformLocation(lightShader, "circleColor");
            //int backgroundColorLocation = glGetUniformLocation(lightShader, "backgroundColor");

            //glUniform2f(circleCenterLocation, 0.4f, 0.5f);
            //glUniform1f(circleRadiusLocation, 0.8f);             
            //glUniform4f(circleColorLocation, 1.0f, 0.0f, 0.0f, 1.0f); 
            //glUniform4f(backgroundColorLocation, 0.4706, 0.4667, 0.4667, 1.0);

            glUniform4f(color, 0.0f, 0.0f, 0.0f, 1.0f);
            glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(light) / (2 * sizeof(float)));


            if (progress1 * 1000 > 750) {
                glBindVertexArray(dangerVAO);

                glUseProgram(warningShader);

                GLuint timeLocation = glGetUniformLocation(warningShader, "time");
                unsigned int color = glGetUniformLocation(warningShader, "color");

                glUniform3f(color, 0.0f, 1.0f, 0.0f);

                float timeValue = glfwGetTime();

                glUniform1f(timeLocation, timeValue);
                glBindTexture(GL_TEXTURE_2D, notWarningTexture);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glBindVertexArray(0);
            }

            if (progress1 * 1000 < 250) {
                glUniform4f(color, 1.0f, 0.0f, 0.0f, 1.0f);
                glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(light) / (2 * sizeof(float)));

                glBindVertexArray(dangerVAO);

                float timeValue = glfwGetTime();

                glUseProgram(warningShader);

                GLuint timeLocation = glGetUniformLocation(warningShader, "time");
                unsigned int color = glGetUniformLocation(warningShader, "color");

                glUniform1f(timeLocation, timeValue);
                glUniform3f(color, 2.0f, 0.0f, 0.0f);

                glBindTexture(GL_TEXTURE_2D, warningTexture);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glBindVertexArray(0);
            };


            glBindVertexArray(digitsVAO[0]);
            glUseProgram(digitShader);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


            // Prvi kvadrat (stotine)
            glBindTexture(GL_TEXTURE_2D, textures[digits[2]]);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            // Drugi 
            glBindTexture(GL_TEXTURE_2D, textures[digits[1]]);
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

            //Treci 
            //if (progress2 == 0.0f) {
            //    glBindTexture(GL_TEXTURE_2D, textures[0]);
            //    glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
            //}
            glBindTexture(GL_TEXTURE_2D, textures[digits[0]]);
            glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
            
            glBindVertexArray(digitsVAO[1]);
            glUseProgram(digitShader);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // Prvi kvadrat 
            glBindTexture(GL_TEXTURE_2D, textures[digits2[2]]);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            //Drugi 
            glBindTexture(GL_TEXTURE_2D, textures[digits2[1]]);
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

            //Treci
            glBindTexture(GL_TEXTURE_2D, textures[digits2[0]]);
            glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
            glBindVertexArray(0);


            glBindVertexArray(nameVAO);
            glUseProgram(digitShader);

            glBindTexture(GL_TEXTURE_2D, nameTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


            glViewport(wWidth / 2, 0, wWidth / 2, wHeight);

            glBindVertexArray(backgroundVAO);
            glUseProgram(digitShader);

            glBindTexture(GL_TEXTURE_2D, backgroundTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            time += 0.009f;

            glBindVertexArray(VAO[1]);
            glUseProgram(sonarShader);

            glUniform1f(glGetUniformLocation(sonarShader, "time"), time);
            glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle) / (2 * sizeof(float)));
            glBindVertexArray(0);

            glBindVertexArray(buttonVAO);
            glUseProgram(digitShader);

            glBindTexture(GL_TEXTURE_2D, buttonTexture);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


            if (isSolarTurnedOn) {
                bool showPoints = (int(std::round(time) * 10) % 15 == 0);

                glUseProgram(lightShader);
                unsigned int color = glGetUniformLocation(lightShader, "color");
                glUniform4f(color, 0.80f, 0.0f, 0.0f, 1.0f);


                if (showPoints) {
                    glPointSize(7.0f);
                    glBindVertexArray(pointsVAO);
                    glDrawArrays(GL_POINTS, 0, CRES);
                }
                float time = glfwGetTime();

                angle = fmod(time * 4, 360.0f);

                pointer[0] = 0.0f;
                pointer[1] = 0.0f;
                pointer[2] = r * cos((3.141592 / 180) * angle);
                pointer[3] = r * sin((3.141592 / 180) * angle);

                trail[trailIndex * 4] = 0.0f;
                trail[trailIndex * 4 + 1] = 0.0f;
                trail[trailIndex * 4 + 2] = pointer[2];
                trail[trailIndex * 4 + 3] = pointer[3];

                trailIndex = (trailIndex + 1) % TRAIL_LENGTH;

                //glUseProgram(lightShader);
                //unsigned int color = glGetUniformLocation(lightShader, "color");
                //glUniform4f(color, 0.60f, 0.0f, 0.0f, 0.20f);

                glUseProgram(pointerShader);


                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                glBindBuffer(GL_ARRAY_BUFFER, trailVBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, TRAIL_LENGTH * 4 * sizeof(float), trail);

                glBindVertexArray(trailVAO);

                glLineWidth(2.0f);
                glDrawArrays(GL_LINES, 0, trailIndex * 2);


                glBindVertexArray(pointerVAO);

                glBindBuffer(GL_ARRAY_BUFFER, pointerVBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointer), pointer);

                glLineWidth(4.0f);
                glDrawArrays(GL_LINES, 0, 2);
            }

            glBindVertexArray(0);
            glUseProgram(0);

            glfwSwapBuffers(window);
            glfwPollEvents();

            last_frame_time = current_frame_time;
        }
    }

    glDeleteTextures(1, &warningShader);
    glDeleteBuffers(1, &dangerVBO);
    glDeleteBuffers(1, &lightVBO);
    glDeleteBuffers(1, &pointsVBO);
    glDeleteBuffers(2, digitsVBO);
    glDeleteVertexArrays(2, digitsVAO);
    glDeleteBuffers(2, VBO);
    glDeleteVertexArrays(2, VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteVertexArrays(1, &pointsVAO);
    glDeleteVertexArrays(1, &dangerVAO);
    glDeleteProgram(pointerShader);
    glDeleteProgram(lightShader);
    glDeleteProgram(sonarShader);
    glDeleteProgram(unifiedShader);

    glfwTerminate();
    return 0;
}


bool isMouseOverRectangle(float x, float y, float left, float right, float bottom, float top) {
    return x >= left && x <= right && y >= bottom && y <= top;
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double mouseX, mouseY;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &mouseX, &mouseY);
        float left = 0.58f;
        float right = 0.75f;
        float bottom = 0.70f;
        float top = 0.85f;

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        double adjustedMouseX = mouseX - windowWidth / 2;
        float normalizedX = (2.0f * adjustedMouseX / (windowWidth / 2)) - 1.0f;
        float normalizedY = -1.0f * (2.0f * mouseY / windowHeight - 1.0f);

        if (isMouseOverRectangle(normalizedX, normalizedY, left, right, bottom, top)) {
            isSolarTurnedOn = !isSolarTurnedOn;
            trail[TRAIL_LENGTH * 4] = { 0 };
        }
    }
}

void updateHand(float angle, float* hand, float r)
{
    // Pretvaranje ugla u radijane
    float radian = angle * (3.141592 / 180);

    // Koordinate kraja kazaljke
    hand[2] = r * cos(radian); // x koordinata
    hand[3] = r * sin(radian); // y koordinata
}

void showNumber(int number, int* digits) {
    if (number >= 10 and number < 100) {
        digits[2] = 0;
    }
    else {
        digits[2] = 0;
        digits[1] = 0;
    }
    if (number == 0) {
        digits[0] = 0;
    }
    else {
        //int digits[10];
        int count = 0;
        while (number > 0) {
            digits[count++] = number % 10;
            number /= 10;
        }
    }
}


unsigned loadWarningTexture(unsigned digitShader, bool warning) {
    std::string texturePath;

    if (warning) {
        texturePath = "res/nedovoljan_nivo.png";
    }
    else {
        texturePath = "res/dovoljan_nivo.png";
    }

    unsigned warningTexture = loadImageToTexture(texturePath.c_str());
    glBindTexture(GL_TEXTURE_2D, warningTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);

    return warningTexture;
}

unsigned loadButtonTexture(unsigned digitShader) {
    std::string texturePath = "res/button.png";
    unsigned buttonTexture = loadImageToTexture(texturePath.c_str());
    glBindTexture(GL_TEXTURE_2D, buttonTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);

    return buttonTexture;
}

unsigned loadMetalTexture(unsigned digitShader) {
    std::string texturePath = "res/background5.png";
    unsigned metalTexture = loadImageToTexture(texturePath.c_str());
    glBindTexture(GL_TEXTURE_2D, metalTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    unsigned uTexLoc = glGetUniformLocation(digitShader, "uTex");
    glUniform1i(uTexLoc, 0);
    glUseProgram(0);
    return metalTexture;
}

unsigned loadNameTexture(unsigned digitShader) {
    std::string texturePath = "res/name.jpg";
    unsigned nameTexture = loadImageToTexture(texturePath.c_str());
    glBindTexture(GL_TEXTURE_2D, nameTexture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(digitShader);
    unsigned uTexLoc = glGetUniformLocation(digitShader, "uTex");
    glUniform1i(uTexLoc, 0);
    glUseProgram(0);

    return nameTexture;
}


void loadTexture(unsigned* textures, int num, unsigned int shader) {
    std::string texturePath = "res/pic_" + std::to_string(num) + ".png";
    unsigned checkerTexture = loadImageToTexture(texturePath.c_str()); //Ucitavamo teksturu
    glBindTexture(GL_TEXTURE_2D, checkerTexture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(shader);
    unsigned uTexLoc = glGetUniformLocation(shader, "uTex");
    glUniform1i(uTexLoc, 0); // Indeks teksturne jedinice (sa koje teksture ce se citati boje)
    glUseProgram(0);
    textures[num] = checkerTexture;
}


unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
    std::string temp = ss.str();
    const char* sourceCode = temp.c_str();

    int shader = glCreateShader(type);

    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}

unsigned int createShader(const char* vsSource, const char* fsSource)
{

    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    program = glCreateProgram();

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);


    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}


static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 2: InternalFormat = GL_RG; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}

