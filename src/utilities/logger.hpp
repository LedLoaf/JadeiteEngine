/*  Simple colored text for logging 
    NOTE: I considered using SetConsoleColor from Windows, but apparently ANSI works on all OS. It's also less calls per function */
#include <iostream>
#include <string>
#include <iomanip>

//==========================================================================================================================

/* Basic ANSI colors */
enum LogColor {
    // Normal
    Black    = 30,
    Red      = 31,
    Green    = 32,
    Yellow   = 33,
    Blue     = 34,
    Magenta  = 35,
    Cyan     = 36,
    White    = 37,

    // Bright
    BrightBlack   = 90,
    BrightRed     = 91,
    BrightGreen   = 92,
    BrightYellow  = 93,
    BrightBlue    = 94,
    BrightMagenta = 95,
    BrightCyan    = 96,
    BrightWhite   = 97,

    // 256-color
    Coral    = 213,
    Lavender = 141,
    Gold     = 220,
    Orange   = 208,
    Pink     = 214,
    Gray     = 250,
};

//==========================================================================================================================

/*  Logging function with colors: (Red, Green, Blue, Orange, Pink, Gold, Coral, Lavender, Gray) */
inline  void Log(int color, const std::string& text) 
{
    std::cout << "\033[38;5;" << color << "m" << text << "\033[0m\n";
}

//==========================================================================================================================

/*  Logging function with colors and padding: Apply Bright prefix to the colors as well (Red, Green, Blue, Orange, Pink, Gold, Coral, Lavender, Gray) 
    Note: I called it Print instead of Log because of how different it is from log and has a prefix AND a label parameter to include */
inline void Print(int color, const std::string& prefix, const std::string& label, int prefixWidth = 19, int labelWidth = 20, int prefixColor = 97) 
{
    std::cout << "\033[38;5;"	 		<< prefixColor << "m"
              << std::left 				<< std::setw(prefixWidth) << prefix
              << "\033[38;5;" 			<< color << "m"
              << std::setw(labelWidth) 	<< label
              << "\033[0m\n";
}     

//==========================================================================================================================

/*  Error logging with colors and padding: Apply Bright prefix to the colors as well (Red, Green, Blue, Orange, Pink, Gold, Coral, Lavender, Gray) */
inline  void LogError(int color, const std::string& text) 
{
    std::cerr << "\033[38;5;" << color << "m" << text << "\033[0m\n";
}

//==========================================================================================================================

/*  Error Logging function with colors and padding: Apply Bright prefix to the colors as well (Red, Green, Blue, Orange, Pink, Gold, Coral, Lavender, Gray) */
inline void LogError(int color, const std::string& type, const std::string& label, int typeWidth = 7, int labelWidth = 20) {
    std::cout << "\033[38;5;" 						<< color << "m"
              << "Adding " 							<< std::setw(typeWidth) << type
              << std::setw(labelWidth)				<< label
              << "\033[0m\n";
}
