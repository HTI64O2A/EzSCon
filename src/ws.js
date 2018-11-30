const EE = require('events').EventEmitter;
const http = require('http');
const ws = require('websocket').server;


// create http server for WebSocket.
const server = http.createServer((request, response) => {
    response.writeHead(404);
    response.end();
});

let wsServer;

class ez extends EE {
    constructor(port) {
        super();
        wsServer = new WS({
            httpServer: server
        });

        wsServer.on('request', this.newConnection);
    }

    newConnection(request) {
        const connection = request.accept('', request.origin);
        connection.on('message', this.reciveMessage);
    }

    reciveMessage(payload) {
        if(payload.type !== 'utf8') return;
        let message;
        try {
            message = JSON.parse(payload.utf8Data);
        } catch (e) {
            console.log('Not vaid json.');
        }
        
        // Emitting
        this.emit(message.type, message.message);
    }

}


module.exports = ez;
