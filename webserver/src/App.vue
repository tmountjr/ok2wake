<template>
  <div id="control">
    <div class="header">
      <div class="home-menu pure-menu pure-menu-horizontal">
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
          <p>Current device time: {{ deviceTime }}</p>
          <p>Current status: {{ deviceStatus }}</p>
          <p>Current timezone: {{ status.tz }}</p>
        </div>
        <div class="pure-u-1">
          <form class="pure-form">
            <fieldset class="inline">
              <select name="tz" id="tz" class="pure-input-1" v-model="currentTimezone">
                <option
                  v-for="tz in timezones"
                  :key="tz"
                  :value="tz"
                  :selected="tz == status.tz"
                >{{ tz }}</option>
              </select>
              <button class="pure-button" @click="updateTz"><i class="fa-solid fa-save"></i></button>
            </fieldset>
          </form>
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

            <Events v-if="events.length > 0" :events="events" />

            <div class="pure-u-1">
              <h2 class="condensed">
                Edit Schedule
                <p class="subtitle">All times are relative to whatever the local timezone is set to.</p>
              </h2>
              <form class="pure-form">
                <fieldset class="inline" v-for="(event,i) in events" :key="`event-edit-${i}`">
                  <legend>
                    Event {{ i }} 
                    <button class="pure-button button-error button-small" @click="removeEvent($event, i)">
                      <i class="fa-solid fa-trash"></i>
                    </button>
                  </legend>
                  <input class="pure-input-1-4" type="number" name="hour" :id="`event-edit-${i}-hour`" v-model.number="event.hour" />
                  <span>h</span>
                  <input class="pure-input-1-4" type="number" name="minute" :id="`event-edit-${i}-minute`" v-model.number="event.minute">
                  <span>m</span>
                  <select class="pure-input-1-4" name="state" :id="`event-edit-${i}-state`" v-model.number="event.state">
                    <option value="1">Wake</option>
                    <option value="2">Sleep</option>
                    <option value="3">Off</option>
                  </select>
                </fieldset>

                <div class="pure-button-group" role="group">
                  <button class="pure-button" @click="addEvent"><i class="fa-solid fa-plus"></i></button>
                  <button class="pure-button" @click="saveEvents"><i class="fa-solid fa-save"></i></button>
                </div>

              </form>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<style lang="scss" scoped>
.home-menu {
  padding: 0.5em;
  box-shadow: 0 1px 1px rgba(0, 0, 0, 0.1);
}

.header {
  position: sticky;
  top: 0;
  background: #fff;

  a:hover, a:focus {
    background: none;
    border: none;
  }
}

.content {
  padding: 1em;
}

.buttonList {
  display: flex;
  flex-wrap: wrap;

  button {
    flex-basis: 100%;
    margin-bottom: 10px;
    font-size: 115%;
  }
}

fieldset.inline {
  display: flex;
  align-items: flex-start;

  span {
    margin: 0 0.5em;
    align-self: center;
  }
}

button.pure-button {
  &.button-small {
    font-size: 75%;
  }

  &.button-error {
    color: white;
    text-shadow: 0 1px 1px rgba(0, 0, 0, 0.2);
    background: rgb(202, 60, 60);
  }
}

h2.condensed p.subtitle {
  margin-top: 0;
  font-size: 60%;
  font-weight: initial;
  font-style: italic;
}
</style>

<script>
import Events from './Events.vue';
import { LEDEvent } from './LEDEvent.model';

const host = "";

const changeState = async (newState) => {
  await fetch(`${host}/status/set?state=${newState}`, {
    method: 'POST',
  });
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
  components: [
    Events,
  ],
  data() {
    return {
      status: {},
      events: [],
      current: {},
      timezones: [],
      currentTimezone: ''
    };
  },
  async mounted() {
    const resp = await fetch('http://worldtimeapi.org/api/timezone');
    const zones = await resp.json();
    this.timezones = zones;
    await this.update();
  },
  methods: {
    async update() {
      const resp = await fetch(`${host}/status`);
      const status = await resp.json();
      this.status.t = status.t;
      this.status.s = status.s;
      this.status.o = status.o;
      this.status.tz = status.tz;
      this.currentTimezone = status.tz;
      await this.currentEvent();
    },
    async updateTz(e) {
      e.preventDefault();
      const resp = await fetch(`${host}/tz/set`, {
        method: 'POST',
        body: JSON.stringify({ tz_name: this.currentTimezone }),
      });
      const data = await resp.json();
      this.status.t = data.t;
      this.status.s = data.s;
      this.status.o = data.o;
      this.status.tz = data.tz;
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
      await fetch(`${host}/reset`, { method: 'POST' });
      await this.update();
    },
    async currentEvent() {
      const resp = await fetch(`${host}/events`);
      const data = await resp.json();
      let { events } = data;
      events = events.map(e => new LEDEvent(e.hour, e.minute, e.state));
      this.events = events;
    },
    translatedStatus(status) {
      return translatedStatus(status);
    },
    removeEvent(e, id) {
      e.preventDefault();
      if (id === 0) {
        this.events = this.events.splice(1);
      } else {
        this.events = [...this.events.slice(0, id), ...this.events.slice(id + 1)];
      }
    },
    addEvent(e) {
      e.preventDefault();
      const now = new Date();
      this.events.push(new LEDEvent(now.getHours(), now.getMinutes(), LEDEvent.LED_STATE_OFF));
    },
    async saveEvents(e) {
      e.preventDefault();
      // sort the events list
      this.events.sort((a, b) => a.secondsSinceMidnight() - b.secondsSinceMidnight());
      await fetch(`${host}/events/set`, {
        method: 'POST',
        body: JSON.stringify(this.events)
      });
      await this.update();
    }
  },
  computed: {
    deviceTime() {
      const { t = null, o = null, tz = null } = this.status;
      if (t && o && tz) {
        /** Correct GMT time */
        const nonOffsetTime = (t - parseInt(o)) * 1000;
        const formatter = new Intl.DateTimeFormat('en-US', {
          dateStyle: 'short',
          timeStyle: 'medium',
          hour12: false,
          timeZone: tz,
        });
        return formatter.format(nonOffsetTime);
      }
      return 'N/A';
    },
    deviceStatus() {
      if ('s' in this.status) {
        return translatedStatus(this.status.s);
      }
      return 'N/A';
    },
  },
}
</script>