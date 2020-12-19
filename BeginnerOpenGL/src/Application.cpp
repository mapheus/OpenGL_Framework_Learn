#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include <array>


#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "PerlinNoise.hpp"
#include "MarchingCubes.h"
#include "Terrain.h"

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(90.f, SCR_WIDTH, SCR_HEIGHT);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

/*
mesh_data meshFromTriangles(std::vector<triangle>& triangles) {
	auto vertices = std::vector<glm::vec3>(triangles.size() * 3);
	auto normals = std::vector<glm::vec3>(triangles.size() * 3);
	auto index_sets = std::vector<glm::uvec3>(triangles.size());

	size_t a = 0u;
	for (unsigned int i = 0; i < triangles.size(); i++) {
		vertices[a] = glm::vec3(triangles[i].vertex[0]);
		normals[a] = triangles[i].normal;

		vertices[a + 1] = glm::vec3(triangles[i].vertex[1]);
		normals[a + 1] = triangles[i].normal;

		vertices[a + 2] = glm::vec3(triangles[i].vertex[2]);
		normals[a + 2] = triangles[i].normal;

		index_sets[i] = glm::uvec3(a, a + 1, a + 2);
		a += 3u;
	}

	mesh_data data;
	glGenVertexArrays(1, &data.vao);
	assert(data.vao != 0u);
	glBindVertexArray(data.vao);

	auto const vertices_offset = 0u;
	auto const vertices_size = static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec3));
	auto const normals_offset = vertices_size;
	auto const normals_size = static_cast<GLsizeiptr>(normals.size() * sizeof(glm::vec3));

	auto const bo_size = static_cast<GLsizeiptr>(vertices_size + normals_size);
	glGenBuffers(1, &data.bo);
	assert(data.bo != 0u);
	glBindBuffer(GL_ARRAY_BUFFER, data.bo);
	glBufferData(GL_ARRAY_BUFFER, bo_size, nullptr, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, vertices_offset, vertices_size, static_cast<GLvoid const*>(vertices.data()));
	glEnableVertexAttribArray(static_cast<unsigned int>(shader_bindings::vertices));
	glVertexAttribPointer(static_cast<unsigned int>(shader_bindings::vertices), 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid const*>(0x0));

	glBufferSubData(GL_ARRAY_BUFFER, normals_offset, normals_size, static_cast<GLvoid const*>(normals.data()));
	glEnableVertexAttribArray(static_cast<unsigned int>(shader_bindings::normals));
	glVertexAttribPointer(static_cast<unsigned int>(shader_bindings::normals), 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid const*>(normals_offset));


	glBindBuffer(GL_ARRAY_BUFFER, 0u);

	data.indices_nb = index_sets.size() * 3u;
	glGenBuffers(1, &data.ibo);
	assert(data.ibo != 0u);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(index_sets.size() * sizeof(glm::uvec3)), reinterpret_cast<GLvoid const*>(index_sets.data()), GL_STATIC_DRAW);

	glBindVertexArray(0u);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);

	return data;
}
*/

void computeTest() {
	Shader test("res/shaders/ctest2.comp", true);
	Shader d("res/shaders/default.shader", false);

	int numVertices = 200;
	int dimension = 3;
	size_t size = sizeof(float**) * numVertices * dimension;

	GLuint computeShaderBuffer;
	//Create the buffer the compute shader will write to
	glGenBuffers(1, &computeShaderBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, computeShaderBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, computeShaderBuffer);



	test.Bind();
	glDispatchCompute(numVertices/20, 1, 1);
	GLuint VAO;

	//Generate the vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Use buffer as a set of vertices
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, computeShaderBuffer);


	d.Bind();
	d.SetUniformMatrix4("u_VP", camera.GetProjectionMatrix() * camera.GetViewMatrix());
	d.SetUniformMatrix4("u_Model",
		glm::translate(glm::mat4(1), glm::vec3(0, -3, 0))
		* glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(1, 0, 0))
		* glm::scale(glm::mat4(1.0f), glm::vec3(2)));

	//Enable the vertex array
	glVertexAttribPointer(0, dimension, GL_FLOAT, 0, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Draw it
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
	glFlush();
}

