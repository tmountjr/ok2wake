#include <Arduino.h>
#include <DimmableLED.h>
#include <unity.h>

DimmableLED dimmable(16);

/** BEGIN UNIT TEST CODE **/

// void setUp(void) {
// }

// void tearDown(void) {
// }

void test_value_at_tick_fadeup(void) {
  dimmable.reset();
  dimmable.fade(0, 255, 10000);
  TEST_ASSERT_EQUAL(0, dimmable._valueAtTick(0, 0));        // start
  TEST_ASSERT_EQUAL(127, dimmable._valueAtTick(5000, 0));   // half
  TEST_ASSERT_EQUAL(255, dimmable._valueAtTick(10000, 0));  // end
}

void test_value_at_tick_fadedown(void) {
  dimmable.reset();
  dimmable.fade(255, 0, 10000);
  TEST_ASSERT_EQUAL(191, dimmable._valueAtTick(2500, 0)); // 25%
  TEST_ASSERT_EQUAL(63, dimmable._valueAtTick(7500, 0));  // 75%
  TEST_ASSERT_EQUAL(0, dimmable._valueAtTick(10000, 0));  // end
}

void test_value_at_tick_low_edge(void) {
  dimmable.reset();
  dimmable.fade(128, 64, 10000);
  // If the fade starts at 2000 and ends at 12000, the tick at 1000 should still
  // be 128.
  TEST_ASSERT_EQUAL(128, dimmable._valueAtTick(1000, 2000));
}

void test_value_at_tick_high_edge(void) {
  dimmable.reset();
  dimmable.fade(64, 128, 10000);
  // If the fade starts at 0 and ends at 10000, the tick at 12000 should be max
  // but not higher.
  TEST_ASSERT_EQUAL(128, dimmable._valueAtTick(12000, 0));
}

void test_complete_flag(void) {
  TEST_ASSERT_EQUAL(true, dimmable.complete);
}

void process() {
  UNITY_BEGIN();
  RUN_TEST(test_value_at_tick_fadeup);
  RUN_TEST(test_value_at_tick_fadedown);
  RUN_TEST(test_value_at_tick_low_edge);
  RUN_TEST(test_value_at_tick_high_edge);
}

/** BEGIN ARDUINO SKETCH CODE **/

byte finished = false;

void setup() {
  delay(2000);
  process();
  dimmable.reset();
}

void loop() {
  if (!finished) {
    dimmable.fade(0, 255, 3000);
    finished = dimmable.complete;
  } else {
    RUN_TEST(test_complete_flag);
    UNITY_END();
  }
}
