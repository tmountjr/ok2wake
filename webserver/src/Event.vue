<template>
  <div class="event">
    <p>
      {{ prefix }} {{ formattedTime }} - {{ translatedStatus }}
    </p>
  </div>
</template>

<script>
import { LEDEvent } from './LEDEvent.model';

export default {
  name: 'event',
  components: [],
  props: {
    event: LEDEvent,
    current: Boolean,
  },
  computed: {
    prefix() {
      return this.current
        ? 'Since'
        : 'At'
    },
    formattedTime() {
      return `${('00' + this.event.hour).slice(-2)}:${('00' + this.event.minute).slice(-2)}`;
    },
    translatedStatus() {
      switch (this.event.state) {
        case 1:
          return 'Wake state';
        case 2:
          return 'Sleep state';
        case 3:
          return 'Off';
        default:
          return 'N/A';
      }
    },
  },
};
</script>
