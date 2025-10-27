using UnityEngine;

public class LightTrigger : MonoBehaviour
{
    public Light Point;

	private void OnTriggerEnter(Collider col)
	{
		if (col.name == "player")
		{
			Point.enabled = true;
		}
	}

	private void OnTriggerExit(Collider col)
	{
		if (col.name == "player")
		{
			Point.enabled = false;
		}
	}
}
