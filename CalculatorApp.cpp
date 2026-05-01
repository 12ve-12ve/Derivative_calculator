#include "CalculatorApp.h"
#include <cmath>

CalculatorApp::CalculatorApp()
{
    formula.reserve(BUFFER_SIZE);
    parsed_formula = "1/x";
    derivative_formula = "-1/(x^2)";

    spec.LineWeight = 3.0f;
    window_flags =  ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

    x_points.resize(STEPS);
    y_points.resize(STEPS);
    der_points.resize(STEPS);
}

void CalculatorApp::AddSign(const char* sign)
{
    size_t formula_size = formula.size();
    size_t sign_size = strlen(sign);
    if (formula_size + sign_size + 1 <= BUFFER_SIZE)
    {
        formula.insert(last_cursor_position, sign);
        last_cursor_position += sign_size;
        input_focus = true;
    }
    else
        input_focus = false;
}

bool CalculatorApp::NumFunButton(const char* sign)
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

bool CalculatorApp::SignAndOperationButton(const char* sign)
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

bool CalculatorApp::FunctionButton(const char* sign)
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

void CalculatorApp::SetOfCalcButtons()
{
    if (SignAndOperationButton("π"))
        AddSign("π");
    ImGui::SameLine();
    if (SignAndOperationButton("e"))
        AddSign("e");
    ImGui::SameLine();
    if (SignAndOperationButton("AC"))
    {
        formula[0] = '\0';
        input_focus = true;
    }
    ImGui::SameLine();
    if (SignAndOperationButton("÷"))
        AddSign("÷");
    if (NumFunButton("7"))
        AddSign("7");
    ImGui::SameLine();
    if (NumFunButton("8"))
        AddSign("8");
    ImGui::SameLine();
    if (NumFunButton("9"))
        AddSign("9");
    ImGui::SameLine();
    if (SignAndOperationButton("×"))
        AddSign("×");
    if (NumFunButton("4"))
        AddSign("4");
    ImGui::SameLine();
    if (NumFunButton("5"))
        AddSign("5");
    ImGui::SameLine();
    if (NumFunButton("6"))
        AddSign("6");
    ImGui::SameLine();
    if (SignAndOperationButton("-"))
        AddSign("-");
    if (NumFunButton("1"))
        AddSign("1");
    ImGui::SameLine();
    if (NumFunButton("2"))
        AddSign("2");
    ImGui::SameLine();
    if (NumFunButton("3"))
        AddSign("3");
    ImGui::SameLine();
    if (SignAndOperationButton("+"))
        AddSign("+");
    if (NumFunButton("Help"))
        ;
    ImGui::SameLine();
    if (NumFunButton("0"))
        AddSign("0");
    ImGui::SameLine();
    if (NumFunButton("."))
        AddSign(".");
    ImGui::SameLine();
    if (SignAndOperationButton("="))
        ;
}

void CalculatorApp::TabOfFunctions(float width)
{
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
                AddSign("sin(x)");
            ImGui::SameLine();
            if (NumFunButton("cos(x)"))
                AddSign("cos(x)");
            ImGui::SameLine();
            if (NumFunButton("tan(x)"))
                AddSign("tan(x)");
            if (NumFunButton("asin(x)"))
                AddSign("asin(x)");
            ImGui::SameLine();
            if (NumFunButton("acos(x)"))
                AddSign("acos(x)");
            ImGui::SameLine();
            if (NumFunButton("atan(x)"))
                AddSign("atan(x)");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Hyperbolic"))
        {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
            if (NumFunButton("sinh(x)"))
                AddSign("sinh(x)");
            ImGui::SameLine();
            if (NumFunButton("cosh(x)"))
                AddSign("cosh(x)");
            ImGui::SameLine();
            if (NumFunButton("tanh(x)"))
                AddSign("tanh(x)");
            if (NumFunButton("asinh(x)"))
                AddSign("asinh(x)");
            ImGui::SameLine();
            if (NumFunButton("acosh(x)"))
                AddSign("acosh(x)");
            ImGui::SameLine();
            if (NumFunButton("atanh(x)"))
                AddSign("atanh(x)");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Exp. & Log."))
        {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
            if (NumFunButton("x^a"))
                AddSign("x^a");
            ImGui::SameLine();
            if (NumFunButton("e^x"))
                AddSign("e^x");
            ImGui::SameLine();
            if (NumFunButton("a^x"))
                AddSign("a^x");
            if (NumFunButton("ln(x)"))
                AddSign("ln(x)");
            ImGui::SameLine();
            if (NumFunButton("log10(x)"))
                AddSign("log10(x)");
            ImGui::SameLine();
            if (NumFunButton("log(a, x)"))
                AddSign("log(a, x)");
            ImGui::EndTabItem();
        }
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
        ImGui::PopFont();
        ImGui::EndTabBar();
    }
}

int CalculatorApp::FormulaInputCallback(ImGuiInputTextCallbackData* data)
{
    CalculatorApp* app = (CalculatorApp*)data->UserData;

    if (app == nullptr)
        return 0;

    if (app->input_focus == true)
    {
        data->CursorPos = app->last_cursor_position;
        data->SelectionStart = data->CursorPos;
        data->SelectionEnd = data->CursorPos;

        app->input_focus = false;
    }

    app->last_cursor_position = data->CursorPos;

    return 0;
}

void CalculatorApp::Render(void)
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("##calc", NULL, window_flags);
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 10, ImGui::GetCursorPosY() + 10));

    ImGui::BeginGroup();

    ImGui::SetNextItemWidth(504);

    ImGui::PushFont(NULL, 32);
    if (input_focus)
        ImGui::SetKeyboardFocusHere();

    ImGui::InputText("##Formula", &formula,
        ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackAlways,
        FormulaInputCallback, this);
    ImGui::PopFont();

    SetOfCalcButtons();
    TabOfFunctions(ImGui::GetContentRegionAvail().x);

    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 660);
    ImGui::BeginGroup();
    if (ImPlot::BeginPlot("##plot", ImVec2(650, 650)))
    {
        if (!pos_reset)
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
                y_points[i] = der_points[i] = NAN;
            else
            {
                y_points[i] = 1.0f / x;
                der_points[i] = -1.0f / (float)pow(x, 2);
            }
        }
        ImPlot::PlotLine("f(x) = 1/x", x_points.data(), y_points.data(), STEPS, spec);
        ImPlot::PlotLine("f'(x) = -1/(x^2)", x_points.data(), der_points.data(), STEPS, spec);
        ImPlot::EndPlot();
    }

    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize("Reset position").x + style.FramePadding.x * 2.0f) - 3);
    if (ImGui::Button("Reset Position"))
        pos_reset = true;

    ImGui::PushFont(NULL, 30);
    ImGui::Text(" f(x)=%s", parsed_formula.c_str(), 2 * BUFFER_SIZE);
    ImGui::PopFont();

    ImGui::PushFont(NULL, 30);
    ImGui::Text("f'(x)=%s", derivative_formula.c_str(), 2 * BUFFER_SIZE);
    ImGui::PopFont();

    ImGui::EndGroup();

    ImGui::End();
}