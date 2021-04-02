#include "context.h"

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

///CMakeLists.txt에서 정의시킴.(7~9),(22~27)줄.
///#define WINDOW_NAME "Hello, OpenGL"
///#define WINDOW_WIDTH 960
///#define WINDOW_HEIGHT 540

void OnFramebufferSizeChange(GLFWwindow* window, int width, int height) {
    SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);
    glViewport(0, 0, width, height);
}

void OnKeyEvent(GLFWwindow* window,
    int key, int scancode, int action, int mods) {
    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}",
        key, scancode,
        action == GLFW_PRESS ? "Pressed" :
        action == GLFW_RELEASE ? "Released" :
        action == GLFW_REPEAT ? "Repeat" : "Unknown",
        mods & GLFW_MOD_CONTROL ? "C" : "-",
        mods & GLFW_MOD_SHIFT ? "S" : "-",
        mods & GLFW_MOD_ALT ? "A" : "-");
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, const char** argv) {
    SPDLOG_INFO("Start OpenGL Program!");
    SPDLOG_INFO("arg count: {}", argc);
    for (int i = 0; i < argc; i++){
        SPDLOG_INFO("argv[{}]: {}", i, argv[i]);
    }

    float circleRadius = 0.75f;
    float circleRadiusin = 0.5f;
    int circleSegmentCount = 16;
    float circleDegreeStart = 0.0;
    float circleDegreeEnd = 360.0;
    float circle_colorRad = 0.0f;
    float circle_colorGreen = 0.0f;
    float circle_colorBlue = 0.0f;

    if (argc >= 2){
        circleRadius = std::stof (argv[1]);
    }
    if (argc >= 3){
       circleRadiusin = std::stof (argv[2]);
    }
    if (argc >= 4){
        circleSegmentCount = std::stoi (argv[3]);
    }
    if (argc >= 5){
        circleDegreeStart = std::stof (argv[4]);
    }
    if (argc >= 6){
        circleDegreeEnd = std::stof (argv[5]);
    }
    if (argc >= 7){
        circle_colorRad = std::stof (argv[6]);
    }
    if (argc >= 8){
        circle_colorGreen = std::stof (argv[7]);
    }
    if (argc >= 9){
        circle_colorBlue = std::stof (argv[8]);
    }

    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit()) {
        const char* description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initialize glfw: {}", description);
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw 윈도우 생성, 실패하면 에러 출력후 종료 (2021 03 10)
    SPDLOG_INFO("Create glfw window");
    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!window) {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }
	
    glfwMakeContextCurrent(window);

    // glad를 활용한 OpenGL 함수 로딩
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    SPDLOG_ERROR("failed to initialize glad");
    glfwTerminate();
    return -1;
    }
    
    auto glVersion = glGetString(GL_VERSION);
    SPDLOG_INFO("OpenGL context version: {}", glVersion);

    OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
    glfwSetKeyCallback(window, OnKeyEvent);
    
    auto context = Context::Create();
    if (!context) {
        SPDLOG_ERROR("failed to create context");
        glfwTerminate();
        return -1;
    }

    context -> CreateCircle(circleRadius, circleRadiusin, circleSegmentCount, circleDegreeStart, circleDegreeEnd, circle_colorRad, circle_colorGreen, circle_colorBlue);
    
    
  // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료 (2021 03 10)
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        context -> Render();
        glfwSwapBuffers(window);
    }
    
    context.reset();

    glfwTerminate();
    return 0;
}

