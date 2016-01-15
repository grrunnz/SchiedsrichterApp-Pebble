/*
 * main.c
 */

#include "main.h"

#define PERSIST_KEY_TICKS 52
#define PERSIST_KEY_STOPWATCHVIBRATE 53
  


/********************************* UpdateDiplay *********************************/

static void updateTextLayer(int hours, int minutes, int seconds) {
  // Update the TextLayer
  snprintf(s_uptime_buffer, sizeof(s_uptime_buffer), "Spielzeit:\n %dh %dm %ds", hours, minutes, seconds);
  text_layer_set_text(s_uptime_layer, s_uptime_buffer);
}

static void updateCountdownLayer(int hours, int minutes, int seconds) {
  // Update the TextLayer
  snprintf(s_countdown_buffer, sizeof(s_countdown_buffer), "Countdown:\n %dh %dm %ds", hours, minutes, seconds);
  text_layer_set_text(s_countdown_layer, s_countdown_buffer);
}

/********************************* ActionMenu *********************************/

static void action_performed_callback(ActionMenu *action_menu, const ActionMenuItem *action, void *context) {
  // An action was selected, determine which one
  Time type = *(Time*)action_menu_item_get_action_data(action);

  switch(type) {
    //case VibrationTypeShort:  vibes_short_pulse();  break;
    //case VibrationTypeLong:   vibes_long_pulse();   break;
    //case VibrationTypeDouble: vibes_double_pulse(); break;
    default:
       switch(type) {
          case Minutes2:  
          countdownsub = false;
          s_countdownstart = 120; 
          s_countdowntime = s_countdownstart;  
          updateCountdownLayer(0,(s_countdownstart/60),0);
          break;
          case Minutes5:
          countdownsub = false;
          s_countdownstart = 5*60; 
          s_countdowntime = s_countdownstart;  
          updateCountdownLayer(0,(s_countdownstart/60),0);
          break;
          case Minutes10:  
          countdownsub = false;
          s_countdownstart = 10*60; 
          s_countdowntime = s_countdownstart;  
          updateCountdownLayer(0,(s_countdownstart/60),0);
          break;
         case swMinutes0:  
           persist_write_int(PERSIST_KEY_STOPWATCHVIBRATE,0);
           StopwatchMinutesVibrate = 0;
          break;
         case swMinutes10:  
           persist_write_int(PERSIST_KEY_STOPWATCHVIBRATE,10);
           StopwatchMinutesVibrate = 10;
          break;
         case swMinutes15:  
           persist_write_int(PERSIST_KEY_STOPWATCHVIBRATE,15);
           StopwatchMinutesVibrate = 15;
          break;
         case swMinutes20:  
           persist_write_int(PERSIST_KEY_STOPWATCHVIBRATE,20);
           StopwatchMinutesVibrate = 20;
          break;
         case swMinutes25:  
           persist_write_int(PERSIST_KEY_STOPWATCHVIBRATE,25);
           StopwatchMinutesVibrate = 25;
          break;
         case swMinutes30:  
           persist_write_int(PERSIST_KEY_STOPWATCHVIBRATE,30);
           StopwatchMinutesVibrate = 30;
          break;
         case swMinutes35:  
           persist_write_int(PERSIST_KEY_STOPWATCHVIBRATE,35);
           StopwatchMinutesVibrate = 35;
          break;
         case swMinutes40:  
           persist_write_int(PERSIST_KEY_STOPWATCHVIBRATE,40);
           StopwatchMinutesVibrate = 40;
          break;
         case swMinutes45:  
           persist_write_int(PERSIST_KEY_STOPWATCHVIBRATE,45);
           StopwatchMinutesVibrate = 45;
          break;
          default: break;
        }
      break;
  }
}

