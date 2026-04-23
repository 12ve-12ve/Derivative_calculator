#include "External/ImGui/imgui.h"
#include "External/ImGui/imgui_impl_glfw.h"
#include "External/ImGui/imgui_impl_opengl3.h"
#include "External/ImGui/implot.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cctype>
#include <cmath>

#define BUFFER_SIZE 64

bool AddSign(char* buff, const char* sign)
{
    size_t formula_size = strlen(buff);
    size_t sign_size = strlen(sign);

    if (formula_size + sign_size + 1 <= BUFFER_SIZE)
    {
        strcat_s(buff, BUFFER_SIZE, sign);
        return true;
    }
    return false;
}

bool NumFunButton(const char* sign)
{
    ImGui::PushID(sign);
    ImGui::PushFont(NULL, 32);
    ImVec2 size = ImVec2(120, 80);

    ImVec2 p = ImGui::GetCursorScreenPos();
    bool clicked = ImGui::InvisibleButton(sign, size);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImU32 color_bg = ImGui::GetColorU32(ImVec4(0.89f, 0.89f, 0.89f, 1.0f));
    ImU32 color_hover = ImGui::GetColorU32(ImVec4(0.82f, 0.82f, 0.82f, 1.0f));
    ImU32 color_text = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

    draw_list->AddRectFilled(p, ImVec2(p.x + size.x, p.y + size.y), color_bg, 8.0f);

    if (ImGui::IsItemHovered())
        draw_list->AddRectFilled(p, ImVec2(p.x + size.x, p.y + size.y), color_hover, 8.0f);

    ImVec2 text_size = ImGui::CalcTextSize(sign);
    ImVec2 text_pos = ImVec2(p.x + (size.x - text_size.x) * 0.5f, p.y + (size.y - text_size.y) * 0.5f);
    draw_list->AddText(text_pos, color_text, sign);
    ImGui::PopFont();
    ImGui::PopID();
    return clicked;
}

bool SignAndOperationButton(const char* sign)
{
    ImGui::PushID(sign);
    ImGui::PushFont(NULL, 32);
    ImVec2 size = ImVec2(120, 80);

    ImVec2 p = ImGui::GetCursorScreenPos();
    bool clicked = ImGui::InvisibleButton(sign, size);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImU32 color_bg = ImGui::GetColorU32(ImVec4(0.82f, 0.82f, 0.82f, 1.0f));
    ImU32 color_hover = ImGui::GetColorU32(ImVec4(0.89f, 0.89f, 0.89f, 1.0f));
    ImU32 color_text = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

    draw_list->AddRectFilled(p, ImVec2(p.x + size.x, p.y + size.y), color_bg, 8.0f);

    if (ImGui::IsItemHovered())
        draw_list->AddRectFilled(p, ImVec2(p.x + size.x, p.y + size.y), color_hover, 8.0f);

    ImVec2 text_size = ImGui::CalcTextSize(sign);
    ImVec2 text_pos = ImVec2(p.x + (size.x - text_size.x) * 0.5f, p.y + (size.y - text_size.y) * 0.5f);
    draw_list->AddText(text_pos, color_text, sign);
    ImGui::PopFont();
    ImGui::PopID();
    return clicked;
}

bool FunctionButton(const char* sign)
{
    ImGui::PushID(sign);
    ImGui::PushFont(NULL, 28);
    ImVec2 size = ImVec2(80, 60);

    ImVec2 p = ImGui::GetCursorScreenPos();
    bool clicked = ImGui::InvisibleButton(sign, size);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImU32 color_bg = ImGui::GetColorU32(ImVec4(0.89f, 0.89f, 0.89f, 1.0f));
    ImU32 color_hover = ImGui::GetColorU32(ImVec4(0.82f, 0.82f, 0.82f, 1.0f));
    ImU32 color_text = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

    draw_list->AddRectFilled(p, ImVec2(p.x + size.x, p.y + size.y), color_bg, 8.0f);

    if (ImGui::IsItemHovered())
        draw_list->AddRectFilled(p, ImVec2(p.x + size.x, p.y + size.y), color_hover, 8.0f);

    ImVec2 text_size = ImGui::CalcTextSize(sign);
    ImVec2 text_pos = ImVec2(p.x + (size.x - text_size.x) * 0.5f, p.y + (size.y - text_size.y) * 0.5f);
    draw_list->AddText(text_pos, color_text, sign);
    ImGui::PopFont();
    ImGui::PopID();
    return clicked;
}

