using UnityEngine;

public class SpotTrigger : MonoBehaviour
{
	public Light Spot;

	private void OnTriggerStay(Collider col)
	{
		if (col.name == "player")
		{
			Spot.transform.Rotate(Vector3.up * 100f * Time.deltaTime);
		}
	}
}
