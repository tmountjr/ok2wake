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
const defaultEvents = [preAm, am, midday];

function findCurrent() {
  if (defaultEvents[defaultEvents.length - 1].isPast() || defaultEvents[0].isFuture()) return defaultEvents[defaultEvents.length - 1];
  for (let i = 0; i < defaultEvents.length; i++) {
    if (defaultEvents[i].isPast() && defaultEvents[i + 1].isFuture()) return defaultEvents[i];
  }
  return defaultEvents[0];
}

// stub some additional api routes to serve static data
app.get('/events', (req, res) => {
  res.json({
    events: defaultEvents
  });
});

app.post('/status/set', (req, res) => {
  console.log(req.body);
  // TODO: figure out how this looks coming from the frontend
  res.json({});
});

app.get('/status', (req, res) => {
  const current = findCurrent();
  res.json({
    t: Math.floor((new Date()).valueOf() / 1000),
    s: current.status
  });
});

app.post('/reset', (req, res) => {
  res.sendStatus(200);
});

const listener = app.listen(port, () => console.log(`WebPack dev server listening on port ${port}.\n`));

module.exports = listener;