void SetOfCalcButtons(char* f, bool& input)
{
    size_t before = strlen(f);
    if (SignAndOperationButton("π"))
        AddSign(f, "π");
    ImGui::SameLine();
    if (SignAndOperationButton("e"))
        AddSign(f, "e");
    ImGui::SameLine();
    if (SignAndOperationButton("AC"))
    {
        f[0] = '\0';
        input = true;
    }
    ImGui::SameLine();
    if (SignAndOperationButton("÷"))
        AddSign(f, "÷");
    if (NumFunButton("7"))
        AddSign(f, "7");
    ImGui::SameLine();
    if (NumFunButton("8"))
        AddSign(f, "8");
    ImGui::SameLine();
    if (NumFunButton("9"))
        AddSign(f, "9");
    ImGui::SameLine();
    if (SignAndOperationButton("×"))
        AddSign(f, "×");
    if (NumFunButton("4"))
        AddSign(f, "4");
    ImGui::SameLine();
    if (NumFunButton("5"))
        AddSign(f, "5");
    ImGui::SameLine();
    if (NumFunButton("6"))
        AddSign(f, "6");
    ImGui::SameLine();
    if (SignAndOperationButton("-"))
        AddSign(f, "-");
    if (NumFunButton("1"))
        AddSign(f, "1");
    ImGui::SameLine();
    if (NumFunButton("2"))
        AddSign(f, "2");
    ImGui::SameLine();
    if (NumFunButton("3"))
        AddSign(f, "3");
    ImGui::SameLine();
    if (SignAndOperationButton("+"))
        AddSign(f, "+");
    if (NumFunButton("Help"))
        ;
    ImGui::SameLine();
    if (NumFunButton("0"))
        AddSign(f, "0");
    ImGui::SameLine();
    if (NumFunButton("."))
        AddSign(f, ".");
    ImGui::SameLine();
    if (SignAndOperationButton("="))
        ;
    size_t after = strlen(f);
    if (before != after)
        input = true;
}

void TabOfFunctions(char* f, bool& input, float width)
{
    size_t before = strlen(f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 25);

    ImGui::PushFont(NULL, 26);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20.0f, 10.0f));
    ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(0.65f, 0.70f, 0.75f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TabHovered, ImVec4(0.70f, 0.78f, 0.98f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TabActive, ImVec4(0.78f, 0.86f, 0.95f, 1.0f));
    if (ImGui::BeginTabBar("Functions"))
    {
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 5, ImGui::GetCursorPosY() - 5));
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImU32 line_color = ImGui::GetColorU32(ImVec4(0.78f, 0.86f, 0.95f, 1.0f));
        ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + width, p.y + 8.0f), line_color);
        if (ImGui::BeginTabItem("Trigonometric"))
        {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
            if (NumFunButton("sin(x)"))
                AddSign(f, "sin(x)");
            ImGui::SameLine();
            if (NumFunButton("cos(x)"))
                AddSign(f, "cos(x)");
            ImGui::SameLine();
            if (NumFunButton("tan(x)"))
                AddSign(f, "tan(x)");
            if (NumFunButton("asin(x)"))
                AddSign(f, "asin(x)");
            ImGui::SameLine();
            if (NumFunButton("acos(x)"))
                AddSign(f, "acos(x)");
            ImGui::SameLine();
            if (NumFunButton("atan(x)"))
                AddSign(f, "atan(x)");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Hyperbolic"))
        {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
            if (NumFunButton("sinh(x)"))
                AddSign(f, "sinh(x)");
            ImGui::SameLine();
            if (NumFunButton("cosh(x)"))
                AddSign(f, "cosh(x)");
            ImGui::SameLine();
            if (NumFunButton("tanh(x)"))
                AddSign(f, "tanh(x)");
            if (NumFunButton("asinh(x)"))
                AddSign(f, "asinh(x)");
            ImGui::SameLine();
            if (NumFunButton("acosh(x)"))
                AddSign(f, "acosh(x)");
            ImGui::SameLine();
            if (NumFunButton("atanh(x)"))
                AddSign(f, "atanh(x)");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Exp. & Log."))
        {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
            if (NumFunButton("x^a"))
                AddSign(f, "x^a");
            ImGui::SameLine();
            if (NumFunButton("e^x"))
                AddSign(f, "e^x");
            ImGui::SameLine();
            if (NumFunButton("a^x"))
                AddSign(f, "a^x");
            if (NumFunButton("log(10, x)"))
                AddSign(f, "log(10, x)");
            ImGui::SameLine();
            if (NumFunButton("log(e, x)"))
                AddSign(f, "log(e, x)");
            ImGui::SameLine();
            if (NumFunButton("log(a, x)"))
                AddSign(f, "log(a, x)");
            ImGui::EndTabItem();
        }
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
        ImGui::PopFont();
        ImGui::EndTabBar();
    }

    size_t after = strlen(f);
    if (before != after)
        input = true;
}

