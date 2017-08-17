import { app as eapp } from 'electron';
import express from 'express';
import { spawn } from 'child_process';
import robot from 'robotjs';
const app = express();

// test route
app.get('/', (req, res) => res.send('Working!'));

// shutdown
app.get('/shutdown', (req, res) => {
    if (process.platform === 'linux' || process.platform === 'darwin') {
        spawn('shutdown', ['now']);
    } else if (process.platform === 'win32') {
        spawn('shutdown', ['-s', '-t', '0']);
    } else {
        res.send('unknown operating system :(');
    }
});

// open link
app.get('/link', (req, res) => {
    let url = req.query.url || 'https://google.com/';
    if (process.platform === 'linux') {
        spawn('xdg-open', [url])
    } else if (process.platform === 'win32') {
        spawn('start', [url]);
    } else if (process.platform === 'darwin') {
        spawn('open', [url]);
    }

    res.send('Opened.');
});

// move mouse in sine form
app.get('/mouse/sine', (req, res) => {
  // Speed up the mouse.
  robot.setMouseDelay(2);

  let twoPI = Math.PI * 2.0;
  let screenSize = robot.getScreenSize();
  let height = (screenSize.height / 2) - 10;
  let width = screenSize.width;

  for (let x = 0; x < width; x++) {
    let y = height * Math.sin((twoPI * x) / width) + height;
    robot.moveMouse(x, y);
  }

  res.send('done.');
});

// move mouse in a circle
app.get('/mouse/circle', (req, res) => {
  // Speed up the mouse.
  robot.setMouseDelay(2);

  // get screen measurements
  let screenSize = robot.getScreenSize();
  let center = {
    x: screenSize.width / 2,
    y: screenSize.height / 2
  };
  let radius = (screenSize.height / 2) - 10;;

  // draw circle
  for (let a = 0; a < 360; a++) {
    let radians = a * (Math.PI / 180);
    let x = center.x + radius * Math.cos(radians);
    let y = center.y + radius * Math.sin(radians);

    robot.moveMouse(x, y);
  }

  res.send('done.');
});

// presses alt + F4
app.get('/close', (req, res) => {
  if (process.platform === 'darwin') {
    robot.keyTap('q', 'command');
  } else {
    robot.keyTap('f4', 'alt');
  }
  res.send('done.');
});

// Starts listening when the app is ready
eapp.on('ready', () =>
    app.listen(3000, () => console.log('Listening @ 0.0.0.0:3000')));