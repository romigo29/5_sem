var http = require('http')
const readline = require('readline');

const PORT = 5000;
const validStates = ['norm', 'stop', 'test', 'idle'];
let currentAppState = 'norm';

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
})

const changeState = () => {
    rl.question(`${currentAppState}->`, (input) => {

        input = input.trim().toLowerCase();

        if (validStates.includes(input)) {
            console.log(`reg=${currentAppState}->${input}`);
            currentAppState = input;
        }

        else if (input == 'exit') {
            console.log("Exiting program...");
            rl.close();
            process.exit(0);
        }

        else {
            console.log('Invalid Value.\n Valid options: norm, stop, test, idle, exit')
        }

        changeState();

    });
}

http.createServer(function (request, response) {

    if (request.url === '/' && request.method === 'GET') {
        response.writeHead(200, { 'content-type': 'text/html; charset=utf-8' });
        response.end(`<html>
                    <head><title>App State</title></head>
                    <body>
                    <h1>${currentAppState}</h1>
                    </body>
                    </html>`);
    } else {
        response.writeHead(404, { 'content-type': 'text/plain' });
        response.end('Not found');
    }
}).listen(PORT);

console.log(`Server running at http://localhost:${PORT}/`);
changeState();


