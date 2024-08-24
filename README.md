# Plant Watch

**See plant data here:** https://linux.plant-watch.com/

**Plant Watch** is a Node.js application that monitors soil moisture levels for a few of my house plants. It receives sensor data every 30 minutes from ESP32 devices equipped with soil moisture sensors and displays the data using a chart powered by Chart.js. The chart holds the last 100 readings so that you can see the change in moisture over time. The app itself is hosted on my VPS with PM2 configured to keep it running and restarting upon failure. The inspiration behind this project was to introduce myself to Iot, working with linux, and self hosting applications. I have had a ton of fun and frustration getting it to work, and have a lot of plans to expand and optimize it going forward.

## Esp32 set up

The setup of the microcontroller is pretty simple. All that I used was a breadboard power supply unit and soil moisture sensor. Going into the project I knew very little about working with microcontrollers like these and I had a ton of fun tinkering with them. I know there is a better way for these to be configured, and I am working on ideas to improve them. Their battery life is a point of weakness, and I am looking into other options.

![ESP32 with sensor](images/moisture-sensor.jpeg)

![Calethea](images/plant.jpeg)
