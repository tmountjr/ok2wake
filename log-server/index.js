const express = require('express');
const bodyParser = require('body-parser');

const app = express();
const port = 3000;

app.use(bodyParser.json());

app.post('/log', (req, res) => {
  console.log(new Date().toString(), req.body.message);
  res.sendStatus(200);
});

app.listen(port, () => console.log(`API listening on port ${port}.`));
