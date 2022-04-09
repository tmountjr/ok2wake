<template>
  <div class="events">
    <div class="pure-u-1">
      <h2 class="condensed">Current</h2>
      <Event :event="firstEvent" :current="true" />
    </div>
    <div class="pure-u-1">
      <h2 class="condensed">Upcoming</h2>
      <Event v-for="(event, i) in upcomingEvents" :key="`event-${i + 1}`" :event="event" />
    </div>
  </div>
</template>

<style scoped>
h2.condensed {
  margin-top: 0;
}
</style>

<script>
import Event from './Event.vue';

export default {
  name: 'events',
  components: [ Event ],
  props: {
    events: Array,
  },
  computed: {
    firstEvent() {
      return this.events[0];
    },
    upcomingEvents() {
      return this.events.slice(1);
    },
  }
}
</script>