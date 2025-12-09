const http = require('http');
const url = require("url");
const parseString = require('xml2js').parseString;
const mp = require('multiparty');
const fs = require("fs");
const path = require("path");
const querystring = require('querystring');

const WriteNotFound = (response) => {

    response.writeHead(404, { "content-type": "text/html;charset=utf-8" });
    response.end("<h1>404 Not Found</h1>");
    return;
}

const WriteNotAllowed = (response) => {

    response.writeHead(405, { 'content-type': 'text/html;charset=utf-8' });
    response.end("<h1>405 Method not alowed. Use GET requests or POST requests only</h1>");
    return;
}

http.createServer((request, response) => {
    let parsedUrl = url.parse(request.url, true);

    if (request.method === "GET") {
        switch (parsedUrl.pathname) {
            case '/':
                response.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
                response.end(`Task first`);
                break;
            case "/2": {
                response.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
                let x = parsedUrl.query.x;
                let y = parsedUrl.query.y;
                response.end(`Second task x = ${x}, y = ${y}`);
                break;

            }

            case "/8": {

                let file = fs.readFileSync("server_pic.jpg");
                response.writeHead(200, {
                    'Content-Type': 'text/plain',
                    'Content-Disposition': 'attachment; filename="server_pic.jpg"'
                });

                console.log(`file sent`);
                response.end(file);
                break;
            }
            default:
                WriteNotFound(request);
                break;
        }
    }

    else if (request.method === "POST") {
        switch (parsedUrl.pathname) {
            case "/3": {
                let data = '';

                request.on('data', (chunk) => {
                    data += chunk;
                });
                request.on('end', () => {
                    const parsedBody = querystring.parse(data);
                    const x = parsedBody.x;
                    const y = parsedBody.y;
                    const s = parsedBody.s;
                    response.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
                    response.end(`Third task x = ${x}, y = ${y}, s = ${s}`);
                });

                break;
            }
            case "/4":
                let data = '';
                request.on('data', (chunk) => {
                    data += chunk;
                });
                request.on('end', () => {
                    data = JSON.parse(data);
                    response.writeHead(200, { 'Content-type': 'application/json; charset=utf-8' });
                    let responsep = {};
                    responsep.__comment = data.comment;
                    responsep.x_plus_y = data.x + data.y;
                    responsep.Concatenation_s_o = data.s + ': ' + data.o.surname + ', ' + data.o.name;
                    responsep.Length_m = data.m.length;
                    response.end(JSON.stringify(responsep));
                });
                break;
            case "/5": {
                let data = '';
                request.on('data', (chunk) => {
                    data += chunk;
                });
                request.on('end', () => {
                    parseString(data, (err, responseult) => {
                        response.writeHead(200, { 'Content-type': 'application/xml' });
                        let id = responseult.request.$.id;
                        let sum = 0;
                        let concat = '';
                        responseult.request.x.forEach((p) => {
                            sum += parseInt(p.$.value);
                        });
                        responseult.request.m.forEach((p) => {
                            concat += p.$.value;
                        });

                        let responseponseText = `<responseponse id="33" request="${id}"><sum element="x" responseult="${sum}"/><concat element="m" responseult="${concat}"/></responseponse>`;
                        response.end(responseponseText);
                    });
                });
                break;
            }
            case "/6":
            case "/7": {

                let form = new mp.Form({ uploadDir: "./static" });

                form.parse(request, (err, fields, files) => {
                    if (err) {
                        response.writeHead(400, { "content-type": "text/html;charset=utf-8" });
                        response.end("<h1>400 Bad request</h1>");
                        return;
                    }

                    let uploaded_files = files.uploadFile;

                    if (!uploaded_files) {
                        response.writeHead(400, { "content-type": "text/html;charset=utf-8" });
                        response.end("<h1>400 Bad request</h1>");
                        return;
                    }

                    if (!Array.isArray(uploaded_files)) {
                        uploaded_files = [uploaded_files];
                    }

                    uploaded_files.forEach(file => {
                        if (!file || !file.originalFilename) return;

                        let new_file_path = path.join("./static", file.originalFilename);

                        fs.rename(file.path, new_file_path, err => {
                            if (err) {
                                console.error(err);
                            }
                        });
                    });

                    response.writeHead(200, { "content-type": "text/html;charset=utf-8" });
                    response.end("<p>File uploaded</p>");
                });
                break;
            }

            default: {
                WriteNotFound(request);
                break;
            }
        }
    }
    else {
        WriteNotAllowed(request);
    }
}).listen(5000, () => console.log('http://localhost:5000'));