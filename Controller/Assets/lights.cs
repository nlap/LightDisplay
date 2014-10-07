using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using UnityOSC;

public class lights : MonoBehaviour {
	static int lastX = 0;
	static int lastY = 0;

	public GameObject cube;
	private Dictionary<string, ServerLog> servers;
    
	// Starts OSC and the server list
	void Start() {

		//draws the cubes
		for(int x = 0; x < 20; x++){
			for(int y = 0; y < 7; y++){
				GameObject clone = (GameObject)Instantiate(cube, new Vector3(x,y,0), Quaternion.Euler(270,0,0));
				clone.name = "cube" + x + "x" + y;
			}
		}

		//don't change this
		OSCHandler.Instance.Init();
		servers = new Dictionary<string, ServerLog>();
        
	}

	// Handles data
	void Update() {

		//don't change this
	    OSCHandler.Instance.UpdateLogs();
	    servers = OSCHandler.Instance.Servers;
		//

		//keep this loop
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
				//

				Debug.Log(gridX + " " + gridY);

				//this is for testing purposes, just change color
				GameObject lastCube = GameObject.Find("cube" + lastX + "x" + lastY);
				GameObject currentCube = GameObject.Find("cube" + gridX + "x" + gridY);

				lastCube.renderer.material.color = Color.white;
				currentCube.renderer.material.color = Color.red;

				lastX = gridX;
				lastY = gridY;
				//
	        }
	    }
	} 
}