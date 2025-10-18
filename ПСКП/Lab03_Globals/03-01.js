const http = require("http");

const validStates = ["norm", "stop", "test", "idle", "exit"];
let currentState = "norm";

http.createServer(function (request, response) {
    response.writeHead("200", { "content-type": "text/html; charset=utf-8" });
    response.end(`<h1>${currentState}<h1>`);
}).listen(5000, () => {
    console.log("Server running at http://localhost:5000/");

});

process.stdin.setEncoding("utf-8");
process.stdin.on("readable", () => {
    
    let chunk = null;
    while ((chunk = process.stdin.read()) != null) {
        let trimmedInput = chunk.trim();

        if (validStates.includes(trimmedInput)) {
            process.stdout.write(`${currentState} -> ${trimmedInput}\n`);

            if (trimmedInput === "exit") {
                process.exit(0);
            }
            else {
                currentState = trimmedInput;
            }
        }
        else {
            process.stdout.write(`Неверное состояние: ${trimmedInput}\n`);
        }
    }
});

