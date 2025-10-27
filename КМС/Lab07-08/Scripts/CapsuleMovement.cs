using UnityEngine;

public class CapsuleMovement : MonoBehaviour
{
	public float MoveSpeed = 5f;
	public float sensitivity = 50f;

	public GameObject Cube1;

	void Update()
	{
		float x = Input.GetAxis("Horizontal");
		float z = Input.GetAxis("Vertical");

		Vector3 move = new Vector3(x, 0, z) * MoveSpeed * Time.deltaTime;
		transform.Translate(move, Space.Self);

		float mouseX = Input.GetAxis("Mouse X");
		transform.Rotate(0, mouseX * sensitivity * Time.deltaTime, 0);

		if (Input.GetKeyDown(KeyCode.T))
		{
			Renderer rend = Cube1.GetComponent<Renderer>();
			Texture tex = Resources.Load<Texture>("glass");      
			rend.material.mainTexture = tex;
		}

	}

	private void OnCollisionEnter(Collision collision)
	{
		collision.gameObject.GetComponent<Renderer>().material.color = Color.red;

		Renderer rend = collision.gameObject.GetComponent<Renderer>();
		if (rend != null)
		{
			Texture tex = Resources.Load<Texture>("crack");
			rend.material.mainTexture = tex;
		}
	}
}
