#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "easylogging++.h"
#include "GameObject.h"
#include "log.h"
#include "PlayerComponent.h"
using tigl::Vertex;


#pragma comment (lib, "opencv_world490d.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

INITIALIZE_EASYLOGGINGPP

GLFWwindow* window;

void init();
void update();
void draw();

int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glwf";
    window = glfwCreateWindow(1400, 800, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);

    tigl::init();

    init();

    while (!glfwWindowShouldClose(window))
    {
        update();
        draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();


    return 0;
}

double lastFrameTime = 0;
std::list<std::shared_ptr<GameObject>> objects;
std::shared_ptr<GameObject> player;

void init()
{
    setupLogger();

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(window, true);
        });

    // Create Player object.
    player = std::make_shared<GameObject>();
    player->position = glm::vec3(0, 1, 5);
    // player->addComponent(std::make_shared<CubeComponent>(1.0f));
    player->addComponent(std::make_shared<PlayerComponent>());
    objects.push_back(player);
}


void update()
{
    // Calculate timings.
    double currentFrameTime = glfwGetTime();
    double deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    // Updated gameObjects.
    for (std::shared_ptr<GameObject>& gameObject : objects)
    {
        gameObject->update(static_cast<float>(deltaTime));
    }
}

void draw()
{
    // Set GL.
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw Gameobjects.
    for (std::shared_ptr<GameObject>& gameObject : objects)
    {
        gameObject->draw();
    }
}
