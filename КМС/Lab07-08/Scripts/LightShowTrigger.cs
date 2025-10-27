using UnityEngine;

public class LightShowTrigger : MonoBehaviour
{
	public Light point1, point2, point3;
	public GameObject cylinder;
	private float intensity = 0f;

	void OnTriggerStay(Collider col)
	{
		if (col.name == "player" || col.name == "robot")
		{
			intensity = Mathf.PingPong(Time.time * 20f, 20f);
			point1.intensity = intensity;
			point2.intensity = intensity;
			point3.intensity = intensity;

			cylinder.transform.Rotate(Vector3.up * 100f * Time.deltaTime);
		}
	}

}