static int CursorToEndCallback(ImGuiInputTextCallbackData* data)
{
    bool* focus_flag = (bool*)data->UserData;

    if (focus_flag != nullptr && *focus_flag == true)
    {
        data->CursorPos = data->BufTextLen;
        data->SelectionStart = data->BufTextLen;
        data->SelectionEnd = data->BufTextLen;

        *focus_flag = false;
    }
    return 0;
}

void glfw_error_callback(int error, const char* description) 
{ 
    std::cerr << "GLFW Error " << error << ": " << description << std::endl; 
}

int main() 
{
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) 
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

#if defined(__APPLE__)
    // GL 3.2 + GLSL 150 (MacOS)
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on MacOS
#else
    // GL 3.0 + GLSL 130 (Windows and Linux)
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(1200, 800, "Derivative Calculator", nullptr, nullptr);
    if (!window) 
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    static const ImWchar signs[] = { 0x0020, 0x00FF, 0x0370, 0x03FF, 0, };
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontFromFileTTF("External\\Fonts\\Arial.ttf");

    ImPlotSpec spec;
    spec.LineWeight = 3.0f;

    ImGui::StyleColorsLight();
    ImPlot::StyleColorsLight();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    bool pos_reset = false;
    static bool input_focus = true;
    const int STEPS = 3001;
    static float x_points[STEPS];
    static float y_points[STEPS];
    static float der_points[STEPS];
    char formula[BUFFER_SIZE] = "\0";
    char parsed_formula[2 * BUFFER_SIZE] = "1/x\0";
    char derivative_formula[2 * BUFFER_SIZE] = "-1/(x^2)\0";

    while (!glfwWindowShouldClose(window)) 
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("##calc", NULL, window_flags);
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 10, ImGui::GetCursorPosY() + 10));

        ImGui::BeginGroup();

        ImGui::SetNextItemWidth(504);
        
        ImGui::PushFont(NULL, 32);
        if (input_focus)
            ImGui::SetKeyboardFocusHere();

        ImGui::InputText("##Formula", formula, IM_ARRAYSIZE(formula), 
            ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackAlways, 
            CursorToEndCallback, &input_focus);
        ImGui::PopFont();

        SetOfCalcButtons(formula, input_focus);
        TabOfFunctions(formula, input_focus, ImGui::GetContentRegionAvail().x);

        ImGui::EndGroup();

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 660);
        ImGui::BeginGroup();
        if (ImPlot::BeginPlot("##plot", ImVec2(650, 650))) 
        {
            if(!pos_reset)
            {
                ImPlot::SetupAxisLimits(ImAxis_X1, -10.0, 10.0, ImGuiCond_Once);
                ImPlot::SetupAxisLimits(ImAxis_Y1, -10.0, 10.0, ImGuiCond_Once);
            }
            else
            {
                ImPlot::SetupAxisLimits(ImAxis_X1, -10.0, 10.0, ImGuiCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y1, -10.0, 10.0, ImGuiCond_Always);
                pos_reset = false;
            }

            ImPlot::SetupAxes("x", "y");

            ImPlotRect limits = ImPlot::GetPlotLimits();
            double x_min = limits.X.Min;
            double x_max = limits.X.Max;  
            double range = x_max - x_min;

            for (int i = 0; i < STEPS; ++i) 
            {
                float x = (float)(x_min + (range * i / (STEPS - 1)));
                x_points[i] = x;

                double dx = range / STEPS;
                if (std::abs(x) < dx * 0.5)
                {
                    y_points[i] = der_points[i] = NAN;
                }
                else
                {
                    y_points[i] = 1.0f / x;
                    der_points[i] = -1.0f / (float)pow(x, 2);
                }
            }
            ImPlot::PlotLine("f(x) = 1/x", x_points, y_points, STEPS, spec);
            ImPlot::PlotLine("f'(x) = -1/(x^2)", x_points, der_points, STEPS, spec);
            ImPlot::EndPlot();
        }

        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize("Reset position").x + style.FramePadding.x * 2.0f) - 3);
        if (ImGui::Button("Reset Position"))
            pos_reset = true;

        ImGui::PushFont(NULL, 30);
        ImGui::Text(" f(x)=%s", parsed_formula, 2 * BUFFER_SIZE);
        ImGui::PopFont();

        ImGui::PushFont(NULL, 30);
        ImGui::Text("f'(x)=%s", derivative_formula, 2 * BUFFER_SIZE);
        ImGui::PopFont();

        ImGui::EndGroup();

        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}