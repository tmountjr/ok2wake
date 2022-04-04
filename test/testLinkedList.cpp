#include <Arduino.h>
#include <LinkedList.h>
#include <LEDEvent.h>
#include <unity.h>

// Node<LEDEvent> *nextUp;

LinkedList<LEDEvent> ll;

LEDEvent *am = new LEDEvent(7, 0, LEDEvent::LED_STATE_WAKE);
LEDEvent *midday = new LEDEvent(8, 0, LEDEvent::LED_STATE_OFF);
LEDEvent *pm = new LEDEvent(18, 30, LEDEvent::LED_STATE_SLEEP);
LEDEvent *overnight = new LEDEvent(19, 0, LEDEvent::LED_STATE_OFF);

// Tests

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
  UNITY_END();
}

void loop()
{
}