static void init_action_menu() {
  // Create the root level
  s_root_level = action_menu_level_create(2);

  // Set up the actions for this level, using action context to pass types
  //action_menu_level_add_action(s_root_level, "Short", action_performed_callback, 
  //                             &(Context){.type=VibrationTypeShort});
  //action_menu_level_add_action(s_root_level, "Long", action_performed_callback, 
  //                             &(Context){.type=VibrationTypeLong});
  //action_menu_level_add_action(s_root_level, "Double", action_performed_callback, 
  //                             &(Context){.type=VibrationTypeDouble});
  
  // Create and set up the secondary level, adding it as a child to the root one
  s_countdown_level = action_menu_level_create(3);
  action_menu_level_add_child(s_root_level, s_countdown_level, "Countdown");

  // Set up the secondary actions
  action_menu_level_add_action(s_countdown_level, "2 Minuten", action_performed_callback, 
                               &(Context){.type=Minutes2});
  action_menu_level_add_action(s_countdown_level, "5 Minuten", action_performed_callback, 
                               &(Context){.type=Minutes5});
  action_menu_level_add_action(s_countdown_level, "10 Minuten", action_performed_callback, 
                               &(Context){.type=Minutes10});
  
  s_stopwatch_level = action_menu_level_create(9);
  
  action_menu_level_add_child(s_root_level, s_stopwatch_level, "Stopwatch");
  
  action_menu_level_add_action(s_stopwatch_level, "vibration off", action_performed_callback, 
                               &(Context){.type=swMinutes0});
  action_menu_level_add_action(s_stopwatch_level, "vibrate on 45 Minuten", action_performed_callback, 
                               &(Context){.type=swMinutes45});
  action_menu_level_add_action(s_stopwatch_level, "vibrate on 40 Minuten", action_performed_callback, 
                               &(Context){.type=swMinutes40});
  action_menu_level_add_action(s_stopwatch_level, "vibrate on 35 Minuten", action_performed_callback, 
                               &(Context){.type=swMinutes35});
  action_menu_level_add_action(s_stopwatch_level, "vibrate on 30 Minuten", action_performed_callback, 
                               &(Context){.type=swMinutes30});
  action_menu_level_add_action(s_stopwatch_level, "vibrate on 25 Minuten", action_performed_callback, 
                               &(Context){.type=swMinutes25});
  action_menu_level_add_action(s_stopwatch_level, "vibrate on 20 Minuten", action_performed_callback, 
                               &(Context){.type=swMinutes20});
  action_menu_level_add_action(s_stopwatch_level, "vibrate on 15 Minuten", action_performed_callback, 
                               &(Context){.type=swMinutes15});
  action_menu_level_add_action(s_stopwatch_level, "vibrate on 10 Minuten", action_performed_callback, 
                               &(Context){.type=swMinutes10});
}

/********************************* Background Worker *********************************/
#define WORKER_TICKS 0

static void worker_message_handler(uint16_t type, AppWorkerMessage *data) {
  if (type == WORKER_TICKS) {
    // Read ticks from worker's packet
    int ticks = data->data0;
    
     // Get time since launch
    int seconds = ticks % 60;
    int minutes = (ticks % 3600) / 60;
    int hours = ticks / 3600;
     
    if(minutes == StopwatchMinutesVibrate && seconds == 0 && hours == 0) {
      vibes_double_pulse();
    }
  
    updateTextLayer(hours, minutes, seconds); 
    
  }
}



/********************************* Tick Service *********************************/

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00:00"), "%H:%M:%S", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00:00"), "%I:%M:%S", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  if(countdownsub){
    if(s_countdowntime >= 0) {
      // Get time since launch
      int seconds = s_countdowntime % 60;
      int minutes = (s_countdowntime % 3600) / 60;
      int hours = s_countdowntime / 3600;
  
      updateCountdownLayer(hours, minutes, seconds); 

      // Decrement s_countdowntime
      s_countdowntime--;
      if(s_countdowntime < 0)
      {
        vibes_short_pulse();
        countdownsub = false;
      }
    }
  }
  
  update_time();
  
}

/*********************************** Clicks ***********************************/

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  //stopwatchsub = !stopwatchsub;
  
  // Check to see if the worker is currently active
  bool running = app_worker_is_running();

  // Toggle running state
  AppWorkerResult result;
  if (running) {
    result = app_worker_kill();

    if (result == APP_WORKER_RESULT_SUCCESS) {
      //text_layer_set_text(s_ticks_layer, "Worker stopped!");
    } else {
      //text_layer_set_text(s_ticks_layer, "Error killing worker!");
    }
  } else {
    result = app_worker_launch();

    if (result == APP_WORKER_RESULT_SUCCESS) {
      //text_layer_set_text(s_ticks_layer, "Worker launched!");
    } else {
      //text_layer_set_text(s_ticks_layer, "Error launching worker!");
    }
  }

  APP_LOG(APP_LOG_LEVEL_INFO, "Result: %d", result);  
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(s_output_layer, "Select pressed!");
  //light_enable_interaction();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(s_output_layer, "Down pressed!");
  countdownsub = !countdownsub;
}

