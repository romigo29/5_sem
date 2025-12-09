// 09-07.js
const http = require('http');
const fs = require('fs');

let bound = "----romigo29";
let body = `--${bound}\r\n`;
body += 'content-disposition:attachment; name="uploadFile"; filename="./MyFile.png"\r\n';
body += 'content-type:application/octet-stream\r\n\r\n';

const options = {
    host: 'localhost',
    path: '/7',
    port: 5000,
    method: 'POST',
    headers: {
        'content-type': 'multipart/form-data; boundary=' + bound,
    }
}


const request = http.request(options, (response) => {
    console.log('Response status code: ', response.statusCode);
    let responseBody = '';
    response.on('data', (chunk) => {
        responseBody += chunk;
    });
    response.on('end', () => {
        console.log('Response body: ', responseBody);
    });
});

request.on('error', (err) => {
    console.error('Error occured: ', err);
});
request.write(body);

let stream = fs.ReadStream('./MyFile.png');

stream.on('data', (chunk) => {
    request.write(chunk);
});
stream.on('end', () => {
    request.end(`\r\n--${bound}--\r\n`);
});