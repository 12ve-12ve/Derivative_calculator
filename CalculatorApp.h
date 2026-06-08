#ifndef CalculatorApp_h
#define CalculatorApp_h

#include <vector>
#include "External/ImGui/imgui.h"
#include "External/ImGui/implot.h"
#include "External/ImGui/imgui_stdlib.h"
#include "FormulaTree.h"

/**
 * @brief Class responsible for rendering the elements of an app and drawing a plot.
 * * This class stores variables needed for the rendering of an app, managing a text input, 
 * displaying formulas, calculating points on the graph as well as ImGui and ImPlot flags.
 * It has methods for rendering, managing a user interface and drawing a graph.
 */
class CalculatorApp
{
    //Constants
    static constexpr int STEPS = 10001;     ///< Number of points that are drawn on the graph.
    static constexpr int BUFFER_SIZE = 64;  ///< The maximum size of formula that can be imputed.

    //Input field flags and parameters
    bool input_focus = true;    ///< Flag used to force or maintain focus on the text input field.
    bool formula_changed = false;   ///< Flag indicating that formula has changed.
    int last_cursor_position = 0;   ///< Stores the last position of the cursor in the input field.
    bool show_help_window = false;  ///< Flag toggling the visibility of the help window.
    bool parsing_error = false;     ///< Displays an error massage if true.

    //Plot data
    bool pos_reset = false;     ///< Flag indicating if the plot camera position should be reset.
    std::vector<float> x_points;    ///< X-axis coordinates for tha main plot.
    std::vector<float> y_points;    ///< Y-axis coordinates for tha main plot.
    std::vector<float> x_der_points;    ///< X-axis coordinates for tha derivative plot.
    std::vector<float> y_der_points;    ///< Y-axis coordinates for tha derivative plot.

    //Text buffers
    std::string formula = "";   ///< User's input.
    std::string parsed_formula = "";    ///< Formula inputted by the user.
    std::string derivative_formula = "";    ///< Formula of the derivative.

    //Abstract Syntax Trees
    std::unique_ptr<Node> head_formula = nullptr;   ///< Smart pointer to the root node of the primary formula tree.
    std::unique_ptr<Node> head_derivative = nullptr;    ///< Smart pointer to the root node of the derivative tree.

    //GUI and Rendering State
    ImGuiWindowFlags window_flags;  ///< Configuration flags for the main ImGui app.
    ImPlotRect prev_limits;     ///< Stores the previous plot bounding box to detect camera movement.

    /**
     * @brief Inserts a number, sign or function into formula string.
     * * This function inserts a text from a button that was pressed by the user into the formula string.
     * Before that it checks if there is enough free space for that text to be put into the string.
     * * @param sign Text from the button.
     */
    void AddSign(const char* sign);

    /**
     * @brief Renders one button that has a number on it.
     * * Function displays the sign parameter on the button. 
     * It changes color when is hovered and return true value if it's pressed.
     * * @param sign The text displayed on the button.
     * @return bool Indicates if the button was pressed.
     */
    bool NumberButton(const char* sign);

    /**
     * @brief Renders one button that has a constant or an operation sign on it.
     * * Function displays the sign parameter on the button.
     * It changes color when is hovered and return true value if it's pressed.
     * * @param sign The text displayed on the button.
     * @return bool Indicates if the button was pressed.
     */
    bool SignAndOperationButton(const char* sign);

    /**
     * @brief Renders one button that has a function on it.
     * * Function displays the sign parameter on the button.
     * It changes color when is hovered and return true value if it's pressed.
     * * @param sign The text displayed on the button.
     * @return bool Indicates if the button was pressed.
     */
    bool FunctionButton(const char* sign);
    
    /**
     * @brief Renders all of the NumberButtons and SignAndOperationButton.
     */
    void SetOfCalcButtons();

    /**
     * @brief Renders a table of all of the FunctionButtons.
     * * @param width The size of the table.
     */
    void TabOfFunctions(float width);

    /**
     * @brief Callback function for handling the formula text input field.
     * * This method is triggered by ImGui during text input operations. It is
     * primarily used to manage the text cursor state and maintain input focus.
     * * @param data Pointer to the ImGui callback data structure containing the current state of the text input.
     * @return int Returns 0 to indicate successful execution.
     */
    static int FormulaInputCallback(ImGuiInputTextCallbackData* data);

    /**
     * @brief Render the "Help" window.
     * * Displays a "Help" window with tips and guidence for the user.
     */
    void RenderHelpMenu();

    /**
     * @brief Divides the inputted formula into tokens.
     * * This function divides the formula inserted by the user and creates a vector of tokens
     * that represent singular operations, constants or variable.
     * Afterwards it sends it to create a formula tree from it.
     */
    void ParseFormula();

    /**
     * @brief Renders the graph of a function.
     * * It checks coordinates that are seen by the user and calculates the points of the formula
     * as well as the derivative. Only starts to calculate the points again if the camera was moved.
     * It also has a mechanism to avoid creating asymptotes and interruptions in the plot.
     */
    void ShowFormulaPlot();

public:

    /**
     * @brief Default constructor for the CalculatorApp.
     * * Initializes the default application state, configures ImGui window flags,
     * and prepares the mathematical data buffers for rendering.
     */
    CalculatorApp();

    /**
     * @brief Renders all of the calculator app and its components.
     */
    void Render();
};

#endif // !CalculatorApp_h