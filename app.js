require("dotenv").config();

const express = require("express");
const http = require("http");
const socketIo = require("socket.io");
const path = require("path");
const app = express();
const server = http.createServer(app);
const io = socketIo(server);
const favicon = require("serve-favicon");

app.use(favicon(path.join(__dirname, "../public", "favicon.ico")));

app.use(express.static(path.join(__dirname, "../public")));

app.use(
    "/node_modules",
    express.static(path.join(__dirname, "../node_modules"))
);

const API_KEY = process.env.API_KEY;

// middleware to check for API key
function checkApiKey(req, res, next) {
    const apiKey = req.headers["x-api-key"];
    if (apiKey !== API_KEY) {
        return res.status(403).send("Forbidden: Invalid API Key");
    }
    next(); // proceed
}

// store last 100 plant readings
const plantReadings = {
    Spathiphyllum: [],
    Philodendron: [],
    Calethea: [],
};

// convert analog value to percentage
function convertToPercentage(analogValue) {
    const minReading = 1150; // fully wet soil
    const maxReading = 2500; // fully dry soil

    // ensure the reading is within the expected range
    if (analogValue < minReading) analogValue = minReading;
    if (analogValue > maxReading) analogValue = maxReading;

    // calculate moisture percentage
    const moisturePercentage =
        ((maxReading - analogValue) / (maxReading - minReading)) * 100;

    return moisturePercentage;
}

// serve webpage
app.get("/", (req, res) => {
    res.sendFile(join(__dirname, "../public/index.html"));
});

// get initial readings
app.get("initialData", (req, res) => {
    res.json(plantReadings);
});

// handle new data received at /data
app.post("/data", checkApiKey, express.json(), (req, res) => {
    const { plant, moisture } = req.body;
    console.log("Data received from ESP32:", req.body);

    if (plantReadings[plant]) {
        const percentageValue = convertToPercentage(moisture);
        const currentTime = new Date().toISOString();
        console.log("Converted to percentage:", percentageValue);

        // ensure only the last 200 readings stored
        if (plantReadings[plant].length >= 100) {
            plantReadings[plant].shift(); // remove the oldest reading
        }
        plantReadings[plant].push({ x: currentTime, y: percentageValue });

        // emit the new reading
        io.emit("newReading", {
            plant,
            value: percentageValue,
            time: currentTime,
        });
    }

    res.status(200).send("Data received");
});

io.on("connection", (socket) => {
    console.log("A user connected");

    // send initial data to the new client
    socket.emit("initialData", plantReadings);
});

server.listen(3000, () => {
    console.log("server running at http://localhost:3000");
});
