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

Node<LEDEvent> *current, *nextUp;

// bootstrap the current hour and minute so they can be mocked in the test
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
      l.head->data->isFuture(currentHour, currentMinute)
  ) {
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

void test_current_first_event(void) {
  currentHour = 7;
  currentMinute = 30;
  findCurrent(ll);
  TEST_ASSERT_EQUAL_PTR(am, current->data);
}

void test_current_second_event(void) {
  currentHour = 8;
  currentMinute = 30;
  findCurrent(ll);
  TEST_ASSERT_EQUAL_PTR(midday, current->data);
}

void test_current_last_event(void) {
  currentHour = 23;
  currentMinute = 0;
  findCurrent(ll);
  TEST_ASSERT_EQUAL_PTR(overnight, current->data);
}

void test_current_before_first(void) {
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
  RUN_TEST(test_current_first_event);
  RUN_TEST(test_current_second_event);
  RUN_TEST(test_current_last_event);
  RUN_TEST(test_current_before_first);
  RUN_TEST(test_current_same_hour_before);
  RUN_TEST(test_current_same_hour_after);
  UNITY_END();
}

void loop()
{
}