int main(void)
{
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Enable debugging*/
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window;
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1080, 720, "Learn OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;

    float positions[24] = {
        // positions          // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // top right
        0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f // top left 
    };


    unsigned int indices[6] = {
        0, 1, 2,
        2, 3, 0
    };

    VertexArray va;
    
    VertexBuffer vb(positions, 4 * 6 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);
    va.AddBuffer(vb, layout);
    IndexBuffer ib(indices, 6);




    Shader shader("res/shaders/Basic.shader", false);
	GLuint tex_output = 0;

    shader.Unbind();

    Renderer renderer;

    //Model m("res/models/backpack/backpack.obj");
    Terrain terr(TerrainType::MARCHING, glm::vec3(80.f, 40.f, 80.f));
    int chunks = 3;
    terr.SetChunkSize(chunks, chunks);
    float b = glfwGetTime();
    terr.SetPosition(glm::vec3(-10, 0, 0));
    float a = glfwGetTime();
    printf("---- RENDER TERRAIN ----\nChunks: %i\nTime: %f\n", chunks * chunks, a - b);
    MarchingCubes mc("res/shaders/ctest2.comp", "res/shaders/calcValues.comp", "res/shaders/marching.shader");
    b = glfwGetTime();
    mc.UpdateTransform(glm::vec3(80, 0, 0), glm::vec3(80.f, 40.f, 80.f));
    a = glfwGetTime();
    printf("---- RENDER ONE MC ----\nTime: %f\n\n", a - b);


    float f = -3.f;
    float dp = 0.05f;
	
    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        // initialize debug output 
        /* Enable debug again*/
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(glDebugOutput, NULL);
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float add = 0;
	float lastcheck = glfwGetTime();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        renderer.Clear();
        processInput(window);


        //computeTest();
        shader.Bind();
        shader.SetUniform3f("lightPos", glm::vec3(-15, 30, 5));
        shader.SetUniform3f("viewPos", camera.GetPosition());

        shader.SetUniform3f("light.position", glm::vec3(f, 1, 2));
        if (f < -13) {
            dp = 0.05f;
        }
        else if (f > 13) dp = -0.05f;
        f += dp;
        shader.SetUniform3f("light.color", glm::vec3(1, 0.6, 0.2));
        shader.SetUniform1f("light.constant", 1.0f);
        shader.SetUniform1f("light.linear", 0.07f);
        shader.SetUniform1f("light.quadratic", 0.017f);
        // Model pos
		
		shader.Bind();
		shader.SetUniformMatrix4("u_VP", camera.GetProjectionMatrix() * camera.GetViewMatrix());
		shader.SetUniformMatrix4("u_Model",
			glm::translate(glm::mat4(1), glm::vec3(0, -3, 0))
			* glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(1, 0, 0))
			* glm::scale(glm::mat4(1.0f), glm::vec3(30)));
       // m.Draw(shader);
		renderer.Draw(va, ib, shader);
        terr.Draw(camera.GetProjectionMatrix() * camera.GetViewMatrix());
        mc.Draw(camera.GetProjectionMatrix()* camera.GetViewMatrix());

        add += 0.02f;
        if (glfwGetTime() - lastcheck > 2) {
            float b = glfwGetTime();
            terr.SetPosition(glm::vec3(-20.f, 0.f, 0.f+add));
            float a = glfwGetTime();
            printf("---- RENDER TERRAIN ----\nChunks: %i\nTime: %f\n", chunks* chunks, a - b);
            b = glfwGetTime();
            mc.UpdateTransform(glm::vec3(80, 0, 0+add), glm::vec3(80.f, 40.f, 80.f));
            a = glfwGetTime();
            printf("---- RENDER ONE MC ----\nTime: %f\n\n", a - b);
            lastcheck = glfwGetTime();
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
