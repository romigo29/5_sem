using UnityEngine;

public class RobotController : MonoBehaviour
{
	void Update()
	{
		float moveX = 0, moveZ = 0;
		if (Input.GetKey(KeyCode.E)) moveZ = 1;
		if (Input.GetKey(KeyCode.R)) moveZ = -1;
		if (Input.GetKey(KeyCode.T)) moveX = -1;
		if (Input.GetKey(KeyCode.Y)) moveX = 1;
		transform.Translate(new Vector3(moveX, 0, moveZ) * 3f * Time.deltaTime);
	}

}
