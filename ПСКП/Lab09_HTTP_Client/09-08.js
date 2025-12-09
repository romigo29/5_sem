const http = require('http');
const fs = require('fs');
const path = require('path');

const outPath = path.join('./static/file_from_server.jpg');

const options = {
    hostname: 'localhost',
    port: 5000,
    path: '/8',
    method: 'GET'
};

const request = http.request(options, response => {
    console.log('status:', response.statusCode);
    const ws = fs.createWriteStream(outPath);
    response.pipe(ws);
    ws.on('finish', () => console.log('Saved to', outPath));
});

request.on('error', (err) => {
    console.error('Error occured: ', err);
});

request.end();
