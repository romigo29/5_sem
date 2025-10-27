using Unity.VisualScripting.Antlr3.Runtime;
using UnityEngine;

public class GateTrigger : MonoBehaviour
{
    public GameObject LeftDoor;
    public GameObject RightDoor;
    private bool isOpen = false;

	public GameObject FlyingCube;
	public GameObject robot;
	private bool robotInTrigger = false;

	private void OnTriggerEnter(Collider col)
	{
		if (col.name == "player")
		{
			isOpen = true;
		}
	}

	private void OnTriggerExit(Collider col)
	{
		if (col.name == "player")
		{
			isOpen = false;
		}
	}

	private void OnTriggerStay(Collider col)
	{
		if (col.name == "robot")
		{
			FlyingCube.transform.Rotate(Vector3.up * 100f * Time.deltaTime);
			FlyingCube.transform.position += Vector3.right * Time.deltaTime;
		}
	}
	void Update()
    {
        if (isOpen)
		{
			LeftDoor.transform.localRotation = Quaternion.Lerp(LeftDoor.transform.localRotation,
				Quaternion.Euler(0, -90, 0), Time.deltaTime);
			RightDoor.transform.localRotation = Quaternion.Lerp(RightDoor.transform.localRotation,
				Quaternion.Euler(0, 90, 0), Time.deltaTime);
		}
		else
		{
			LeftDoor.transform.localRotation = Quaternion.Lerp(LeftDoor.transform.localRotation,
				Quaternion.Euler(0, 0, 0), Time.deltaTime);
			RightDoor.transform.localRotation = Quaternion.Lerp(RightDoor.transform.localRotation,
				Quaternion.Euler(0, 0, 0), Time.deltaTime);
		}
    }
}
