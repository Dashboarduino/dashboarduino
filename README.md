# dashboarduino


This repository houses the code for the Arduino used in this project. 

The communication with the instrument cluster is abstracted to a separate
class `MondeoDashboard` with the idea that in the future this could be
expanded to include other dashboards, possibly dynamically loaded.

This also includes the command structure for sending information to the
dashboard.