const express = require("express");
const app = express();
const sqlite3 = require("sqlite3").verbose();

app.get("/", (req, res) => {
  res.render("index", { text: "world" });
});

app.listen(3000);
