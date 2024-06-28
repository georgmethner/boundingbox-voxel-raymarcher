#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Input.hpp"
#include "Player.hpp"
#include "Window.hpp"
#include "load/LoadShader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "ObjectHandler.hpp"
#include "FrustumCull.h"

const unsigned int SCREEN_WIDTH = 1920;
const unsigned int SCREEN_HEIGHT = 1080;

const int SIZE = 1024;


const GLfloat vertices[] =
{
        -0.5, -0.5, -0.5,
        0.5, -0.5, -0.5,
        0.5, 0.5, -0.5,
        -0.5, 0.5, -0.5,
        -0.5, -0.5, 0.5,
        0.5, -0.5, 0.5,
        0.5, 0.5, 0.5,
        -0.5, 0.5, 0.5
};

const GLuint indices[] =
{
        0, 1, 2,
        2, 3, 0,
        1, 5, 6,
        6, 2, 1,
        7, 6, 5,
        5, 4, 7,
        4, 0, 3,
        3, 7, 4,
        4, 5, 1,
        1, 0, 4,
        3, 2, 6,
        6, 7, 3
};


const char* screenVertexShaderSource = LoadShader::Load("../src/shader/vertex_pass.glsl");
const char* screenFragmentShaderSource = LoadShader::Load("../src/shader/fragment_pass.glsl");



float deltaTime;
float lastTime;


Input input;
Player player;
Window window;

glm::mat4 proj;
glm::mat4 view;

int skyboxTex;


// DEBUG PARAMETERS
float x1 = 10, x2;
glm::vec2 xy1;


float avg_framerate = 0;
int frame_count = 0;

void handleUI()
{
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

    frame_count++;
    avg_framerate += ImGui::GetIO().Framerate;

   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

	ImGui::Text("%.3f MS", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
    ImGui::Text("%.1f avg FPS", avg_framerate / frame_count);
	ImGui::DragFloat3("pos", &player.position[0], 0.03);
	ImGui::DragFloat3("dir", &player.dir[0], 0.03);

	ImGui::DragFloat("yaw", &player.yaw);
	ImGui::DragFloat("pitch", &player.pitch);

	ImGui::DragFloat("X1", &x1);
	ImGui::DragFloat("X2", &x2);
	ImGui::DragFloat2("XY1", &xy1[0]);
}

void Start()
{
    input = Input();
    input.window = window.getWindow();
    player = Player();
    player.input = input;

    proj = glm::perspective(45.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    xy1 = glm::vec2(0, 0);

    //skyboxTex = createTexture("src/images/aerodynamics_workshop.jpg", 1);
}

void Update()
{
	auto now = (float)glfwGetTime();
	deltaTime = now - lastTime;
	lastTime = now;

	player.Update(deltaTime);

	view = glm::lookAt(player.position, player.position + player.dir, glm::vec3(0.0, 1.0, 0.0));
}

int main()
{
	window = Window();
	window.StartGLFW(SCREEN_WIDTH, SCREEN_HEIGHT);


	GLuint VAO, VBO, EBO;
	glCreateVertexArrays(1, &VAO);
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);

	glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glNamedBufferData(EBO, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(VAO, 0);
	glVertexArrayAttribBinding(VAO, 0, 0);
	glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);

	glEnableVertexArrayAttrib(VAO, 1);
	glVertexArrayAttribBinding(VAO, 1, 0);
	glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));

	glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 3 * sizeof(GLfloat));
	glVertexArrayElementBuffer(VAO, EBO);


	GLuint screenTex;
	glCreateTextures(GL_TEXTURE_2D, 1, &screenTex);
	glTextureParameteri(screenTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(screenTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(screenTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(screenTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(screenTex, 1, GL_RGBA32F, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindImageTexture(0, screenTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	GLuint screenVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(screenVertexShader, 1, &screenVertexShaderSource, nullptr);
	glCompileShader(screenVertexShader);
	GLuint screenFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(screenFragmentShader, 1, &screenFragmentShaderSource, nullptr);
	glCompileShader(screenFragmentShader);

	GLuint screenShaderProgram = glCreateProgram();
	glAttachShader(screenShaderProgram, screenVertexShader);
	glAttachShader(screenShaderProgram, screenFragmentShader);
	glLinkProgram(screenShaderProgram);

	glDeleteShader(screenVertexShader);
	glDeleteShader(screenFragmentShader);

    //Z-Buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


	Start();

    ObjectHandler objectHandler = ObjectHandler();

	while (!glfwWindowShouldClose(window.getWindow()))
	{

		Update();
		handleUI();

        Frustum frustum = Frustum(proj * view);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        glUseProgram(screenShaderProgram);
		glBindTextureUnit(0, screenTex);

        glUniformMatrix4fv(glGetUniformLocation(screenShaderProgram, "proj"), 1, GL_FALSE, &glm::inverse(proj)[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(screenShaderProgram, "view"), 1, GL_FALSE, &glm::inverse(view)[0][0]);
		glUniform1i(glGetUniformLocation(screenShaderProgram, "screen"), 0);

        glUniform1f(glGetUniformLocation(screenShaderProgram, "x1"), x1);

		glBindVertexArray(VAO);


        for (int i = 0; i < ObjectHandler::object_count; i++)
        {
            if (!frustum.IsBoxVisible(ObjectHandler::objects[i].object_matrix * glm::vec4(-0.5, -0.5, -0.5, 1),
                                       ObjectHandler::objects[i].object_matrix * glm::vec4(0.5, 0.5, 0.5, 1)))
                continue;

            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_3D, ObjectHandler::objects[i].texture);
            glUniform1i(glGetUniformLocation(screenShaderProgram, "object_texture"), i);

            glUniform1ui(glGetUniformLocation(screenShaderProgram, "object_index"), i);
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, nullptr);
        }


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();

		
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window.getWindow());
	glfwTerminate();
}