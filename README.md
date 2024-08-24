# Plant Watch

**See plant data here:** https://linux.plant-watch.com/

**Plant Watch** is a Node.js application that monitors soil moisture levels for various plants in real-time. It receives sensor data every 30 minutes from ESP32 devices equipped with soil moisture sensors and displays the data using charts powered by Chart.js. The chart holds the last 100 readings so that you can see the change in moisture over time. The app is self hosted on my VPS with PM2 vonfigured to keep it running and restarting upon failure. The inspiration behind this project was to introduce myself to Iot and working with linux/self hosted applications. I have had a ton of fun and frustration getting it to work, and have a lot of plans to expand and optimize it going forward.

## Esp32 set up

The setup of the microcontroller was pretty simple. All that I used was a breadboard power supply unit and soil moisture sensor. Going into the project I knew very little about working with microcontrollers like these and I had a ton of fun tinkering with them. I know there is a better way for these to be configured, and the amount of time they remain powered is limited, even with the esp32's entering deep sleep between readings.

![ESP32 with sensor](images/moisture-sensor.jpeg)

![Calethea](images/plant.jpeg)

## License

This project is licensed under the MIT License. See the LICENSE file for more details.
