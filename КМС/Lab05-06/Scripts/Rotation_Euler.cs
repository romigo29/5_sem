using UnityEngine;

public class Rotation_Euler : MonoBehaviour
{

	public float SpeedX = 90f;
	public float SpeedZ = 60f;

	private float AngleX;
	private float AngleZ;

	void Update()
	{
		AngleX += SpeedX * Time.deltaTime;
		AngleZ += SpeedZ * Time.deltaTime;

		transform.eulerAngles = new Vector3(AngleX, 0f, AngleZ);
	}
}