void up_long_click_handler(ClickRecognizerRef recognizer, void *context) {
    updateTextLayer(0,0,0);  
}

void up_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
  if(!app_worker_is_running()) {
     persist_write_int(PERSIST_KEY_TICKS, 0);
  }
}

void down_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(!countdownsub)
  {   
    s_countdowntime = s_countdownstart;
  
    updateCountdownLayer(0,(s_countdownstart/60),0);
  }  
}

void back_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(countdownsub)
  { 
    
    //save data
    
    
    dialog_choice_window_push();
    
  }
  else
  {
    // save data 
    
    window_stack_pop_all(true);
  }
}

void down_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
  //
}

void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Settings 
  // Configure the ActionMenu Window about to be shown
  ActionMenuConfig config = (ActionMenuConfig) {
    .root_level = s_root_level,
    .colors = {
      .background = GColorIslamicGreen,
      .foreground = GColorWhite,
    },
    .align = ActionMenuAlignCenter
  };

  // Show the ActionMenu
  s_action_menu = action_menu_open(&config);
}

void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
  //
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
  
  // long click config:
  window_long_click_subscribe(BUTTON_ID_UP, 700, up_long_click_handler, up_long_click_release_handler);
  window_long_click_subscribe(BUTTON_ID_DOWN, 700, down_long_click_handler, down_long_click_release_handler);
  window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler, select_long_click_release_handler);
}

/******************************** Main Window *********************************/

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_uptime_layer = text_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  text_layer_set_font(s_uptime_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_background_color(s_uptime_layer, GColorClear);
  text_layer_set_text_color(s_uptime_layer, GColorWhite);
  text_layer_set_text_alignment(s_uptime_layer, GTextAlignmentCenter);
  updateTextLayer(0,0,0);
  layer_add_child(window_layer, text_layer_get_layer(s_uptime_layer));
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 60, 144, 45));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  // Create output CountDownLayer
  s_countdown_layer = text_layer_create(GRect(0, 105, 144, 50));
  text_layer_set_font(s_countdown_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_countdown_layer, GTextAlignmentCenter);
  text_layer_set_background_color(s_countdown_layer, GColorClear);
  text_layer_set_text_color(s_countdown_layer, GColorWhite);
  updateCountdownLayer(0,5,0);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_countdown_layer));
  
}

static void main_window_unload(Window *window) {
  //tick_timer_service_unsubscribe();
  
  // Destroy output TextLayer
  text_layer_destroy(s_uptime_layer);
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  // Destroy CountdownLayer
  text_layer_destroy(s_countdown_layer);
  
  //action_bar_layer_destroy(s_action_bar);
  
  action_menu_hierarchy_destroy(s_root_level, NULL, NULL);
}

static void init(void) {
  
  StopwatchMinutesVibrate = persist_read_int(PERSIST_KEY_STOPWATCHVIBRATE);
  // Create main Window
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorIslamicGreen);
  window_set_click_config_provider(s_main_window, click_config_provider);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
  
  init_action_menu();
  
  int ticks;
  
  ticks = persist_read_int(PERSIST_KEY_TICKS);
  
   // Get time since launch
    int seconds = ticks % 60;
    int minutes = (ticks % 3600) / 60;
    int hours = ticks / 3600;
  
    updateTextLayer(hours, minutes, seconds); 
  
  // Make sure the time is displayed from the start
  update_time();

  

  // Subscribe to TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  //tick_timer_service_subscribe(SECOND_UNIT, countdown_handler);
  //tick_timer_service_subscribe(MINUTE_UNIT, tick_handler2);
  app_worker_message_subscribe(worker_message_handler);
}

static void deinit(void) {
  // Destroy main Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

