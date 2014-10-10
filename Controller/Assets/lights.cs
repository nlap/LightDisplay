// Beach Pong - Nocturne 2014, Halifax, NS
//
// Light display controller
// Updates the large arduino-controlled light grid with game status
//
// Arthur Silva Bastos, Nathan Lapierre
// GPLv3 - See LICENSE

using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using UnityOSC;
using System.IO.Ports;

public class lights : MonoBehaviour {
	static int lastX = 0;
	static int lastY = 0;

	public GameObject cube;
	private Dictionary<string, ServerLog> servers;
	private SerialPort port;
    
	// Starts OSC and the server list
	void Start() {

		//draws the cubes
		for(int x = 0; x < 20; x++){
			for(int y = 0; y < 7; y++){
				GameObject clone = (GameObject)Instantiate(cube, new Vector3(x,y,0), Quaternion.Euler(270,0,0));
				clone.name = "cube" + x + "x" + y;
			}
		}

		//open serial port to Arduino
		port = new SerialPort("/dev/tty.usbserial-A100evJ6", 115200);
		port.Open();

		//init OSC to recieve messages from game server
		OSCHandler.Instance.Init();
		servers = new Dictionary<string, ServerLog>();
        
	}

	//close open port on exit
	~lights() {
		port.Close();
	}

	// Handles data
	void Update() {

		//receive OSC message from game server with status
	    OSCHandler.Instance.UpdateLogs();
	    servers = OSCHandler.Instance.Servers;

		String lightArray = "";

	    foreach (KeyValuePair<string, ServerLog> item in servers)
	    {
	        if (item.Value.log.Count > 0)
	        {
	            int lastPacketIndex = item.Value.packets.Count - 1;
					
				//use those lines to get X and Y position
				//there is a threshold because the real ball position is a float
				//(and yes, 0 means Y and 1 means X, keep it inverted)
				int gridX = ((int)item.Value.packets[lastPacketIndex].Data[1]+21)/2;
				if(gridX > 19) gridX = 19;
				else if (gridX < 0) gridX = 0;
				

				int gridY = ((int)item.Value.packets[lastPacketIndex].Data[0]+12)/4;
				if(gridY > 6) gridY = 6;
				else if (gridY < 0) gridY = 0;

				Debug.Log(gridX + " " + gridY);

				//show ball position on representation of light display
				GameObject lastCube = GameObject.Find("cube" + lastX + "x" + lastY);
				GameObject currentCube = GameObject.Find("cube" + gridX + "x" + gridY);

				lastCube.renderer.material.color = Color.white;
				currentCube.renderer.material.color = Color.red;

				lastX = gridX;
				lastY = gridY;

				//build array of light values to send to arduino
				for (int i=0; i<=200; i++) {
					if (i == gridX * gridY) {
						lightArray += "ppp";
					} else {
						lightArray += "aaa";
					}
				}

	        }
	    }

		//send the light values
		port.Write(lightArray);

	} 
}