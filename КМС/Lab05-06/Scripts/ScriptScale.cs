using UnityEngine;

public class ScriptScale : MonoBehaviour
{
	public float ScaleSpeedX = 0.5f;
	public float ScaleSpeedY = 0.5f;

	void Update()
	{
		Vector3 s = transform.localScale;

		s.x += ScaleSpeedX * Time.deltaTime;
		s.y += ScaleSpeedY * Time.deltaTime;

		transform.localScale = s;

	}
}
