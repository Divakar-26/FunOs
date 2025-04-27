// Foreground text on black background
#define BLACK_ON_BLACK      ((0 << 4) | 0)
#define BLUE_ON_BLACK       ((0 << 4) | 1)
#define GREEN_ON_BLACK      ((0 << 4) | 2)
#define CYAN_ON_BLACK       ((0 << 4) | 3)
#define RED_ON_BLACK        ((0 << 4) | 4)
#define MAGENTA_ON_BLACK    ((0 << 4) | 5)
#define BROWN_ON_BLACK      ((0 << 4) | 6)
#define LIGHTGRAY_ON_BLACK  ((0 << 4) | 7)
#define DARKGRAY_ON_BLACK   ((0 << 4) | 8)
#define LIGHTBLUE_ON_BLACK  ((0 << 4) | 9)
#define LIGHTGREEN_ON_BLACK ((0 << 4) | 10)
#define LIGHTCYAN_ON_BLACK  ((0 << 4) | 11)
#define LIGHTRED_ON_BLACK   ((0 << 4) | 12)
#define LIGHTMAGENTA_ON_BLACK ((0 << 4) | 13)
#define YELLOW_ON_BLACK     ((0 << 4) | 14)
#define WHITE_ON_BLACK      ((0 << 4) | 15)

// Foreground text on white background
#define BLACK_ON_WHITE      ((15 << 4) | 0)
#define BLUE_ON_WHITE       ((15 << 4) | 1)
#define GREEN_ON_WHITE      ((15 << 4) | 2)
#define RED_ON_WHITE        ((15 << 4) | 4)
#define MAGENTA_ON_WHITE    ((15 << 4) | 5)
#define YELLOW_ON_WHITE     ((15 << 4) | 14)
#define WHITE_ON_WHITE      ((15 << 4) | 15) // Invisible text!!

// Reverse combinations
#define WHITE_ON_RED        ((4 << 4) | 15)
#define WHITE_ON_GREEN      ((2 << 4) | 15)
#define WHITE_ON_BLUE       ((1 << 4) | 15)
#define BLACK_ON_GREEN      ((2 << 4) | 0)
#define BLACK_ON_CYAN       ((3 << 4) | 0)
