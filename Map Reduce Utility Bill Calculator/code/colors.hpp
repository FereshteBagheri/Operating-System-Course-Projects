#include <ostream>

enum class Color {
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37,
    DEFAULT = 39,
    RESET = 0,
};

std::ostream& operator<<(std::ostream& os, Color clr) {
    return os << "\x1B[" << static_cast<int>(clr) << 'm';
}
