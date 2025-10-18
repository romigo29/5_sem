using UnityEngine;

public class ScriptPosition : MonoBehaviour
{

    public float SpeedX = 5f;
	public float SpeedY = 0.5f;
	public float SpeedZ = -1f;

    void Update()
    {
        transform.position += new Vector3(
            SpeedX * Time.deltaTime,
            SpeedY * Time.deltaTime,
            SpeedZ * Time.deltaTime
            );
    }
}
