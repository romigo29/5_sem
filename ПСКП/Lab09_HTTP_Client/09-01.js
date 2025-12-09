const http = require("http");

let options = {
    host: 'localhost',
    path: '/',
    port: 5000,
    method: 'GET'
}

const request = http.request(options, (response) => {
    console.log("response status code: " + response.statusCode);
    console.log("response status message: " + response.statusMessage);
    console.log("remote address: " + response.socket.remoteAddress);
    console.log("remote port: " + response.socket.remotePort);

    let data = '';

    response.on('data', (chunk) => {
        data += chunk.toString('utf8');
    });

    response.on('end', () => {
        console.log(data);
    });
});

request.on('error', (e) => {
    console.log("http request error: " + e.message);
})
request.end();