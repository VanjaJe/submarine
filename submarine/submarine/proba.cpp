//#define _CRT_SECURE_NO_WARNINGS
//
//#include <iostream>
//#include <fstream>
//#include <sstream>
//
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//#define CRES 30 // Circle Resolution = Rezolucija kruga
//
//
//unsigned int compileShader(GLenum type, const char* source);
//unsigned int createShader(const char* vsSource, const char* fsSource);
//static unsigned loadImageToTexture(const char* filePath);
//
//int main(void)
//{
//
//    if (!glfwInit())
//    {
//        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
//        return 1;
//    }
//
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow* window;
//    unsigned int wWidth = 900;
//    unsigned int wHeight = 470;
//    const char wTitle[] = "[Generic Title]";
//    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
//
//    if (window == NULL)
//    {
//        std::cout << "Prozor nije napravljen! :(\n";
//        glfwTerminate();
//        return 2;
//    }
//
//    glfwMakeContextCurrent(window);
//
//
//    if (glewInit() != GLEW_OK)
//    {
//        std::cout << "GLEW nije mogao da se ucita! :'(\n";
//        return 3;
//    }
//
//    unsigned int unifiedShader = createShader("basic.vert", "basic.frag");
//    unsigned int sonarShader = createShader("sonar.vert", "sonar.frag");
//
//
//    float vertices[] =
//    {
//        -0.65, 0.15,     0.0, 0.0, 0.0, // Donja leva
//        0.65, 0.15,      0.0, 0.0, 0.0, // Donja desna
//        -0.65, 0.45,     0.0, 0.0, 0.0, // Gornja leva
//        0.65, 0.45,      0.0, 0.0, 0.0, // Gornja desna
//
//        -0.65, -0.05,    0.0, 0.0, 0.0, // Donja leva
//        0.65, -0.05,     0.0, 0.0, 0.0, // Donja desna
//        -0.65, -0.35,    0.0, 0.0, 0.0, // Gornja leva
//        0.65, -0.35,     0.0, 0.0, 0.0  // Gornja desna
//    };
//
//    float progressVertices[] =
//    {
//        // Prvi progress bar
//        -0.65,  0.15,     0.0, 1.0, 0.0, // Donja leva
//        -0.65,  0.45,     0.0, 1.0, 0.0, // Gornja leva
//        -0.65 + 0.0, 0.15, 0.0, 1.0, 0.0, // Dinamicka donja desna
//        -0.65 + 0.0, 0.45, 0.0, 1.0, 0.0, // Dinamicka gornja desna
//
//        // Drugi progress bar
//        -0.65, -0.05,     0.0, 1.0, 0.0, // Donja leva
//        -0.65, -0.35,     0.0, 1.0, 0.0, // Gornja leva
//        -0.65 + 0.0, -0.05, 0.0, 1.0, 0.0, // Dinamicka donja desna
//        -0.65 + 0.0, -0.35, 0.0, 1.0, 0.0  // Dinamicka gornja desna
//    };
//
//    unsigned int VAO[2];
//    glGenVertexArrays(2, VAO);
//    unsigned int VBO[2];
//    glGenBuffers(2, VBO);
//
//    glBindVertexArray(VAO[0]);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//
//    unsigned int progressVAO;
//    glGenVertexArrays(1, &progressVAO);
//    unsigned int progressVBO;
//    glGenBuffers(1, &progressVBO);
//
//    glBindVertexArray(progressVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, progressVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
//
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//
//    //float vertices[] =
//    //{
//    //    0.25, 0.0,   1.0, 1.0, 1.0, 1.0,
//    //    -0.25, 0.0,  1.0, 1.0, 1.0, 1.0,
//    //    -0.25, 0.25, 1.0, 1.0, 1.0, 1.0,
//    //    0.25, 0.25,  1.0, 1.0, 1.0, 1.0,
//    //};
//
//    //unsigned int indices[] = {
//    //    0, 1, 3, // Prvi trougao (gornji desni, donji desni, gornji levi)
//    //    1, 2, 3  // Drugi trougao (donji desni, donji levi, gornji levi)
//    //};
//
//    //float vertices2[] = {
//    //    0.75, 0.5,   0.0, 1.0, 0.0, 1.0, // Zeleni kvadrat
//    //    0.25, 0.5,   0.0, 1.0, 0.0, 1.0,
//    //    0.25, 0.75,  0.0, 1.0, 0.0, 1.0,
//    //    0.75, 0.75,  0.0, 1.0, 0.0, 1.0,
//    //};
//
//    //unsigned int indices2[] = {
//    //    0, 1, 3,
//    //    1, 2, 3
//    //};
//
//
//    /*unsigned int VAO, VBO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//
//    glGenBuffers(1, &VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    glGenBuffers(1, &EBO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//    unsigned int stride = (2 + 4) * sizeof(float);
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    unsigned int VAO2, VBO2, EBO2;
//    glGenVertexArrays(1, &VAO2);
//    glBindVertexArray(VAO2);
//
//    glGenBuffers(1, &VBO2);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
//
//    glGenBuffers(1, &EBO2);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    glBindVertexArray(0);*/
//
//
//    float circle[(CRES + 2) * 2];
//    float r = 0.8;
//
//    circle[0] = 0;
//    circle[1] = 0;
//    int i;
//    for (i = 0; i <= CRES; i++)
//    {
//        circle[2 + 2 * i] = r * cos((3.141592 / 180) * (i * 360 / CRES));
//        circle[2 + 2 * i + 1] = r * sin((3.141592 / 180) * (i * 360 / CRES));
//    }
//
//    /*unsigned int VAO3, VBO3;
//    glGenVertexArrays(1, &VAO3);
//    glBindVertexArray(VAO3);
//    glGenBuffers(1, &VBO3);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);*/
//
//    glBindVertexArray(VAO[1]);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    float angle = 0.0f; // Pocetni ugao kazaljke
//    float angleStep = 1.0f;
//
//    unsigned int VAO4, VBO4;
//    float needle[4]; // Kazaljka ima dva vrha (pocetak i kraj)
//
//    // Inicijalizacija kazaljke
//    glGenVertexArrays(1, &VAO4);
//    glBindVertexArray(VAO4);
//
//    glGenBuffers(1, &VBO4);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO4);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(needle), needle, GL_DYNAMIC_DRAW);
//
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    float progress1 = 0.5f;
//    float progress2 = 0.75f;
//
//    while (!glfwWindowShouldClose(window))
//    {
//
//        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        {
//            glfwSetWindowShouldClose(window, GL_TRUE);
//        }
//
//        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        {
//            progress2 -= 0.0005f; // Drugi bar se smanjuje
//            //if (progress2 < 0.0f) progress2 = 1.0f;
//        }
//
//        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        {
//            progress2 += 0.0005f; // Drugi bar se smanjuje
//            //if (progress2 < 0.0f) progress2 = 1.0f;
//        }
//
//        glClearColor(0.4706, 0.4667, 0.4667, 1.0);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        glUseProgram(unifiedShader);
//
//        glViewport(0, 0, wWidth / 2, wHeight);
//
//        glBindVertexArray(VAO[0]);
//        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
//
//        progress1 += 0.00004f; // Primer animacije
//        if (progress1 > 1.0f) progress1 = 0.0f;
//
//
//
//        // Ažuriranje verteksa za progres bar
//        progressVertices[2 * 5 + 0] = -0.65 + 1.3f * progress1;
//        progressVertices[3 * 5 + 0] = -0.65 + 1.3f * progress1;
//
//        progressVertices[6 * 5 + 0] = -0.65 + 1.3f * progress2;
//        progressVertices[7 * 5 + 0] = -0.65 + 1.3f * progress2;
//
//        glBindBuffer(GL_ARRAY_BUFFER, progressVBO);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(progressVertices), progressVertices);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//
//        glBindVertexArray(progressVAO);
//        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // Prvi progress bar
//        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4); // Drugi progress bar
//        glBindVertexArray(0);
//
//        /*glViewport(0, 0, wWidth / 2, wHeight);
//        glBindVertexArray(VAO);
//
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
//        glBindVertexArray(VAO2);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
//
//        glViewport(wWidth / 2, 0, wWidth / 2, wHeight);
//        //glBindVertexArray(VAO3);
//        //glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle) / (2 * sizeof(float)));
//        glBindVertexArray(VAO[1]);
//        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle) / (2 * sizeof(float)));
//
//        //needle[0] = 0.0f; // Centar (X, Y)
//        //needle[1] = 0.0f;
//        //needle[2] = r * cos((3.141592 / 180.0) * angle); // Kraj kazaljke (X)
//        //needle[3] = r * sin((3.141592 / 180.0) * angle); // Kraj kazaljke (Y)
//
//        //glBindBuffer(GL_ARRAY_BUFFER, VBO4);
//        //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(needle), needle);
//
//        //glActiveTexture(GL_TEXTURE0);
//        //glBindTexture(GL_TEXTURE_2D, checkerTexture);
//
//        //glDrawArrays(GL_TRIANGLES, 0, 4);
//
//        //glBindTexture(GL_TEXTURE_2D, 0);
//        glBindVertexArray(0);
//        glUseProgram(0);
//
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//
//    //glDeleteTextures(1, &checkerTexture);
//    //glDeleteBuffers(1, &VBO);
//    //glDeleteVertexArrays(1, &VAO);
//
//    glDeleteBuffers(2, VBO);
//    glDeleteVertexArrays(2, VAO);
//    glDeleteProgram(unifiedShader);
//
//    glfwTerminate();
//    return 0;
//}
//
//unsigned int compileShader(GLenum type, const char* source)
//{
//    std::string content = "";
//    std::ifstream file(source);
//    std::stringstream ss;
//    if (file.is_open())
//    {
//        ss << file.rdbuf();
//        file.close();
//        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
//    }
//    else {
//        ss << "";
//        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
//    }
//    std::string temp = ss.str();
//    const char* sourceCode = temp.c_str();
//
//    int shader = glCreateShader(type);
//
//    int success;
//    char infoLog[512];
//    glShaderSource(shader, 1, &sourceCode, NULL);
//    glCompileShader(shader);
//
//    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//    if (success == GL_FALSE)
//    {
//        glGetShaderInfoLog(shader, 512, NULL, infoLog);
//        if (type == GL_VERTEX_SHADER)
//            printf("VERTEX");
//        else if (type == GL_FRAGMENT_SHADER)
//            printf("FRAGMENT");
//        printf(" sejder ima gresku! Greska: \n");
//        printf(infoLog);
//    }
//    return shader;
//}
//unsigned int createShader(const char* vsSource, const char* fsSource)
//{
//
//    unsigned int program;
//    unsigned int vertexShader;
//    unsigned int fragmentShader;
//
//    program = glCreateProgram();
//
//    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
//    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);
//
//
//    glAttachShader(program, vertexShader);
//    glAttachShader(program, fragmentShader);
//
//    glLinkProgram(program);
//    glValidateProgram(program);
//
//    int success;
//    char infoLog[512];
//    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
//    if (success == GL_FALSE)
//    {
//        glGetShaderInfoLog(program, 512, NULL, infoLog);
//        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
//        std::cout << infoLog << std::endl;
//    }
//
//    glDetachShader(program, vertexShader);
//    glDeleteShader(vertexShader);
//    glDetachShader(program, fragmentShader);
//    glDeleteShader(fragmentShader);
//
//    return program;
//}
//static unsigned loadImageToTexture(const char* filePath) {
//    int TextureWidth;
//    int TextureHeight;
//    int TextureChannels;
//    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
//    if (ImageData != NULL)
//    {
//        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);
//
//        GLint InternalFormat = -1;
//        switch (TextureChannels) {
//        case 1: InternalFormat = GL_RED; break;
//        case 2: InternalFormat = GL_RG; break;
//        case 3: InternalFormat = GL_RGB; break;
//        case 4: InternalFormat = GL_RGBA; break;
//        default: InternalFormat = GL_RGB; break;
//        }
//
//        unsigned int Texture;
//        glGenTextures(1, &Texture);
//        glBindTexture(GL_TEXTURE_2D, Texture);
//        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
//        glBindTexture(GL_TEXTURE_2D, 0);
//        stbi_image_free(ImageData);
//        return Texture;
//    }
//    else
//    {
//        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
//        stbi_image_free(ImageData);
//        return 0;
//    }
//}