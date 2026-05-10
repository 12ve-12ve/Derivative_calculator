#include "CalculatorApp.h"
#include <cmath>
#include "Parser.h"

CalculatorApp::CalculatorApp()
{
    formula.reserve(BUFFER_SIZE);

    window_flags =  ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
    
    prev_limits.X.Min = -10;
    prev_limits.X.Max = 10;
    prev_limits.Y.Min = -10;
    prev_limits.Y.Max = 10;

    x_points.resize((int)(1.5 * STEPS));
    y_points.resize((int)(1.5 * STEPS));
    x_der_points.resize((int)(1.5 * STEPS));
    y_der_points.resize((int)(1.5 * STEPS));
}

void CalculatorApp::AddSign(const char* sign)
{
    size_t formula_size = formula.size();
    size_t sign_size = strlen(sign);
    if (formula_size + sign_size + 1 <= BUFFER_SIZE)
    {
        formula.insert(last_cursor_position, sign);
        last_cursor_position += (int)sign_size;
        input_focus = true;
    }
    else
        input_focus = false;
}

bool CalculatorApp::NumberButton(const char* sign)
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
    ImGui::PushFont(NULL, 30);
    ImVec2 size = ImVec2(140, 80);

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
    //if (SignAndOperationButton("π"))
    //    AddSign("π");
    if (SignAndOperationButton("PI"))
        AddSign("PI");
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
    //if (SignAndOperationButton("÷"))
    //    AddSign("÷");
    if (SignAndOperationButton("/"))
        AddSign("/");
    if (NumberButton("7"))
        AddSign("7");
    ImGui::SameLine();
    if (NumberButton("8"))
        AddSign("8");
    ImGui::SameLine();
    if (NumberButton("9"))
        AddSign("9");
    ImGui::SameLine();
    //if (SignAndOperationButton("×"))
    //    AddSign("×");
    if (SignAndOperationButton("*"))
        AddSign("*");
    if (NumberButton("4"))
        AddSign("4");
    ImGui::SameLine();
    if (NumberButton("5"))
        AddSign("5");
    ImGui::SameLine();
    if (NumberButton("6"))
        AddSign("6");
    ImGui::SameLine();
    if (SignAndOperationButton("-"))
        AddSign("-");
    if (NumberButton("1"))
        AddSign("1");
    ImGui::SameLine();
    if (NumberButton("2"))
        AddSign("2");
    ImGui::SameLine();
    if (NumberButton("3"))
        AddSign("3");
    ImGui::SameLine();
    if (SignAndOperationButton("+"))
        AddSign("+");
    if (NumberButton("Help"))
        ;
    ImGui::SameLine();
    if (NumberButton("0"))
        AddSign("0");
    ImGui::SameLine();
    if (NumberButton("."))
        AddSign(".");
    ImGui::SameLine();
    if (SignAndOperationButton("="))
        ParseFormula();
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
            if (FunctionButton("sin(x)"))
                AddSign("sin(x)");
            ImGui::SameLine();
            if (FunctionButton("cos(x)"))
                AddSign("cos(x)");
            ImGui::SameLine();
            if (FunctionButton("tan(x)"))
                AddSign("tan(x)");
            if (FunctionButton("arcsin(x)"))
                AddSign("arcsin(x)");
            ImGui::SameLine();
            if (FunctionButton("arccos(x)"))
                AddSign("arccos(x)");
            ImGui::SameLine();
            if (FunctionButton("arctan(x)"))
                AddSign("arctan(x)");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Hyperbolic"))
        {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
            if (FunctionButton("sinh(x)"))
                AddSign("sinh(x)");
            ImGui::SameLine();
            if (FunctionButton("cosh(x)"))
                AddSign("cosh(x)");
            ImGui::SameLine();
            if (FunctionButton("tanh(x)"))
                AddSign("tanh(x)");
            if (FunctionButton("arsinh(x)"))
                AddSign("arsinh(x)");
            ImGui::SameLine();
            if (FunctionButton("arcosh(x)"))
                AddSign("arcosh(x)");
            ImGui::SameLine();
            if (FunctionButton("artanh(x)"))
                AddSign("artanh(x)");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Exp. & Log."))
        {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
            if (FunctionButton("pow(x, a)"))
                AddSign("pow(x, a)");
            ImGui::SameLine();
            if (FunctionButton("pow(e, x)"))
                AddSign("pow(e, x)");
            ImGui::SameLine();
            if (FunctionButton("sqrt(x)"))
                AddSign("sqrt(x)");
            if (FunctionButton("ln(x)"))
                AddSign("ln(x)");
            ImGui::SameLine();
            if (FunctionButton("log10(x)"))
                AddSign("log10(x)");
            ImGui::SameLine();
            if (FunctionButton("log(a, x)"))
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

    if (ImGui::InputText("##Formula", &formula,
        ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackAlways,
        FormulaInputCallback, this))
    {
        ParseFormula();
        input_focus = true;
    }
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

        ImPlotSpec axis_spec;
        axis_spec.LineWeight = 2.0f;
        axis_spec.LineColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
        double zero = 0.0;
        ImPlot::PlotInfLines("##Y_Axis", &zero, 1, axis_spec);
        axis_spec.Flags = ImPlotInfLinesFlags_Horizontal;
        ImPlot::PlotInfLines("##X_Axis", &zero, 1, axis_spec);

        ShowFormulaPlot();

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

void CalculatorApp::ParseFormula()
{
    std::vector<Token> tokens;
    std::string formula_part;
    Token new_token;
    int i = 0;
    if (!formula.empty() && formula[0] == '-')
    {
        i++;
        new_token.val = '0';
        new_token.token_type = Token::TokenType::CONSTANT;
        tokens.push_back(new_token);
        new_token.val = '-';
        new_token.token_type = Token::TokenType::OPERATOR;
        tokens.push_back(new_token);
        formula_part.clear();
    }

    for (; i < formula.size(); ++i)
    {
        if (isblank(formula[i]))
            continue;
        else if (isalnum(formula[i]) || formula[i] == '.')
            formula_part += formula[i];
        else if (OperatorSign(formula[i]))
        {
            if (!formula_part.empty())
            {
                if (CheckToken(formula_part, new_token))
                {
                    tokens.push_back(new_token);
                    formula_part.clear();
                }
                else 
                    return;
            }    
            tokens.push_back(CreateOperatorToken(formula[i]));
        }
        else
            return;
    }

    if (!formula_part.empty())
    {
        if (CheckToken(formula_part, new_token))
        {
            tokens.push_back(new_token);
            formula_part.clear();
        }
        else
            return;
    }

    head_formula = ModifyToRPN(tokens);
    if (head_formula != nullptr)
    {
        formula_changed = true;
        parsed_formula = "f(x)=" + formula;
        head_derivative = head_formula->GetDerivative();
    }
}

void CalculatorApp::ShowFormulaPlot()
{
    if (head_formula != nullptr)
    {
        ImPlotRect limits = ImPlot::GetPlotLimits();
        bool camera_moved = (prev_limits.X.Min != limits.X.Min) || (prev_limits.X.Max != limits.X.Max) || 
            (prev_limits.Y.Min != limits.Y.Min) || (prev_limits.Y.Max != limits.Y.Max);

        if (camera_moved || formula_changed)
        {
            x_points.clear();
            y_points.clear();
            x_der_points.clear();
            y_der_points.clear();
            formula_changed = false;

            double x_min = limits.X.Min;
            double x_max = limits.X.Max;
            double x_range = x_max - x_min;
            double y_range = limits.Y.Max - limits.Y.Min;
            float prev_y = NAN;
            float prev_der_y = NAN;

            for (int i = 0; i < STEPS; ++i)
            {
                float x = (float)(x_min + (x_range * i / (STEPS - 1)));
                float y = head_formula->Calculate(x);
                if (std::isinf(y)) y = NAN;

                if (!std::isnan(prev_y) && !std::isnan(y))
                {
                    bool sign_changed = (prev_y > 0.0f && y < 0.0f) || (prev_y < 0.0f && y > 0.0f);
                    bool jump = std::abs(y - prev_y) > (y_range * 1.5);

                    if (sign_changed && jump)
                    {
                        x_points.push_back(x);
                        y_points.push_back(NAN);
                    }
                }
                x_points.push_back(x);
                y_points.push_back(y);
                prev_y = y;

                if (head_derivative != nullptr)
                {
                    float der_y = head_derivative->Calculate(x);
                    if (std::isinf(der_y)) der_y = NAN;

                    if (!std::isnan(prev_der_y) && !std::isnan(der_y))
                    {
                        bool sign_changed = (prev_der_y > 0.0f && der_y < 0.0f) || (prev_der_y < 0.0f && der_y > 0.0f);
                        bool jump = std::abs(der_y - prev_der_y) > (y_range * 1.5);

                        if (sign_changed && jump)
                        {
                            x_der_points.push_back(x);
                            y_der_points.push_back(NAN);
                        }
                    }
                    x_der_points.push_back(x);
                    y_der_points.push_back(der_y);

                    prev_der_y = der_y;
                }
            }
            prev_limits = limits;
        }
        ImPlotSpec f_x_spec;
        f_x_spec.LineWeight = 3.0f;
        f_x_spec.LineColor = ImVec4(0.26f, 0.53f, 0.96f, 1.0f);
        ImPlot::PlotLine(parsed_formula.c_str(), x_points.data(), y_points.data(), (int)x_points.size(), f_x_spec); 
        if (head_derivative != nullptr && !x_der_points.empty())
        {
            f_x_spec.LineColor = ImVec4(0.99f, 0.73f, 0.01f, 1.0f);
            ImPlot::PlotLine("f'(x)", x_der_points.data(), y_der_points.data(), (int)x_der_points.size(), f_x_spec);
        }
    }
}