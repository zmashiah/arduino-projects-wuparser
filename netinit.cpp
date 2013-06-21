/* ===============================================================================
 *	Network Initialization code for Arduino Ethernet
 * ===============================================================================
 * By: Zakie Mashiah		zmashiah@gmail.com			June-15-2013
 * You may use, modify, publish this code for any type of use provided that
 * you keep a visible indication of the author
 * ===============================================================================
*/
#include <Ethernet.h>

// If DHCP will fail, our fallback it static IP address. The below variables are the
// network data for static IP
IPAddress _stipAddr(10, 0, 0, 177);	// Static IP addess	: At my home 10.0.0.[1-99] are allocated by DHCP on DSL router
IPAddress _dnsAddr(10, 0, 0, 138);	// Static IP DNS IP Address: And of course it is also the DNS
IPAddress _sbnmAddr(255, 0, 0, 0);		// Static IP Subnet mask Address: On 10 network, change if you move to 192.168
IPAddress _dgwAddr(10, 0, 0, 138);	// Static IP Default Gateway : At my home this is the DSL router IP


static void printAddr(IPAddress ip)
{
    for(int i=0; i < 4; i++)
    {
		Serial.print(ip[i], DEC);
		Serial.print('.');
	}
	Serial.print(' ');
}

void initNetwork()
{
	// This is our MAC addess, if you have more than one board, make sure to have each one set with different MAC address
	byte _macAddress[] = { 0xDe, 0xad, 0xBe, 0xef, 0x00, 0x07 };  // MAC Address of the board
	Serial.print("InitNetwork...");
	if (Ethernet.begin(_macAddress) == 0)
	{
		Serial.println("\tDHCP failed, using static IP:");
		//for ( ; ; );	// infinite loop or try static IP address as below
                Ethernet.begin(_macAddress, _stipAddr, _dnsAddr, _dgwAddr, _sbnmAddr);
	}
	else
		Serial.println("\tGot DHCP address:");
	Serial.print("\tMy IP     :\t"); printAddr(Ethernet.localIP() ); Serial.println();
	Serial.print("\tSubnet    :\t"); printAddr(Ethernet.subnetMask() ); Serial.println();
	Serial.print("\tDNS Server:\t"); printAddr(Ethernet.dnsServerIP() ); Serial.println();
	Serial.print("\tDefault GW:\t"); printAddr(Ethernet.gatewayIP() ); Serial.println();

	Serial.println("Network init complete");
	delay(2000);
}


