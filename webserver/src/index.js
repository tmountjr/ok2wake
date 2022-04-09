import { createApp } from 'vue';
import App from './App.vue';
import Event from './Event.vue';
import Events from './Events.vue';

const app = createApp(App);
app.component('Event', Event);
app.component('Events', Events);
app.mount('#app');
