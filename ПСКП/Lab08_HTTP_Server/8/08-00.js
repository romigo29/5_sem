const http = require("http");
const url = require("url");
const fs = require("fs");
const path = require("path");
const { parse } = require("querystring");
const xml2js = require("xml2js");
const xmlBuilder = require("xmlbuilder");
const mp = require("multiparty");

function write405(response) {
  response.writeHead(405, { "content-type": "text/plain; charset=utf-8" });
  response.end("Method not allowed");
}

function write200Html(response, text = "") {
  response.writeHead(200, { "content-type": "text/html; charset=utf-8" });
  response.end(text);
}

const MIME = {
  ".txt": "text/plain",
  ".html": "text/html",
  ".css": "text/css",
  ".js": "text/javascript",
  ".png": "image/png",
  ".docx": "application/msword",
  ".json": "application/json",
  ".xml": "application/xml",
  ".mp4": "video/mp4",
};

function handleConnection(request, response) {
  const parsedUrl = url.parse(request.url, true);
  if (request.method !== "GET") {
    return write405(response);
  }
  if (parsedUrl.query.set === undefined) {
    write200Html(response, `<h1>${server.keepAliveTimeout} ms</h1>`);
  } else {
    server.keepAliveTimeout = +parsedUrl.query.set;
    console.log(`установлено новое значение параметра KepAliveTimeout=${server.keepAliveTimeout}`);
    write200Html(response, "<h1>OK</h1>");
  }
}

function handleHeaders(request, response) {
  if (request.method !== "GET") {
    write405(response);
  }
  let headers = "<h3>Request headers:</h3>";
  for (let i in request.headers) {
    headers += `<p>${i}: ${request.headers[i]}</p>`;
  }

  headers += "</br><h3>Response headers:</h3>";
  response.setHeader("Content-Type", "text/html; charset=utf-8");
  response.setHeader("X-User-Header", "value");

  const resHeaders = response.getHeaders();
  for (let i in resHeaders) {
    headers += `<p>${i}: ${resHeaders[i]}</p>`;
  }

  write200Html(response, headers);
}

function handleParameter(request, response) {
  if (request.method !== "GET") {
    return write405(response);
  }

  const parsedUrl = url.parse(request.url, true);

  let x, y;
  if (parsedUrl.query && parsedUrl.query.x !== undefined && parsedUrl.query.y !== undefined) {
    x = parsedUrl.query.x;
    y = parsedUrl.query.y;
  } else {
    const parts = parsedUrl.pathname.split("/");
    if (parts.length >= 4) {
      x = parts[2];
      y = parts[3];
    } else {
      return write200Html(response, "<h2>Ошибка: параметры x и y не указаны</h2>");
    }
  }

  if (isNaN(Number(x)) || isNaN(Number(y))) {
    if (parsedUrl.query && (parsedUrl.query.x !== undefined || parsedUrl.query.y !== undefined)) {
      return write200Html(response, "<h2>Ошибка: x и y должны быть числовыми значениями</h2>");
    } else {
      return write200Html(response, `<h2>Некорректные значения. URI: ${request.url}</h2>`);
    }
  }

  const div = y === 0 ? "деление на ноль" : Math.floor(x / y);

  const html = `
    <h3>Анализ параметров</h3>
    <p>x = ${x}</p>
    <p>y = ${y}</p>
    <hr>
    <p>Сумма: ${+x + +y}</p>
    <p>Разность: ${x - y}</p>
    <p>Произведение: ${x * y}</p>
    <p>Частное: ${div}</p>
  `;
  write200Html(response, html);
}

function handleClose(request, response) {
  if (request.method !== "GET") {
    return write405(response);
  }
  setTimeout(() => {
    server.close(() => console.log("Сервер остановлен спустя 10 секунд"));
  }, 10000);
  write200Html(response, "<p>Сервер будет остановлен через 10 секунд</p>");
}

function handleSocket(request, response) {
  if (request.method !== "GET") {
    return write405(response);
  }

  let html = "<h3>Client:</h3>";
  html += `<p>IP = ${request.socket.remoteAddress}</p>`;
  html += `<p>Port = ${request.socket.remotePort}</p>`;

  html += "<h3>Server:</h3>";
  html += `<p>IP = ${request.socket.localAddress}</p>`;
  html += `<p>Port = ${request.socket.localPort}</p>`;
  write200Html(response, html);
}

function handleReqData(request, response) {
  if (request.method !== "GET") {
    return write405(response);
  }

  const CHUNK_SIZE = 1024 * 8;
  const chunks = [];
  let total = 0;

  request.on("readable", () => {
    let buf;
    while (null != (buf = request.read(CHUNK_SIZE))) {
      console.log(`request.read(): ${buf.length} bytes`);
      chunks.push(buf);
      total += buf.length;
    }
  });

  request.on("end", () => {
    const body = total ? Buffer.concat(chunks, total).toString("utf8") : "";
    write200Html(response, `<p>got = ${body.length}</p><p>Received data: ${body}</p>`);
  });

  request.on("error", (err) => {
    console.error("request error:", err);
    if (!response.headersSent) response.writeHead(500);
    response.end("internal server error");
  });
}

function handleRespStatus(request, response) {
  if (request.method !== "GET") {
    return write405(response);
  }
  const parsedUrl = url.parse(request.url, true);
  const code = parsedUrl.query.code;
  const mess = parsedUrl.query.mess;

  response.writeHead(+code, mess);
  response.end();
}

