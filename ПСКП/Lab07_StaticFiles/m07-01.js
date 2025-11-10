const fs = require('fs');
const path = require('path');

function Stat(staticFolder = './static') {
    this.STATIC_FOLDER = staticFolder;

    let pathStatic = (fn) => path.join(this.STATIC_FOLDER, fn);

    this.writeHTTP404 = (res) => {
        res.statusCode = 404;
        res.statusMessage = 'Resource not found';
        res.end("404: Resource not found");
    };

    let pipeFile = (filePath, res, headers) => {
        res.writeHead(200, headers);
        fs.createReadStream(filePath).pipe(res);
    };


    this.isStatic = (ext, fn) => {
        let reg = new RegExp(`\\.${ext}$`);
        return reg.test(fn);
    };

    this.sendFile = (req, res, headers, customPath) => {
        const filePath = customPath ? pathStatic(customPath) : pathStatic(req.url);

        fs.access(filePath, fs.constants.R_OK, err => {
            if (err) this.writeHTTP404(res);
            else pipeFile(filePath, res, headers);
        });
    };
}

module.exports = (parm) => new Stat(parm);
