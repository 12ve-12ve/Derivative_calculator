#ifndef CalculatorApp_h
#define CalculatorApp_h

#include <vector>
#include "External/ImGui/imgui.h"
#include "External/ImGui/implot.h"
#include "External/ImGui/imgui_stdlib.h"
#include "FormulaTree.h"

class CalculatorApp
{
    static constexpr int STEPS = 10001;
    static constexpr int BUFFER_SIZE = 64;

    bool pos_reset = false;
    bool input_focus = true;
    bool formula_changed = false;
    int last_cursor_position = 0;

    std::vector<float> x_points;
    std::vector<float> y_points;
    std::vector<float> x_der_points;
    std::vector<float> y_der_points;

    std::string formula = "";
    std::string parsed_formula = "";
    std::string derivative_formula = "";

    std::unique_ptr<Node> head_formula = nullptr;
    std::unique_ptr<Node> head_derivative = nullptr;

    ImGuiWindowFlags window_flags;
    ImPlotRect prev_limits;

    void AddSign(const char* sign);
    bool NumberButton(const char* sign);
    bool SignAndOperationButton(const char* sign);
    bool FunctionButton(const char* sign);
    void SetOfCalcButtons();
    void TabOfFunctions(float width);
    static int FormulaInputCallback(ImGuiInputTextCallbackData* data);
    void ParseFormula();
    void ShowFormulaPlot();

public:
    CalculatorApp();
    void Render();
};

#endif // !CalculatorApp_h