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
        <!-- <div class="pure-u-1">
          <h1>Config Options</h1>
          <form class="pure-form">
            <fieldset>
              <legend>Wake State Color</legend>
              <input type="color" name="wake_color" id="wake_color" v-model="wakeColor">
            </fieldset>
            <fieldset>
              <legend>Sleep State Color</legend>
              <input type="color" name="sleep_color" id="sleep_color" v-model="sleepColor">
            </fieldset>
          </form>
        </div> -->
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
</style>

<script>
import * as axios from 'axios';

const host = 'http://192.168.86.191:8080';

export default {
  name: 'control',
  components: [],
  data() {
    return {
      status: {},
      events: [],
      wakeColor: '#00ff00',
      sleepColor: '#ff00ff',
    };
  },
  methods: {
    async update() {
      const resp = await fetch(`${host}/status`);
      const status = await resp.json();
      console.log(status);
      this.status.t = status.t;
      this.status.s = status.s;
    },
    async wake() {
      await axios.post(`${host}/status/set`, 'ledstate=1');
      await this.update();
    },
    async sleep() {
      await axios.post(`${host}/status/set`, 'ledstate=2');
      await this.update();
    },
    async off() {
      await axios.post(`${host}/status/set`, 'ledstate=3');
      await this.update();
    },
    async reset() {
      await axios.post(`${host}/reset`);
      await this.update();
    },
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
        if (this.status.s === 1) return 'Wake mode';
        if (this.status.s === 2) return 'Sleep mode';
        return 'Off';
      }
      return 'N/A';
    }
  },
}
</script>