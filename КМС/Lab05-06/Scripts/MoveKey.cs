using UnityEngine;

public class MoveKey : MonoBehaviour
{

	public float moveSpeed = 5f;

	void Update()
	{
		Vector3 move = Vector3.zero;

		if (Input.GetKey(KeyCode.W))	move.y += 1;
		if (Input.GetKey(KeyCode.S))	move.y -= 1;

		if (Input.GetKey(KeyCode.A))	move.x -= 1;
		if (Input.GetKey(KeyCode.D))	move.x += 1;

		if (Input.GetKey(KeyCode.Q))	move.z += 1;
		if (Input.GetKey(KeyCode.E))	move.z -= 1;

		transform.Translate(move * moveSpeed * Time.deltaTime);
	}
}
