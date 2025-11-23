const http = require('http');
const url = require("url");
const parseString = require('xml2js').parseString;
const mp = require('multiparty');
const fs = require("fs");
const querystring = require('querystring');

function write200Html(response, text = "") {
    response.writeHead(200, { "content-type": "text/html; charset=utf-8" });
    response.end(text);
}



http.createServer((req, res) => {
    let parsedUrl = url.parse(req.url, true);
    let form = new mp.Form({ uploadDir: './static' });

    switch (parsedUrl.pathname) {
        case '/':
            res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
            res.end(`Task first`);
            break;
        case "/2": {
            res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
            let x = parsedUrl.query.x;
            let y = parsedUrl.query.y;
            res.end(`Second task x = ${x}, y = ${y}`);
            break;

        }
        case "/3": {
            let data = '';

            req.on('data', (chunk) => {
                data += chunk;
            });
            req.on('end', () => {
                const parsedBody = querystring.parse(data);
                const x = parsedBody.x;
                const y = parsedBody.y;
                const s = parsedBody.s;
                res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
                res.end(`Third task x = ${x}, y = ${y}, s = ${s}`);
            });

            break;
        }
        case "/4":
            let data = '';
            req.on('data', (chunk) => {
                data += chunk;
            });
            req.on('end', () => {
                data = JSON.parse(data);
                res.writeHead(200, { 'Content-type': 'application/json; charset=utf-8' });
                let resp = {};
                resp.__comment = data.comment;
                resp.x_plus_y = data.x + data.y;
                resp.Concatenation_s_o = data.s + ': ' + data.o.surname + ', ' + data.o.name;
                resp.Length_m = data.m.length;
                res.end(JSON.stringify(resp));
            });
            break;
        case "/5": {
            let data = '';
            req.on('data', (chunk) => {
                data += chunk;
            });
            req.on('end', () => {
                parseString(data, (err, result) => {
                    res.writeHead(200, { 'Content-type': 'application/xml' });
                    let id = result.req.$.id;
                    let sum = 0;
                    let concat = '';
                    result.req.x.forEach((p) => {
                        sum += parseInt(p.$.value);
                    });
                    result.req.m.forEach((p) => {
                        concat += p.$.value;
                    });

                    let responseText = `<response id="33" req="${id}"><sum element="x" result="${sum}"/><concat element="m" result="${concat}"/></response>`;
                    res.end(responseText);
                });
            });
            break;
        }
        case "/6":
        case "/7":

            if (req.method == "POST") {
                const form = new mp.Form({ uploadDir: './static' });

                form.on('file', (name, file) => {
                    console.log(`filename: ${name} = ${file.originalFilename} in ${file.path}`);
                });

                form.on('error', err => {
                    if (!res.writableEnded) {
                        res.writeHead(500, { 'Content-Type': 'text/html; charset=utf-8' });
                        res.end(`<p>form returned error: ${err}</p>`);
                    }
                });

                form.on('close', () => {
                    if (!res.writableEnded) {
                        res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
                        res.end('<p>File uploaded</p>');
                    }
                });

                form.parse(req);

            }
            else {
                res.writeHead(405, { 'Content-Type': 'text/plain; charset=utf-8' });
                res.end('405 Method Not Allowed');
            }
            break;


        case "/8":
            res.writeHead(200, { 'Content-Type': 'text/html' });
            let file = fs.readFileSync("server_pic.jpg");
            console.log(`sended file`);
            res.end(file);
            break;
    }
}).listen(5000, () => console.log('http://localhost:5000'));