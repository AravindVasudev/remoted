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

// Starts listening when the app is ready
eapp.on('ready', () => {
    app.listen(3000, () => console.log('Listening @ 0.0.0.0:3000'));
});