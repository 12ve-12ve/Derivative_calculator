#ifndef CalculatorApp_h
#define CalculatorApp_h

#include <vector>
#include "External/ImGui/imgui.h"
#include "External/ImGui/implot.h"
#include "External/ImGui/imgui_stdlib.h"

class CalculatorApp
{
    static constexpr int STEPS = 3001;
    static constexpr int BUFFER_SIZE = 64;

    bool pos_reset = false;
    bool input_focus = true;
    int last_cursor_position = 0;

    std::vector<float> x_points;
    std::vector<float> y_points;
    std::vector<float> der_points;

    std::string formula;
    std::string parsed_formula;
    std::string derivative_formula;

    ImGuiWindowFlags window_flags;
    ImPlotSpec spec;

    void AddSign(const char* sign);
    bool NumFunButton(const char* sign);
    bool SignAndOperationButton(const char* sign);
    bool FunctionButton(const char* sign);
    void SetOfCalcButtons();
    void TabOfFunctions(float width);
    static int FormulaInputCallback(ImGuiInputTextCallbackData* data);

public:
    CalculatorApp();
    void Render();
};

#endif // !CalculatorApp_h