
# 📡Horizon - Meteorological System based on Internet of things

![Horizon in action](https://raw.githubusercontent.com/SimpleMethod/Horizon/master/doc/Horizon.jpg)

# 🥰 Best engineering thesis at university of technology in faculty of Electrical Engineering, Automatics and Computer Science 🥰
https://weaii.tu.kielce.pl/2020/05/15/wyniki-konkursu-prac-dyplomowych-ieee-2020/

# 🧗‍♂️Main milestones

1. Development of a device for measuring atmospheric conditions.
2. Development of a device for receiving telemetry and sending it to IoT server. 
3. Development of an IoT server able to collect data and analyse.
4. Development of an instrument for wind direction mapping.  

# 🚀 Features
 1. Getting meteorological data such as:
 - Air temperature,
 - Atmospheric pressure,
- Air humidity,
- Soil moisture,
- Wind direction,
- Dust value 2.5 PM and 10 PM.
2.  Communication between weather stations and base stations using IEEE 802.11 standard.
3.  Sending received data to IoT server via GSM communication from base stations.
4.  Ability to view data via a graphical interface from any device.
5. IoT server aggregating and analyzing data. 

# 🗺️ Technologies used in project 
1. Weather station:
- Based on two microcontainers:  STM32 and ESP32 programmed in C++. Development of necessary libraries based on technical documentation,
- Used LTC415 as voltage monitor,
- Photovoltaic panels,
- Voltage regulator,
- Gel rechargeable battery.
2. Receiving station:
- Used ESP32  programmed in C++. Development of necessary libraries based on technical documentation,
- Used SIM700e as GSM modem.
3. IoT server:
- Used microservice architecture,
- Based completely on AWS services (AWS Lambda, Amazon DynamoDB, API gateway, IAM), Lambda programmed in Node.JS,
- Graphical interface based on AngularJS running in one page. 

# 🧪 Visualization of weather station PCB 
![Visualization of weather station PCB ](https://raw.githubusercontent.com/SimpleMethod/Horizon/master/doc/M_Top.png)
## 🧶  Weather station communication diagram
![Weather station communication diagram](https://raw.githubusercontent.com/SimpleMethod/Horizon/master/doc/client_wire_connections.png)

# 📈  Communication diagram
![enter image description here](https://raw.githubusercontent.com/SimpleMethod/Horizon/master/doc/Infrastructure.png)

# 🧊 Database schema
![enter image description here](https://raw.githubusercontent.com/SimpleMethod/Horizon/master/doc/database.png)

# 📙 Soil moisture sensor case
![Soil moisture sensor case](https://raw.githubusercontent.com/SimpleMethod/Horizon/master/doc/M_Case.png)

# 🧭 Wind vane 
Working on the measurement of change of resistance.
![enter image description here](https://raw.githubusercontent.com/SimpleMethod/Horizon/master/doc/W_Case.png)

## Visualization of PCB
![Wind vane pcb](https://raw.githubusercontent.com/SimpleMethod/Horizon/master/doc/W_Top.png)
