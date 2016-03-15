using UnityEngine;
using System.Collections;

public class Movement : MonoBehaviour {

	private float xSpeed;
	private float ySpeed;

	// Use this for initialization
	void Start () {
		xSpeed = Input.GetAxis("Horizontal");
		ySpeed = Input.GetAxis("Vertical");
	}
	
	// Update is called once per frame
	void Update () {
	
	}
	
	void FixedUpdate() {
		if (Input.GetAxisRaw ("Horizontal") == -1) {
			xSpeed = -5f;
		} else if (Input.GetAxisRaw ("Horizontal") == 0) {
			xSpeed = 0f;
		} else if (Input.GetAxisRaw ("Horizontal") == 1) {
			xSpeed = 5f;
		}

		if (Input.GetAxisRaw ("Jump") == 1) {
			ySpeed = 3f;
		}
		GetComponent<Rigidbody2D> ().velocity = new Vector2 (xSpeed, ySpeed);
	}
}
