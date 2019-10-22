//
// Created by sneeuwpop on 12-09-19.
//

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "glm/gtx/rotate_vector.hpp"
#include <stack>
#include "imgui.h"
#include <GLFW/glfw3.h>

#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include <iostream>

#include "../util/interfaces/screen.h"
#include "../graphics/shader_program.h"
#include "../graphics/flying_camera.h"
#include "../util/input/keyboard.h"
#include "../game/lsystem.h"
#include "../util/debug/gizmos.h"
#include "../game/marching_cubes_terrain.h"

using namespace glm;

class TreeScreen : public Screen
{
public:
    FlyingCamera camera = FlyingCamera();
    GLint MVPLocation;
    ShaderProgram defaultShaderProgram = ShaderProgram::fromAssetFiles("shaders/default.vert", "shaders/default.frag");

    Gizmos gizmos;
    MarchingCubesTerrain terrain;

    const static int cacheSize = 100;
    vec3 plusCache[cacheSize];
    vec3 minCache[cacheSize];

    const float angle = radians(33.3f);

    TreeScreen()
    {
        for (int i=0; i<cacheSize; i++) {
            plusCache[i] = MATH::randomPointOnSphere(70, 180);
            minCache[i] = MATH::randomPointOnSphere(70, -180);
        }
        // Shader Program
        defaultShaderProgram.use();

        // Model View Projection
        MVPLocation = defaultShaderProgram.uniformLocation("MVP");
    }

    void setup(GLFWwindow* window) {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        // TODO: Set optional io.ConfigFlags values, e.g. 'io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard' to enable keyboard controls.
        // TODO: Fill optional fields of the io structure later.
        // TODO: Load TTF/OTF fonts if you don't want to use the default font.

        const char* glsl_version = "#version 300 es"; // TODO: We should not hardcoded that here

        // Initialize helper Platform and Renderer bindings (here we are using imgui_impl_win32 and imgui_impl_dx11)
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    double time = 0;
    bool anyKeyPressed = false;
    mat4 modelMatrix = translate(mat4(1.0f), vec3(0, 0, 0)); // identity matrix

    std::string buf = std::string("F[-x[+x]++x]") + std::string(10, '\0');
    int applyNtimes = 5;
    bool twoD = false;
    float growthSize = 0.2f;

    void render(double deltaTime)
    {
        time += deltaTime;
        glClearColor(135.0/255.0, 206.0/255.0, 235.0/255.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////// CAMERA

        defaultShaderProgram.use(); // imgui may have changed the current shader

        if (anyKeyPressed)
            camera.update(deltaTime);
        else {
            if (INPUT::KEYBOARD::pressed(GLFW_KEY_TAB))
                anyKeyPressed = true;
            camera.position = vec3(sin(time * 0.5) *5,  2,  cos(time * 0.5) *5);
            camera.lookAt(VEC3::Y);
            camera.Camera::update();
        }

        mat4 mvp = camera.combined * modelMatrix;
        glUniformMatrix4fv( MVPLocation, 1, GL_FALSE, &mvp[0][0]);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////// TEST

        terrain.render();

        vec3 ringPos = VEC3::Y;
        vec3 rotationAngle = VEC3::Z;
        float width = 0.5f;

        vec3 oldPos = VEC3::X * width;
        oldPos = glm::rotate(oldPos, (float) mod(time/10.0f, 0.5), rotationAngle);
        int n = 10;
        float stepSize = 2*MATH::PI / static_cast<float>(n);
        float v = stepSize;

        for (int j=0; j <= 5; j++) {
            for (int i = 1; i <= n; i++) {
                vec3 newPos = vec3(cos(v)*(width - (j/15.f)), 0, sin(v)*(width - (j/15.f)));
                newPos = glm::rotate(newPos, (float) mod(time / 10.0f, 0.5), rotationAngle);
//                gizmos.drawLine(ringPos + oldPos, ringPos + newPos); // original circle

                gizmos.drawLine(ringPos + oldPos, ringPos + newPos); // bottom
                gizmos.drawLine(ringPos + oldPos, ringPos + oldPos + VEC3::Y); // left
//                gizmos.drawLine(ringPos + newPos, ringPos + newPos + VEC3::Y);
                gizmos.drawLine(ringPos + oldPos + VEC3::Y, ringPos + newPos);
//                gizmos.drawLine(ringPos + oldPos + VEC3::Y, ringPos + newPos + VEC3::Y);

                oldPos = newPos;
                v += stepSize;
            }
            ringPos += VEC3::Y;
        }

//        for (float i=0; i <= 180.0f; i += 0.1f)
//            gizmos.drawCube(VEC3::Y * 1.5f + MATH::randomPointOnSphere(i, -180.0f), 0.01f, vec4(i/180.0f,i/180.0,i/180.0,0));

//        gizmos.drawLine(VEC3::ZERO, VEC3::X * 5.0f, COLOR::RED);
//        gizmos.drawLine(VEC3::ZERO, VEC3::Y * 5.0f, COLOR::GREEN);
//        gizmos.drawLine(VEC3::ZERO, VEC3::Z * 5.0f, COLOR::BLUE);


        //////////////////////////////////////////////////////////////////////////////////////////////////////// LSYSTEM
        LSystem lSystem = LSystem("X");
        lSystem.addPattern('X', buf); // "F[-X][+X],FX"

        vec3 curPoint = VEC3::ZERO, oldPoint = VEC3::ZERO;
        vec3 direction = VEC3::Y;

        std::stack<vec3> memory = std::stack<vec3>();

        lSystem.applyNtimes(applyNtimes);
        std::string str = lSystem.getStr();
        for (int i = 0; i < str.length(); i++) {
            switch (toupper(str[i])) {
                case 'F': oldPoint = vec3(curPoint); curPoint += (direction) * growthSize; gizmos.drawLine(oldPoint, curPoint, COLOR::BLACK); break;
                case '+': direction = twoD ? glm::rotate(direction, angle, VEC3::Z) : plusCache[i % cacheSize]; break;
                case '-': direction = twoD ? glm::rotate(direction, -angle, VEC3::Z) : minCache[i % cacheSize]; break;
                case '[': memory.push(curPoint); memory.push(direction); break;
                case ']':
                    if (memory.empty()) break;
                    direction = memory.top(); memory.pop();
                    curPoint = memory.top(); memory.pop(); break;
            }
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////// GUI
        defaultShaderProgram.use();
        VertexBuffer::bindDefault();

        // Feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("LSystem pattern");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Change the text and see what happens");
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::Text(" F : Move forward");
        ImGui::Text(" X : Replace this X with the pattern");
        ImGui::Text(" + : Rotate right \n - : Rotate left");
        ImGui::Text(" [ : Store current location \n ] : Pop last stored location");
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        //        char *buf = new char[10];
        ImGui::InputText("pattern", &buf[0], buf.size()+ 1 );
        ImGui::Text("Times to apply pattern");
        ImGui::SliderInt("", &applyNtimes, 1, 6);
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::SliderFloat("Growth size", &growthSize, 0, 1);
        ImGui::Checkbox("2d instead of 3d", &twoD);
        ImGui::End();

        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void resize(int width, int height)
    {
        camera.resize(width, height);
    }
};