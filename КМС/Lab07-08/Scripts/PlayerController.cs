using UnityEngine;

public class PlayerController : MonoBehaviour
{
	public float MoveSpeed = 5f;
	public float sensitivity = 10f;

	void Update()
	{
		float x = Input.GetAxis("Horizontal");
		float z = Input.GetAxis("Vertical");

		Vector3 move = new Vector3(x, 0, z) * MoveSpeed * Time.deltaTime; 
		transform.Translate(move, Space.Self);

		float mouseX = Input.GetAxis("Mouse X");
		transform.Rotate(0, mouseX * sensitivity * Time.deltaTime, 0);
	}
}
