const http = require("http");

let options = {
    host: 'localhost',
    path: '/',
    port: 5000,
    method: 'GET'
}

const req = http.request(options, (resp) => {
    console.log("response status code: " + resp.statusCode);
    console.log("response status message: " + resp.statusMessage);
    console.log("remote address: " + resp.socket.remoteAddress);
    console.log("remote port: " + resp.socket.remotePort);

    let data = '';

    resp.on('data', (chunk) => {
        data += chunk.toString('utf8');
    });

    resp.on('end', () => {
        console.log(data);
    });
});

req.on('error', (e) => {
    console.log("http request error: " + e.message);
})
req.end();