var http = require('http')
var fs = require('fs');

const PORT = 5000;

http.createServer(function (request, response) {

    if (request.url === '/png') {
        const fname = './pic.png';
        let png = null;

        fs.stat(fname, (err, stat) => {
            if (err) {
                console.log('error:', err);
            }
            else {
                png = fs.readFileSync(fname);
                response.writeHead(200, { 'Content-Type': 'image/png', 'Content-Length': stat.size });
                response.end(png, 'binary');
            }
        });
    }
    else {
        response.writeHead(404, { 'Content-Type': 'text/plain' });
        response.end('Not found');
    }
}).listen(PORT);

console.log(`Server running at http://localhost:${PORT}/`);