#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "shader.h"
#include "camera.h" 

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;
constexpr float ASPECT_RATIO = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);

Camera cam { glm::vec3(0.0f, 0.0f, 5.0f) };
bool first_mouse = true;
float mouse_lastx, mouse_lasty;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window, float delta_time) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.process_movement(FORWARD, delta_time);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.process_movement(BACKWARD, delta_time);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.process_movement(LEFT, delta_time);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.process_movement(RIGHT, delta_time); 
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (first_mouse) {
        mouse_lastx = xpos;
        mouse_lasty = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - mouse_lastx;
    float yoffset = ypos - mouse_lasty;

    mouse_lastx = xpos;
    mouse_lasty = ypos;

    cam.process_mouse(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    cam.process_scroll(static_cast<float>(yoffset));
}

GLFWwindow *init_window() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Heat Equation",
            nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to open GLFW window\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD\n";
        return nullptr;
    }

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    return window;
}

int main() {
    GLFWwindow *window = init_window();
    if (window == nullptr)
        return -1;

    float vertices[] = {
        // Front Face
        0.5f, 0.5f, -0.5f, // 0
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        // Back Face
        0.5f, 0.5f, 0.5f, // 4
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        // Top Face
        0.5f, 0.5f, 0.5f, // 8
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        // Bottom Face
        0.5f, -0.5f, 0.5f, // 12
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        // Left Face
        -0.5f, -0.5f, 0.5f, // 16
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        // Right Face
        0.5f, -0.5f, 0.5f, // 20
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f
    };

    unsigned int indices[] = {
        // Front Face
        0, 1, 3,
        1, 2, 3,
        // Back Face
        4, 5, 7,
        5, 6, 7,
        // Top Face
        8, 9, 11,
        9, 10, 11,
        // Bottom Face
        12, 13, 15,
        13, 14, 15,
        // Left Face
        16, 17, 19,
        17, 18, 19,
        // Right Face
        20, 21, 23,
        21, 22, 23
    };

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
            (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    Shader shader { "shaders/cube.vert", "shaders/cube.frag" };
    glm::mat4 model, view, projection;

    shader.use();
    model = glm::mat4(1.0f);
    shader.set_mat4("model", model);

    float last_frame = 0.0f;
    float current_frame, delta_time;
    while (!glfwWindowShouldClose(window)) {
        current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        process_input(window, delta_time);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        
        projection = glm::perspective(
                glm::radians(cam.get_zoom()), 
                ASPECT_RATIO, 
                0.1f, 
                100.0f
        );
        view = cam.get_view_matrix();

        shader.set_mat4("projection", projection);
        shader.set_mat4("view", view);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
