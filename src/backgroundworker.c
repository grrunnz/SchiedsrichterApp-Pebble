#include <pebble_worker.h>

#define WORKER_TICKS 0
#define PERSIST_KEY_TICKS 52

static int32_t s_ticks = 0;

static void tick_handler(struct tm *tick_timer, TimeUnits units_changed) {
  // Update value
  s_ticks++;

  // Construct a data packet
  AppWorkerMessage msg_data = {
    .data0 = s_ticks
  };

  // Send the data to the foreground app
  app_worker_send_message(WORKER_TICKS, &msg_data);
}

static void worker_init() {
  // Use the TickTimer Service as a data source
  s_ticks = persist_read_int(PERSIST_KEY_TICKS);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void worker_deinit() {
  // Stop using the TickTimerService
  persist_write_int(PERSIST_KEY_TICKS, s_ticks);
  tick_timer_service_unsubscribe();
}

int main(void) {
  worker_init();
  worker_event_loop();
  worker_deinit();
}