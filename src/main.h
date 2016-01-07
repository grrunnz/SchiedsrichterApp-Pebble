#include <pebble.h>

  typedef enum {
  Minutes2,
  Minutes5,
  Minutes10,
  swMinutes0,
  swMinutes10,
  swMinutes15,
  swMinutes20,
  swMinutes25,
  swMinutes30,
  swMinutes35,
  swMinutes40,
  swMinutes45
} Time;

typedef struct {
  Time type;
} Context;

static Window *s_main_window;
static TextLayer *s_uptime_layer;
static TextLayer *s_time_layer;
static TextLayer *s_countdown_layer;
static char s_uptime_buffer[32];
static char s_countdown_buffer[32];
int StopwatchMinutesVibrate;
bool countdownsub = false;

static int s_countdowntime = 300;
static int s_countdownstart = 300;

static ActionMenu *s_action_menu;
static ActionMenuLevel *s_root_level, *s_countdown_level, *s_stopwatch_level;


static void updateTextLayer(int hours, int minutes, int seconds);
static void updateCountdownLayer(int hours, int minutes, int seconds);

static void action_performed_callback(ActionMenu *action_menu, const ActionMenuItem *action, void *context);
static void init_action_menu();
static void update_time();
static void tick_handler(struct tm *tick_time, TimeUnits units_changed);

static void up_click_handler(ClickRecognizerRef recognizer, void *context);
static void select_click_handler(ClickRecognizerRef recognizer, void *context);
static void down_click_handler(ClickRecognizerRef recognizer, void *context);
void up_long_click_handler(ClickRecognizerRef recognizer, void *context);

void down_long_click_handler(ClickRecognizerRef recognizer, void *context);

void select_long_click_handler(ClickRecognizerRef recognizer, void *context);

static void click_config_provider(void *context);


static void main_window_load(Window *window);
static void main_window_unload(Window *window);
static void init(void);
static void deinit(void);
int main(void);