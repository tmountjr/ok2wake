const express = require('express');
const webpack = require('webpack');
const webpackDevMiddleware = require('webpack-dev-middleware');
const cors = require('cors');
const bodyParser = require('body-parser');
const logMiddleware = (req, res, next) => {
  const current = (new Date()).toISOString();
  console.log(`[${current}] ${req.method}:${req.url}`);
  next();
}
const { LEDEvent } = require('./src/LEDEvent.model');

const port = 3000;

const app = express();
const webpackConfig = require('./webpack.config');
const compiler = webpack({
  ...webpackConfig,
  mode: 'development',
  devtool: 'eval-source-map',
});

app.use(bodyParser.text());
app.use(bodyParser.json());
app.use(cors());
app.use(logMiddleware);
app.use(
  webpackDevMiddleware(compiler, {
    publicPath: webpackConfig.output.publicPath,
  })
);

const preAm = new LEDEvent(6, 0, LEDEvent.LED_STATE_SLEEP);
const am = new LEDEvent(7, 0, LEDEvent.LED_STATE_WAKE);
const midday = new LEDEvent(8, 0, LEDEvent.LED_STATE_OFF);
let defaultEvents = [preAm, am, midday];
let timezoneName = 'America/New_York'

function findCurrent() {
  if (defaultEvents[defaultEvents.length - 1].isPast() || defaultEvents[0].isFuture()) return defaultEvents[defaultEvents.length - 1];
  for (let i = 0; i < defaultEvents.length; i++) {
    if (defaultEvents[i].isPast() && defaultEvents[i + 1].isFuture()) return defaultEvents[i];
  }
  return defaultEvents[0];
}

// stub some additional api routes to serve static data
app.post('/events/set', (req, res) => {
  // console.log(req.body);
  let inbound = req.body.map(e => new LEDEvent(e.hour, e.minute, e.state));
  defaultEvents = inbound;
  res.status(200).send();
});

app.get('/events', (req, res) => {
  res.json({
    events: defaultEvents
  });
});

app.post('/status/set', (req, res) => {
  console.log(req.body);
  res.status(200).send();
});

app.post('/tz/set', (req, res) => {
  timezoneName = req.body.tz_name;
  const current = findCurrent();
  const now = new Date()
  res.json({
    t: Math.floor(now.valueOf() / 1000),
    s: current.state,
    o: now.getTimezoneOffset() * 60,
    tz: timezoneName,
  });
});

app.get('/status', (req, res) => {
  const current = findCurrent();
  const now = new Date()
  res.json({
    t: Math.floor(now.valueOf() / 1000),
    s: current.state,
    o: now.getTimezoneOffset() * 60,
    tz: timezoneName,
  });
});

app.post('/reset', (req, res) => {
  res.sendStatus(200);
});

const listener = app.listen(port, () => console.log(`WebPack dev server listening on port ${port}.\n`));

module.exports = listener;