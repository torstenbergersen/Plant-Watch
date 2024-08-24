const express = require("express");
const http = require("http");
const socketIo = require("socket.io");
const path = require("path");
const app = express();
const server = http.createServer(app);
const io = socketIo(server);

app.use(express.static(path.join(__dirname, "../public")));

// store last 100 plant readings
const plantReadings = {
  Spathiphyllum: [],
  Philodendron: [],
  Calethea: [],
};

// convert analog value to percentage
function convertToPercentage(analogValue) {
  const minReading = 1500; // 100% moisture (fully submerged)
  const maxReading = 3370; // 0% moisture (open air)

  // ensure the reading is within the expected range
  if (analogValue < minReading) analogValue = minReading;
  if (analogValue > maxReading) analogValue = maxReading;

  // calculate moisture percentage
  const moisturePercentage =
    ((analogValue - maxReading) / (minReading - maxReading)) * 100;

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
app.post("/data", express.json(), (req, res) => {
  const { plant, moisture } = req.body;
  console.log("Data received from ESP32:", req.body);

  if (plantReadings[plant]) {
    const percentageValue = convertToPercentage(moisture);
    const currentTime = new Date().toISOString();
    console.log("Converted to percentage:", percentageValue);

    // ensure only the last 100 readings stored
    if (plantReadings[plant].length >= 100) {
      plantReadings[plant].shift(); // remove the oldest reading
    }
    plantReadings[plant].push({ x: currentTime, y: percentageValue });

    // emit the new reading
    io.emit("newReading", { plant, value: percentageValue, time: currentTime });
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