function handleFormParameter(request, response) {
  if (request.method === "GET") {
    let data = fs.readFileSync("form.html");
    write200Html(response, data);
  } else if (request.method === "POST") {
    let body = "";

    request.on("data", (chunk) => {
      body += chunk.toString();
    });

    request.on("end", () => {
      let param = parse(body);
      response.end(`<p>Text: ${param.inputText} </p>
                    <p>Number: ${param.inputNumber}</p>
                    <p>Date: ${param.inputDate}</p>
                    <p>Checked checkbox(s): ${param.inputCheck}</p>
                    <p>Selected radiobutton: ${param.inputRadio}</p>
                    <p>Textarea: ${param.inputTextArea}</p>
                    <p>Submit: ${param.inputSubmitForm}</p>`);
    });
  } else {
    write405(response);
  }
}

function handleJson(request, response) {
  if (request.method !== "POST") {
    return write405(response);
  }
  response.writeHead(200, { "Content-Type": "application/json; charset=utf-8" });
  let data = "";

  request.on("data", (chunk) => {
    data += chunk.toString();
  });

  request.on("end", () => {
    data = JSON.parse(data);
    let answer = {};
    answer.__comment = data.__comment;
    answer.x_plus_y = data.x + data.y;
    answer.Concatination_s_o = `${data.s}: ${data.o.surname}, ${data.o.name}`;
    answer.Length_m = data.m.length;
    response.end(JSON.stringify(answer));
  });
}

function handleXml(request, response) {
  if (request.method !== "POST") {
    return write405(response);
  }
  let data = "";

  request.on("data", (chunk) => {
    data += chunk.toString();
  });

  request.on("end", () => {
    xml2js.parseString(data, (error, result) => {
      if (error) {
        response.writeHead(400, { "Content-Type": "text/xml; charset=utf-8" });
        response.end(`<result>некорректная структура xml</result>`);
        return;
      }

      let sum = 0;
      let mess = "";

      result.request.x.forEach((elem) => {
        sum += Number.parseInt(elem.$.value);
      });

      result.request.m.forEach((elem) => {
        mess += elem.$.value;
      });

      let xml = xmlBuilder
        .create("response")
        .att("id", +result.request.$.id + result.request.x.length + result.request.m.length)
        .att("request", result.request.$.id);
      xml.ele("sum", { element: "x", sum: `${sum}` });
      xml.ele("concat", { element: "m", result: `${mess}` });
      let rc = xml.toString({ pretty: true });

      response.writeHead(200, { "Content-Type": "text/xml; charset=utf-8" });
      response.end(rc);
    });
  });
}

function handleFiles(request, response) {
  if (request.method !== "GET") {
    return write405(response);
  }
  const parsedUrl = url.parse(request.url);
  const parts = parsedUrl.pathname.split("/").filter((s) => s.trim().length !== 0);

  if (parts.length !== 2) {
    fs.readdir("./static", (err, files) => {
      if (err) {
        response.writeHead(500, { "Content-Type": "text/plain" });
        response.end("Ошибка чтения директории");
        return;
      }

      response.writeHead(200, {
        "Content-Type": "text/plain",
        "X-static-files-count": files.length,
      });

      response.end();
    });
  } else {
    const filename = `./static/${parts[1]}`;
    if (!fs.existsSync(filename)) {
      response.writeHead(404);
      response.end();
    }

    const file = fs.readFileSync(filename);
    const ext = path.extname(filename);

    response.writeHead(200, { "Content-Type": MIME[ext] });
    response.end(file);
  }
}

function handleUpload(request, response) {
  if (request.method === "GET") {
    const file = fs.readFileSync("webform.html");
    write200Html(response, file);
  } else if (request.method === "POST") {
    let form = new mp.Form({ uploadDir: "./static" });

    form.on("file", (name, file) => {
      console.log(`filename: ${name} = ${file.originalFilename} in ${file.path}`);
    });

    form.on("error", (err) => {
      response.writeHead(500, { "Content-Type": "text/html; charset=utf-8" });
      response.end(`<p>form returned error: ${err}</p>`);
    });

    form.on("close", () => {
      write200Html(response, "<p>File uploaded</p>");
    });

    form.parse(request);
  } else {
    write405(response);
  }
}

const server = http.createServer((request, response) => {
  request.readableHighWaterMark = 1024;
  const parsedUrl = url.parse(request.url);
  if (parsedUrl.pathname === "/connection") {
    handleConnection(request, response);
  } else if (parsedUrl.pathname === "/headers") {
    handleHeaders(request, response);
  } else if (parsedUrl.pathname.startsWith("/parameter")) {
    handleParameter(request, response);
  } else if (parsedUrl.pathname.startsWith("/close")) {
    handleClose(request, response);
  } else if (parsedUrl.pathname.startsWith("/socket")) {
    handleSocket(request, response);
  } else if (parsedUrl.pathname.startsWith("/req-data")) {
    handleReqData(request, response);
  } else if (parsedUrl.pathname.startsWith("/resp-status")) {
    handleRespStatus(request, response);
  } else if (parsedUrl.pathname.startsWith("/formparameter")) {
    handleFormParameter(request, response);
  } else if (parsedUrl.pathname.startsWith("/json")) {
    handleJson(request, response);
  } else if (parsedUrl.pathname.startsWith("/xml")) {
    handleXml(request, response);
  } else if (parsedUrl.pathname.startsWith("/files")) {
    handleFiles(request, response);
  } else if (parsedUrl.pathname.startsWith("/upload")) {
    handleUpload(request, response);
  } else {
    write200Html(response, "<h3>Путь не поддерживается</h3>");
  }
});

server.listen(5000, () => console.log("Server running at http://localhost:5000/"));

server.on("connection", (socket) => {
  console.log(`server.on connection ${socket.remoteAddress}:${socket.remotePort}`);
});
