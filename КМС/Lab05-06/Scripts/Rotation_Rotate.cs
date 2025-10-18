using UnityEngine;

public class Rotation_Rotate : MonoBehaviour
{

    public float AngularSpeedY = 90f;

    void Update()
    {
        transform.Rotate(0f, AngularSpeedY * Time.deltaTime, 0f, Space.Self);
    }
}
