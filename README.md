arduino-projects-wuparser
=========================

Parser WeatherUnderground to fetch the amount of rain for the day.

It includes 3 pieces really
1. Generic simple JSON document parser
2. WeatherUnderground client for Arduino environment
3. Arduino DHCP based Ethernet client that uses the above

The generic JSON parser can extract a single entry from a JSON document, no matter how deep the entry is embedded.
In the case of JSON arrays, it will always pick the entry from the first member of the array. It is desinged for
environments with extreme memory challenges like Arduino that can not hold a single complete JSON reply of forecast.
The parser is a state machine that parses lines from the JSON document one by one, to cope with the memory challenge.

The WU client is straight forward based on the JSON parser + Arduiono EthernetClient

The Arduino .ino file is DHCP client and calls the WU client every 10 minutes

