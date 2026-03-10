const express = require("express");
const WebSocket = require("ws");

const app = express();
const server = app.listen(8080, () => {
    console.log("HTTP server on 8080");
});

const wss = new WebSocket.Server({ server });

let espClient = null;

wss.on("connection", (ws) => {
    console.log("Client connected");

    ws.on("message", (message) => {
        console.log("Received:", message.toString());

        const msg = message.toString();

        if (msg === "ESP_CONNECTED") {
            espClient = ws;
            console.log("ESP registered");
            return;
        }

        // Forward browser command to ESP
        if (espClient && ws !== espClient) {
            espClient.send(msg);
        }
    });

    ws.on("close", () => {
        if (ws === espClient) {
            espClient = null;
        }
    });
});
