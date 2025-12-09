const http = require('http');
const fs = require('fs');

let bound = "----romigo29";
let body = `--${bound}\r\n`;
body += 'content-disposition:attachment; name="uploadFile"; filename="MyFile.txt"\r\n';
body += 'content-type:text/plain\r\n\r\n';
body += fs.readFileSync("./MyFile.txt");
body += `\r\n--${bound}--\r\n`;


const options = {
    hostname: 'localhost',
    port: 5000,
    path: '/6',
    method: 'POST',
    headers: {
        'Content-Type': 'multipart/form-data; boundary=' + bound,
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
    })
});

request.on('error', (err) => {
    console.error(err);
});


request.end(body);

