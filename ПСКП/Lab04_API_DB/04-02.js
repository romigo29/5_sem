var http = require('http');
var url = require('url');
var fs = require('fs');
var data = require('./db');

const PORT = 5000;
const PATH = '/api/db'

var db = new data.DB();

db.on('GET', async (request, response) => {
    console.log('DB.GET');
    response.writeHead(200, { 'content-type': 'application/json;charset=utf-8' });
    response.end(JSON.stringify(db.select()))

});

db.on('POST', async (request, response) => {
    console.log('DB.POST');
    request.on('data', data => {
        let row = JSON.parse(data);
        db.insert(row);
        response.writeHead(200, { 'content-type': 'application/json;charset=utf-8' });
        response.end(JSON.stringify(db.select()))
    })
});


db.on('PUT', async (request, response) => {
    console.log('DB.PUT');
    request.on('data', data => {
        let row = JSON.parse(data);
        db.update(row);
        response.writeHead(200, { 'content-type': 'application/json;charset=utf-8' });
        response.end(JSON.stringify(db.select()))
    })
});

db.on('DELETE', async (request, response) => {
    console.log('DB.DELETE');
    let parsedURL = url.parse(request.url, true);
    const id = parsedURL.query.id;

    if (!id) {
        response.writeHead(400, { 'content-type': 'text/html;charset=utf-8' });
        response.end('<h1>400 Bad request</h1>')
    }

    const deletedRow = await db.delete(id);
    response.writeHead(200, { 'content-type': 'application/json;charset=utf-8' });
    response.end(JSON.stringify(deletedRow));
});

http.createServer(function (request, response) {

    if (url.parse(request.url).pathname === PATH) {
        db.emit(request.method, request, response);
    }

    else if (request.url === '/') {
        let html = fs.readFileSync('./index.html');
        response.writeHead(200, { 'content-type': 'text/html;charset=utf-8' });
        response.end(html);
    }

    else {
        response.writeHead(404, { 'content-type': 'text/plain; charset=utf-8' });
        response.end('Not found');
    }
}).listen(PORT);

console.log(`Server running at http://localhost:${PORT}${PATH}`);


