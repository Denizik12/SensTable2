/**************************websocket_example.js*************************************************/
var bodyParser = require("body-parser");
const express = require('express'); //express framework to have a higher level of methods
const app = express(); //assign app variable the express class/method
var http = require('http');
var path = require("path");

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());
const server = http.createServer(app);//create a server

const WebSocket = require('ws');
const s = new WebSocket.Server({ server });

var clientDict = [];

console.log("Server started.");

function database(outputId, outputType) {
    var mysql = require('mysql');

    // database configurations
    var con = mysql.createConnection({
        host: "localhost",
        user: "senstable",
        password: "Senstable2",
        database: "senstable"
    });

    // connect to the database
    con.connect(function(err) {
        if (err) throw err;
        var sql = 'SELECT * FROM outputs WHERE id = ' + mysql.escape(outputId);
        // check if outputId already exists.
        con.query(sql, function (err, result) {
          if (err) throw err;
          if (result == ""){
            // if it doesn't exist, it will add the outputId with the outputType to the database
            con.query("INSERT INTO outputs (id, outputType) VALUES ('" + mysql.escape(outputId) + "', " + mysql.escape(outputType) + ")");
          }
        });
    });
}

s.on('connection', function (ws, req) {
    ws.on('message', function (message) {
        try {
            message = JSON.parse(message);
            console.log(message);

            if (message.hasOwnProperty("client")) {
                if (clientDict[message["client"]["id"]] === undefined)
                    clientDict[message["client"]["id"]] = [];

                clientDict[message["client"]["id"]].push(ws);
                database(message["client"]["id"], message["client"]["outputType"]);
            }
        } catch (err) {
            console.error(err);
        }
        if (message.hasOwnProperty("output")) {
            if (!(clientDict[message["output"]["id"]] === undefined)) {
                clientDict[message["output"]["id"]].forEach(client => {
                    client.send(message["output"]["value"]);
                });
            }
        }
    });

    ws.on('close', function () {
        if (!(clientDict === undefined)) {
            clientDict.forEach(id => {
                if (!(id === undefined) && id.includes(ws)) {
                    var index = id.indexOf(ws);
                    id.splice(index, 1);
                }
            });
        }
    });
});

server.listen(5050);