using UnityEngine;

public class Input_GetAxis : MonoBehaviour
{

	public float MoveSpeed = 5f;
    public float sensitivity = 100f;

	private float rotationX = 0f;        

	void Start()
    {
        
    }

    void Update()
    {
        float x = Input.GetAxis("Horizontal");
		float z = Input.GetAxis("Vertical");

        transform.Translate(x * MoveSpeed * Time.deltaTime, 0, z * MoveSpeed * Time.deltaTime);

		float MouseX = Input.GetAxis("Mouse X");
		float MouseY = Input.GetAxis("Mouse Y");

        rotationX -= MouseX * sensitivity;
        rotationX = Mathf.Clamp(rotationX, 0f, 90f);

        transform.rotation = Quaternion.Euler(rotationX, transform.eulerAngles.y + MouseY * sensitivity * Time.deltaTime, 0f);
	}
}
