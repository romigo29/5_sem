using UnityEngine;

public class Rotation_Quaternion : MonoBehaviour
{

	public float SpeedX = 90f;
	public float SpeedZ = 60f;

    private Quaternion StartPosition;
	private float AngleX;
	private float AngleZ;

	void Start()
    {
        StartPosition = transform.rotation;
    }

    void Update()
    {
		AngleX += SpeedX * Time.deltaTime;
		AngleZ += SpeedZ * Time.deltaTime;

		Quaternion rotationX = Quaternion.AngleAxis(AngleX, Vector3.right);
		Quaternion rotationZ = Quaternion.AngleAxis(AngleZ, Vector3.forward);

		transform.rotation = StartPosition * rotationX * rotationZ;

	}
}
