#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "quads.h"
#include "renderer.h"
#include "triangles.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void process_input();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static GLFWwindow* g_window = nullptr;
static Renderer* g_renderer = nullptr;
static QuadManager* g_quad_manager = nullptr;
static TriangleManager* g_triangle_manager = nullptr;

int main(int argc, char* argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    g_window = glfwCreateWindow(800, 600, "demo", nullptr, nullptr);
    if (!g_window) {
        std::cout << "GLFW failed to create window\n" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(g_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to load GLAD\n";
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(g_window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

    g_renderer = new Renderer{g_window, 800, 600};
    glfwSetFramebufferSizeCallback(g_window, &framebuffer_size_callback);
    auto& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    g_quad_manager = new QuadManager;
    g_triangle_manager = new TriangleManager;
    g_quad_manager->set_projection_mat(glm::ortho(0.0f, 800.f, 0.0f, 600.0f, -1.0f, 1.0f));
    g_quad_manager->set_view_mat(glm::mat4(1.0f));
    g_triangle_manager->set_projection_mat(glm::ortho(0.0f, 800.f, 0.0f, 600.0f, -1.0f, 1.0f));
    g_triangle_manager->set_view_mat(glm::mat4(1.0f));
    std::shared_ptr<Quad> quad = std::make_shared<Quad>(ColorF{100, 0, 0, 255});
    std::shared_ptr<Quad> quad2 = std::make_shared<Quad>(ColorF{0, 100, 0, 255});
    VertexF vertices[3] = {VertexF{-50, -50}, VertexF{50, -50}, VertexF{0, 50}};
    std::shared_ptr<Triangle> triangle =
        std::make_shared<Triangle>(vertices, ColorF{0, 0, 100, 255});
    g_quad_manager->add(quad2);
    g_quad_manager->add(quad);
    g_triangle_manager->add(triangle);

    glm::vec2 offset{0, 0}, offset2{0, 0}, offset3{0, 0};
    glm::vec2 scale{100.0f}, scale2{100.0f}, scale3{1.0f};
    float rotation = 0, rotation2 = 0, rotation3 = 0;
    while (!glfwWindowShouldClose(g_window)) {
        process_input();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Transform");

        ImGui::SliderFloat2("Offset", &offset.x, 0.0f, 800.0f);
        ImGui::SliderFloat2("Scale", &scale.x, 0.0f, 400.0f);
        ImGui::SliderFloat("Rotation", &rotation, 0.0f, 2.0f * 3.14f);

        ImGui::SliderFloat2("Offset2", &offset2.x, 0.0f, 800.0f);
        ImGui::SliderFloat2("Scale2", &scale2.x, 0.0f, 400.0f);
        ImGui::SliderFloat("Rotation2", &rotation2, 0.0f, 2.0f * 3.14f);

        ImGui::SliderFloat2("Offset3", &offset3.x, 0.0f, 800.0f);
        ImGui::SliderFloat2("Scale3", &scale3.x, 0.0f, 400.0f);
        ImGui::SliderFloat("Rotation3", &rotation3, 0.0f, 2.0f * 3.14f);

        ImGui::End();

        ImGui::Render();
        g_renderer->set_clear_color(Color{180, 180, 200, 255});
        g_renderer->begin_drawing();

        quad->set_position(offset);
        quad->set_rotation(rotation);
        quad->set_scale(scale);
        quad2->set_position(offset2);
        quad2->set_rotation(rotation2);
        quad2->set_scale(scale2);
        triangle->set_position(offset3);
        triangle->set_scale(scale3);
        triangle->set_rotation(rotation3);

        g_quad_manager->draw();
        g_triangle_manager->draw();

        glfwPollEvents();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
        g_renderer->end_drawing(g_window);
    }

    delete g_renderer;
    delete g_quad_manager;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(g_window);
    glfwTerminate();
    return 0;
}

void process_input() {
    if (glfwGetKey(g_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(g_window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    g_renderer->resize_viewport(width, height);
}
