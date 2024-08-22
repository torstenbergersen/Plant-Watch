const express = require("express");
const http = require("http");
const socketIo = require("socket.io");
const { join } = require("path");
const app = express();
const server = http.createServer(app);
const io = socketIo(server);
const path = require("path");

app.use(express.static(path.join(__dirname, "../public")));

// store last 100 plant readings
const plantReadings = {
  Spathiphyllum: [],
  Philodendron: [],
  Calethea: [],
};

// serve webpage
app.get("/", (req, res) => {
  res.sendFile(join(__dirname, "../public/index.html"));
});

// get initial readings
app.get("initialData", (req, res) => {
  res.json(plantReadings);
});

// Socket.io connection handling
io.on("connection", (socket) => {
  console.log("A client connected");
  // send initial data to the newly connected client
  socket.emit("initialData", plantReadings);
});

// convert and display moisture data

server.listen(3000, () => {
  console.log("server running at http://localhost:3000");
});
