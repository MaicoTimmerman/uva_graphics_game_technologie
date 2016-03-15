using UnityEngine;
using System.Collections;

public class Movement : MonoBehaviour {

	private float xSpeed;
	private float ySpeed;

	// Use this for initialization
	void Start () {
		xSpeed = Input.GetAxis("HorizontalA");
		ySpeed = Input.GetAxis("Vertical");
	}
	
	// Update is called once per frame
	void Update () {
	
	}
	
	void FixedUpdate() {
		// float speed = 0f;
		if (Input.GetButtonDown ("HorizontalA")) {
			xSpeed -= 5f;
		}
		if (Input.GetButtonDown ("HorizontalD")) {
			xSpeed += 5f;
		}
		if (Input.GetButtonUp ("HorizontalD") || Input.GetButtonUp ("HorizontalA")) {
			xSpeed = 0f;
		}
		// xSpeed = speed;
		if (xSpeed > 5f) {
			xSpeed = 5f;
		} else if (xSpeed < -5f) {
			xSpeed = -5f;
		}
			
		GetComponent<Rigidbody2D> ().velocity = new Vector2 (xSpeed, 0f * ySpeed);
	}
}
