#include <Arduino.h>
#include <LinkedList.h>
#include <LEDEvent.h>
#include <unity.h>
#include <iostream>
#include <stdexcept>

LinkedList<LEDEvent> ll;

LEDEvent *am = new LEDEvent(7, 0, LEDEvent::LED_STATE_WAKE);
LEDEvent *midday = new LEDEvent(8, 0, LEDEvent::LED_STATE_OFF);
LEDEvent *pm = new LEDEvent(18, 30, LEDEvent::LED_STATE_SLEEP);
LEDEvent *overnight = new LEDEvent(19, 0, LEDEvent::LED_STATE_OFF);

Node<LEDEvent> *nextUp;

// bootstrap the current hour and minute so they can be mocked in the test
int currentHour, currentMinute;

// Find the event that is next to fire.
void findNextUp(LinkedList<LEDEvent> l)
{
  nextUp = l.head;
  LEDEvent *nextUpEvent = nextUp->data;

  // Edge case: if the first event is future or the last event is past, don't loop
  // because in both cases we use the head of the list.
  if (
      l.tail->data->isPast(currentHour, currentMinute) ||
      l.head->data->isFuture(currentHour, currentMinute))
    return;

  while (nextUpEvent->isPast(currentHour, currentMinute))
  {
    nextUp = nextUp->next;
    nextUpEvent = nextUp->data;
  }
}

// Tests

// void setUp(void)
// {
// }

// void tearDown(void)
// {
// }

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

// Test that the system correctly chooses the next event after the initial event has fired.
void test_next_up_midday(void)
{
  currentHour = 9;
  currentMinute = 0;
  findNextUp(ll);
  TEST_ASSERT_EQUAL_PTR(pm, nextUp->data);
}

// Test that the system correctly chooses the next event if the current time is
// after the last defined event.
void test_next_up_after_last(void)
{
  currentHour = 22;
  currentMinute = 0;
  findNextUp(ll);
  TEST_ASSERT_EQUAL_PTR(am, nextUp->data);
}

// Test that the system correctly chooses the next event if the current time is
// before the first defined event.
void test_next_up_before_first(void)
{
  currentHour = 6;
  currentMinute = 0;
  findNextUp(ll);
  TEST_ASSERT_EQUAL_PTR(am, nextUp->data);
}

// Test that the system correctly chooses the next event if the current time is
// in the same hour as but still prior to an event.
void test_next_up_same_hour_before(void)
{
  currentHour = 18;
  currentMinute = 20;
  findNextUp(ll);
  TEST_ASSERT_EQUAL_PTR(pm, nextUp->data);
}

// Test that the system correctly chooses the next event if the current time is
// in the same hour as but after an event.
void test_next_up_same_hour_after(void)
{
  currentHour = 18;
  currentMinute = 40;
  findNextUp(ll);
  TEST_ASSERT_EQUAL_PTR(overnight, nextUp->data);
}

void setup()
{
  delay(2000);
  ll.add(am);
  ll.add(midday);
  ll.add(pm);
  ll.add(overnight);

  UNITY_BEGIN();
  RUN_TEST(test_head);
  RUN_TEST(test_tail);
  RUN_TEST(test_loop);
  RUN_TEST(test_next_up_midday);
  RUN_TEST(test_next_up_after_last);
  RUN_TEST(test_next_up_before_first);
  UNITY_END();
}

void loop()
{
}