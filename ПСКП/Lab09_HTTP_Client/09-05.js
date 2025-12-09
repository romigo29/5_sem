const http = require('http');
let xmlbuilder = require('xmlbuilder');

let xmldoc = xmlbuilder.create('request').att('id', 33);
xmldoc.ele('x').att('value', 3);
xmldoc.ele('x').att('value', 1);
xmldoc.ele('x').att('value', 2);
xmldoc.ele('m').att('value', 'a');
xmldoc.ele('m').att('value', 'b');
xmldoc.ele('m').att('value', 'c');


const options = {
    hostname: 'localhost',
    port: 5000,
    path: '/5',
    method: 'POST',
    headers: {
        'Content-Type': 'application/xml',
        'Accept': 'application/xml'
    }
};

const request = http.request(options, (response) => {
    let data = '';

    console.log(`Статус ответа: ${response.statusCode}`);

    response.on('data', (chunk) => {
        data += chunk;
    });

    response.on('end', () => {
        console.log('Данные ответа:');
        console.log(data);
    });
});

request.on('error', (error) => {
    console.error(`Ошибка запроса: ${error.message}`);
});

request.write(xmldoc.toString({pretty: true}));
request.end();