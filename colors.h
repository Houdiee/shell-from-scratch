#ifndef COLORS_H
#define COLORS_H

#define ESC "\033["

#define RESET ESC "0m"

#define BOLD ESC "1m"
#define FAINT ESC "2m"
#define ITALIC ESC "3m"
#define UNDERLINE ESC "4m"
#define BLINK ESC "5m"
#define INVERSE ESC "7m"
#define HIDDEN ESC "8m"
#define STRIKETHROUGH ESC "9m"

#define BLACK_COLOR ESC "30m"
#define RED_COLOR ESC "31m"
#define GREEN_COLOR ESC "32m"
#define YELLOW_COLOR ESC "33m"
#define BLUE_COLOR ESC "34m"
#define MAGENTA_COLOR ESC "35m"
#define CYAN_COLOR ESC "36m"
#define WHITE_COLOR ESC "37m"

#define BG_BLACK_COLOR ESC "40m"
#define BG_RED_COLOR ESC "41m"
#define BG_GREEN_COLOR ESC "42m"
#define BG_YELLOW_COLOR ESC "43m"
#define BG_BLUE_COLOR ESC "44m"
#define BG_MAGENTA_COLOR ESC "45m"
#define BG_CYAN_COLOR ESC "46m"
#define BG_WHITE_COLOR ESC "47m"

#endif
