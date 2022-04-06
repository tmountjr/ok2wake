#include <Arduino.h>
#include <unity.h>

#define TEST_DIMMABLE_LED
// #define TEST_DIMMABLE_RGBLED
#define TEST_LINKED_LIST

#ifdef TEST_DIMMABLE_LED
#include <DimmableLED.h>

DimmableLED dimmable(16); // onboard LED

void test_value_at_tick_fadeup(void)
{
  dimmable.reset();
  dimmable.fade(0, 255, 10000);
  TEST_ASSERT_EQUAL(0, dimmable._valueAtTick(0, 0));       // start
  TEST_ASSERT_EQUAL(127, dimmable._valueAtTick(5000, 0));  // half
  TEST_ASSERT_EQUAL(255, dimmable._valueAtTick(10000, 0)); // end
}

void test_value_at_tick_fadedown(void)
{
  dimmable.reset();
  dimmable.fade(255, 0, 10000);
  TEST_ASSERT_EQUAL(191, dimmable._valueAtTick(2500, 0)); // 25%
  TEST_ASSERT_EQUAL(63, dimmable._valueAtTick(7500, 0));  // 75%
  TEST_ASSERT_EQUAL(0, dimmable._valueAtTick(10000, 0));  // end
}

void test_value_at_tick_low_edge(void)
{
  dimmable.reset();
  dimmable.fade(128, 64, 10000);
  // If the fade starts at 2000 and ends at 12000, the tick at 1000 should still
  // be 128.
  TEST_ASSERT_EQUAL(128, dimmable._valueAtTick(1000, 2000));
}

void test_value_at_tick_high_edge(void)
{
  dimmable.reset();
  dimmable.fade(64, 128, 10000);
  // If the fade starts at 0 and ends at 10000, the tick at 12000 should be max
  // but not higher.
  TEST_ASSERT_EQUAL(128, dimmable._valueAtTick(12000, 0));
}

void test_complete_flag(void)
{
  TEST_ASSERT_EQUAL(true, dimmable.complete());
}
#endif

#ifdef TEST_DIMMABLE_RGBLED
#include <DimmableRGBLED.h>

DimmableRGBLED led(0, 4, 5);
#endif

#ifdef TEST_LINKED_LIST
#include <LinkedList.h>
#include <LEDEvent.h>

LinkedList<LEDEvent> ll;

LEDEvent *am = new LEDEvent(7, 0, LEDEvent::LED_STATE_WAKE);
LEDEvent *midday = new LEDEvent(8, 0, LEDEvent::LED_STATE_OFF);
LEDEvent *pm = new LEDEvent(18, 30, LEDEvent::LED_STATE_SLEEP);
LEDEvent *overnight = new LEDEvent(19, 0, LEDEvent::LED_STATE_OFF);

Node<LEDEvent> *current, *nextUp;
int currentHour, currentMinute;

// Find the event that is next to fire.
void findCurrent(LinkedList<LEDEvent> l)
{
  current = l.head;
  nextUp = current->next;

  // Edge case: if the first event is future or the last event is past, don't loop
  // because in both cases we use the head of the list.
  if (
      l.tail->data->isPast(currentHour, currentMinute) ||
      l.head->data->isFuture(currentHour, currentMinute))
  {
    current = l.tail;
    nextUp = current->next;
    return;
  }

  // Set the pointers to where current is in the past and next is in the future.
  while (!(current->data->isPast(currentHour, currentMinute) && current->next->data->isFuture(currentHour, currentMinute)))
  {
    current = current->next;
    nextUp = current->next;
  }
}

// Test that the head pointer in the list matches the pointer of the first item.
void test_head(void)
{
  TEST_ASSERT_EQUAL_PTR(am, ll.head->data);
}

// Test that the tail pointer in the list matches the pointer of the last item.
void test_tail(void)
{
  TEST_ASSERT_EQUAL_PTR(overnight, ll.tail->data);
}

// Test that the list is a closed loop, going back to the head after the tail.
void test_loop(void)
{
  TEST_ASSERT_EQUAL_PTR(am, ll.tail->next->data);
}

void test_current_first_event(void)
{
  currentHour = 7;
  currentMinute = 30;
  findCurrent(ll);
  TEST_ASSERT_EQUAL_PTR(am, current->data);
}

void test_current_second_event(void)
{
  currentHour = 8;
  currentMinute = 30;
  findCurrent(ll);
  TEST_ASSERT_EQUAL_PTR(midday, current->data);
}

void test_current_last_event(void)
{
  currentHour = 23;
  currentMinute = 0;
  findCurrent(ll);
  TEST_ASSERT_EQUAL_PTR(overnight, current->data);
}

void test_current_before_first(void)
{
  currentHour = 6;
  currentMinute = 0;
  findCurrent(ll);
  TEST_ASSERT_EQUAL_PTR(overnight, current->data);
}

void test_current_same_hour_before(void)
{
  currentHour = 18;
  currentMinute = 20;
  findCurrent(ll);
  TEST_ASSERT_EQUAL_PTR(midday, current->data);
}

void test_current_same_hour_after(void)
{
  currentHour = 18;
  currentMinute = 40;
  findCurrent(ll);
  TEST_ASSERT_EQUAL_PTR(pm, current->data);
}
#endif

void process()
{
  UNITY_BEGIN();

#ifdef TEST_DIMMABLE_LED
  RUN_TEST(test_value_at_tick_fadeup);
  RUN_TEST(test_value_at_tick_fadedown);
  RUN_TEST(test_value_at_tick_low_edge);
  RUN_TEST(test_value_at_tick_high_edge);
#endif

#ifdef TEST_DIMMABLE_RGBLED

#endif

#ifdef TEST_LINKED_LIST
  ll.add(am);
  ll.add(midday);
  ll.add(pm);
  ll.add(overnight);
  RUN_TEST(test_head);
  RUN_TEST(test_tail);
  RUN_TEST(test_loop);
  RUN_TEST(test_current_first_event);
  RUN_TEST(test_current_second_event);
  RUN_TEST(test_current_last_event);
  RUN_TEST(test_current_before_first);
  RUN_TEST(test_current_same_hour_before);
  RUN_TEST(test_current_same_hour_after);
#endif
}

void setup()
{
delay(2000);
process();
UNITY_END();
}

void loop()
{

}