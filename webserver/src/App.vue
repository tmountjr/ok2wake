<template>
  <div id="control">
    <div class="header">
      <div class="home-menu pure-menu pure-menu-horizontal pure-menu-fixed">
        <a href="" class="pure-menu-heading">ok2wake Control</a>
      </div>
    </div>

    <div class="content">
      <div class="pure-g">
        <div class="pure-u-1">
          <button class="pure-button" @click="update">Refresh Device Data</button>
        </div>
      </div>
      <div class="pure-g">
        <div class="pure-u-1">
          <h1>Current Status</h1>
          <p>Current device time: {{ deviceTime }} </p>
          <p>Current status: {{ deviceStatus }}</p>
        </div>
        <div class="pure-u-1">
          <h1>Update Status</h1>
          <div class="buttonList">
            <button class="pure-button" @click="wake">Set to "Wake" mode</button>
            <button class="pure-button" @click="sleep">Set to "Sleep" mode</button>
            <button class="pure-button" @click="off">Set to "Off" mode</button>
            <button class="pure-button" @click="reset">Reset to default</button>
          </div>
        </div>
        <div class="pure-u-1">
          <h1>Events</h1>

          <div class="pure-g">
            <div class="pure-u-1" v-if="events.length == 0">
              <p>Click "Refresh Device Data" above to populate lists.</p>
            </div>
            <div class="pure-u-1" v-if="'hour' in current">
              <h2 class="condensed">Current</h2>
              <p v-if="'hour' in current">Since {{ `${('00' + current.hour).slice(-2)}:${('00' + current.minute).slice(-2)}` }} - {{ translatedStatus(current.ledstate) }}</p>
            </div>
            <div class="pure-u-1" v-if="events.length">
              <h2 class="condensed">Upcoming</h2>
              <p v-for="(event, i) in events" :key="`event-${i}`">At {{ `${('00' + event.hour).slice(-2)}:${('00' + event.minute).slice(-2)}` }} - {{ translatedStatus(event.ledstate) }}</p>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.home-menu {
  padding: 0.5em;
  box-shadow: 0 1px 1px rgba(0, 0, 0, 0.1);
}

header a:hover,
header a:focus {
  background: none;
  border: none;
}

.content {
  padding: 4em 1em 3em;
}

.buttonList {
  display: flex;
  flex-wrap: wrap;
}

.buttonList button {
  flex-basis: 100%;
  margin-bottom: 10px;
  font-size: 115%;
}

h2.condensed {
  margin-top: 0;
}
</style>

<script>
import * as axios from 'axios';

const host = "";

const changeState = async (newState) => {
  await axios.post(`${host}/status/set`, `ledstate=${newState}`);
};

const translatedStatus = (status) => {
  switch (status) {
    case 1:
      return 'Wake mode';
    case 2:
      return 'Sleep mode';
    case 3:
      return 'Off';
    default:
      return 'N/A';
  }
};

export default {
  name: 'control',
  components: [],
  data() {
    return {
      status: {},
      events: [],
      current: {},
    };
  },
  methods: {
    async update() {
      const resp = await fetch(`${host}/status`);
      const status = await resp.json();
      this.status.t = status.t;
      this.status.s = status.s;
      await this.currentEvent();
    },
    async wake() {
      await changeState(1);
      await this.update();
    },
    async sleep() {
      await changeState(2);
      await this.update();
    },
    async off() {
      await changeState(3);
      await this.update();
    },
    async reset() {
      await axios.post(`${host}/reset`);
      await this.update();
    },
    async currentEvent() {
      const resp = await axios.get(`${host}/events`);
      const { events } = resp.data;
      const c = events.shift();
      this.current = c;
      this.events = events;
    },
    translatedStatus(status) {
      return translatedStatus(status);
    }
  },
  computed: {
    deviceTime() {
      if ('t' in this.status) {
        // The time is offset by four hours in the c++ code to allow for the use
        // of local times for events. Unfortunately when it reports the epoch time
        // it also adds in that offset. So here we have to remove the offset first,
        // then turn into milliseconds.
        const nonOffsetTime = (this.status.t + (4 * 60 * 60)) * 1000;
        return new Date(nonOffsetTime).toLocaleString();
      }
      return 'N/A';
    },
    deviceStatus() {
      if ('s' in this.status) {
        // return this.translatedStatus(this.status.s);
        return translatedStatus(this.status.s);
      }
      return 'N/A';
    },
  },
}
</script>